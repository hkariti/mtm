#include <string.h>
#include <stdlib.h>
#include "evolutions.h"
#include "utils.h"

struct EvolutionEntry_t {
	PokedexEntry evolved_pokemon;
	int level;
};

static EvolutionEntry evolutionEntryCreate(PokedexEntry evolved_pokemon,
                                           int level) {
  if (NULL == evolved_pokemon) return NULL;
  EvolutionEntry entry;
  entry = malloc(sizeof(struct EvolutionEntry_t));
  if (NULL == entry) return NULL;
  entry->evolved_pokemon = evolved_pokemon;
  entry->level = level;

  return entry;
}

static void evolutionEntryDestroy(EvolutionEntry entry) {
  free(entry);
}

static EvolutionEntry evolutionEntryCopy(EvolutionEntry original_entry) {
  if (NULL == original_entry) return NULL;
  EvolutionEntry new_entry;
  new_entry = evolutionEntryCreate(original_entry->evolved_pokemon,
                                   original_entry->level);
  if (NULL == new_entry) return NULL;

  return new_entry;
}

Evolutions createEvolutions() {
  Map evolutions;
  evolutions = mapCreate((copyMapKeyElements)stringCopy,
                         (copyMapDataElements)evolutionEntryCopy,
                         (freeMapKeyElements)free,
                         (freeMapDataElements)evolutionEntryDestroy,
                         (compareMapKeyElements)strcmp);

  return evolutions;
}

void destroyEvolutions(Evolutions evolutions) {
  mapDestroy(evolutions);
}

MapResult evolutionsAddEntry(Evolutions evolutions, char* pokemon_to_evolve,
                             int level, PokedexEntry evolved_pokemon) {
  if (NULL == evolutions || NULL == pokemon_to_evolve || NULL == evolved_pokemon) {
    return MAP_NULL_ARGUMENT;
  }
  EvolutionEntry entry = evolutionEntryCreate(evolved_pokemon, level);
  if (NULL == entry) return MAP_OUT_OF_MEMORY;
  MapResult put_result = mapPut(evolutions, pokemon_to_evolve, entry);
  evolutionEntryDestroy(entry);
  return put_result;
}

PokedexEntry getEvolution(Evolutions evolutions, char* pokemon_to_evolve,
                          int level) {
  if (NULL == evolutions || NULL == pokemon_to_evolve) return NULL;
  EvolutionEntry evolution_entry = mapGet(evolutions, pokemon_to_evolve);
  if (NULL == evolution_entry || level < evolution_entry->level) return NULL;
  return evolution_entry->evolved_pokemon;
}
