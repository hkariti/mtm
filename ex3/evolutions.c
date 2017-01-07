#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "evolutions.h"
#include "utils.h"

// The internal data type of the evolutions map
typedef struct EvolutionEntry_t {
  PokedexEntry evolved_pokemon;
  int level;
} *EvolutionEntry;

/** 
 * evolutionEntryCreate - create a new evolution entry
 *
 * @param evolved_pokemon - Evolved form of the entry's pokemon
 * @param level           - Required level of the entry's pokemon to
 *                          evolve to evolved_pokemon
 *
 * @return
 *  NULL - NULL arg was passed or allocation error
 *  A new entry on success.
 */
static EvolutionEntry evolutionEntryCreate(PokedexEntry evolved_pokemon,
                                           int level) {
  if (NULL == evolved_pokemon || level < 0) return NULL;
  EvolutionEntry entry;
  entry = malloc(sizeof(struct EvolutionEntry_t));
  if (NULL == entry) return NULL;
  entry->evolved_pokemon = evolved_pokemon;
  entry->level = level;

  return entry;
}

/** 
 * evolutionEntryDestroy - Destroys an evolution entry.
 *
 * @param entry - Entry to destroy
 *
 * Note that the pokedexEntry for evolved_pokemon will not be freed. It's
 * assumed the pokedex type is managing these.
 */
static void evolutionEntryDestroy(EvolutionEntry entry) {
  free(entry);
}

/**
 * evolutionEntryCopy - Copy an evolution entry.
 *
 * @param entry - Entry to copy
 *
 * Note that the pokedexEntry for evolved_pokemon will be reused for the
 * copy. It's assumed the pokedex type is managing memory for the entries.
 *
 * @return
 *  NULL - a null arg was passed or memory error
 *  A new entry copy on success
 */
static EvolutionEntry evolutionEntryCopy(EvolutionEntry original_entry) {
  if (NULL == original_entry) return NULL;
  EvolutionEntry new_entry;
  new_entry = evolutionEntryCreate(original_entry->evolved_pokemon,
                                   original_entry->level);
  if (NULL == new_entry) return NULL;

  return new_entry;
}

Evolutions evolutionsCreate() {
  Map evolutions;
  evolutions = mapCreate((copyMapKeyElements)stringCopy,
                         (copyMapDataElements)evolutionEntryCopy,
                         (freeMapKeyElements)free,
                         (freeMapDataElements)evolutionEntryDestroy,
                         (compareMapKeyElements)stringCompare);

  return evolutions;
}

void evolutionsDestroy(Evolutions evolutions) {
  mapDestroy(evolutions);
}

EvolutionsErrorCode evolutionsAddEntry(Evolutions evolutions,
                                       char* pokemon_to_evolve, int level,
                                       PokedexEntry evolved_pokemon) {
  if (NULL == evolutions || NULL == pokemon_to_evolve ||
      NULL == evolved_pokemon || level < 0) {
    return EVOLUTIONS_INVALID_ARGUMENT;
  }
  EvolutionEntry entry = evolutionEntryCreate(evolved_pokemon, level);
  if (NULL == entry) return EVOLUTIONS_OUT_OF_MEMORY;
  MapResult put_result = mapPut(evolutions, pokemon_to_evolve, entry);
  assert(put_result != MAP_NULL_ARGUMENT);
  evolutionEntryDestroy(entry);
  if (MAP_OUT_OF_MEMORY == put_result) return EVOLUTIONS_OUT_OF_MEMORY;
  return EVOLUTIONS_SUCCESS;
}

PokedexEntry evolutionsGet(Evolutions evolutions, char* pokemon_to_evolve,
                          int level) {
  if (NULL == evolutions || NULL == pokemon_to_evolve || level < 0) return NULL;
  EvolutionEntry evolution_entry = mapGet(evolutions, pokemon_to_evolve);
  if (NULL == evolution_entry || level < evolution_entry->level) return NULL;
  return evolution_entry->evolved_pokemon;
}
