#include <stdlib.h>
#include <string.h>
#include "evolutions.h"

struct EvolutionEntry_t {
	PokedexEntry evolved_pokemon;
	int level;
};

static EvolutionEntry evolutionEntryCreate(PokedexEntry evolved_pokemon,
                                           int level) {
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
    EvolutionEntry new_entry;
    new_entry = evolutionEntryCreate(original_entry->evolved_pokemon,
            original_entry->level);
    if (NULL == new_entry) return NULL;

    return new_entry;
}

static char* evolutionsKeyCopy(char* key) {
    int length = strlen(key);
    char* key_copy = malloc(length + 1);
    if (NULL == key_copy) return NULL;
    strcpy(key_copy, key);
    return key_copy;
}

static int evolutionsKeyCmp(MapKeyElement str1, MapKeyElement str2) {
    return strcmp((char*)str1, (char*)str2);
}

Evolutions createEvolutions() {
    Map evolutions;
    evolutions = mapCreate((copyMapKeyElements)evolutionsKeyCopy,
                           (copyMapDataElements)evolutionEntryCopy,
                           free,
                           free,
                           evolutionsKeyCmp);

    return evolutions;
}

void destroyEvolutions(Evolutions evolutions) {
  mapDestroy(evolutions);
}

bool evolutionsAddEntry(Evolutions evolutions, char* pokemon_to_evolve,
                        int level, PokedexEntry evolved_pokemon) {
  EvolutionEntry entry = evolutionEntryCreate(evolved_pokemon, level);
  if (NULL == entry) return false;
  MapResult put_result = mapPut(evolutions, pokemon_to_evolve, entry);
  if (MAP_SUCCESS != put_result) return false;

  evolutionEntryDestroy(entry);
  return true;
}

PokedexEntry getEvolution(Evolutions evolutions, char* pokemon_to_evolve,
                          int level) {
  EvolutionEntry evolution_entry = mapGet(evolutions, pokemon_to_evolve);
  if (NULL == evolution_entry || level < evolution_entry->level) return NULL;
  return evolution_entry->evolved_pokemon;
}
