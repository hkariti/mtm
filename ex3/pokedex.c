#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "pokedex.h"
#include "map.h"
#include "utils.h"

struct PokedexEntry_t {
	char* species;
	int cp;
	Set types;
};

/**
 * pokedexEntryDestroy - Deallocates a pokedexEntry and frees its resources.
 *
 * @param entry - a pokedex entry. If entry is NULL nothing will be done.
 */
static void pokedexEntryDestroy(PokedexEntry entry) {
  if (NULL == entry) return;
  setDestroy(entry->types);
  free(entry->species);
  free(entry);
}

/**
 * pokedexEntryCreate - Create a new pokedex entry
 *
 * @param species   - the pokemon species
 * @param cp        - the pokemon intial cp value
 * @param types     - a set of uppercase strings, representing the types
 *                    of the pokemon
 *
 * @return
 *  NULL - alloc failure or null argument
 *  A new pokedexEntry on success.
 */
static PokedexEntry pokedexEntryCreate(char* species, int cp,
                                       Set types) {
  if (NULL == species || NULL == types) return NULL;
  PokedexEntry entry = malloc(sizeof(struct PokedexEntry_t));
  if (NULL == entry) return NULL;
  char *species_copy = stringCopy(species);
  Set types_copy = setCopy(types);
  entry->species = species_copy;
  entry->cp = cp;
  entry->types = types_copy;
  if (NULL == species_copy || NULL  == types_copy) {
    pokedexEntryDestroy(entry);
    return NULL;
  }
  return entry;
}

/**
 * pokedexEntryCopy - copy a pokedexEntry
 *
 * @param original_entry - A pokedexEntry to copy
 *
 * @return
 *   NULL - null argument or memory error
 *   A new pokedexEntry, with the same species, cp and types on success.
 *   A copy will be made of all of these.
 */
static PokedexEntry pokedexEntryCopy(PokedexEntry original_entry) {
  if (NULL == original_entry) return NULL;
  PokedexEntry new_entry;
  new_entry = pokedexEntryCreate(original_entry->species, original_entry->cp,
                                 original_entry->types);
  return new_entry;
}

Pokedex pokedexCreate() {
  Map pokedex;
  pokedex = mapCreate((copyMapKeyElements)stringCopy,
                      (copyMapDataElements)pokedexEntryCopy,
                      (freeMapKeyElements)free,
                      (freeMapDataElements)pokedexEntryDestroy,
                      (compareMapKeyElements)stringCompare);
  return pokedex;
}

void pokedexDestroy(Pokedex pokedex) {
  mapDestroy(pokedex);
}

PokedexErrorCode pokedexAddPokemon(Pokedex pokedex, char* species, int cp,
                            Set types) {
  if (NULL == pokedex || NULL == species || NULL == types) {
    return POKEDEX_INVALID_ARGUMENT;
  }
  PokedexEntry entry;
  MapResult put_result;
  entry = pokedexEntryCreate(species, cp, types);
  if (NULL == entry) return POKEDEX_OUT_OF_MEMORY;
  put_result = mapPut(pokedex, species, entry);
  assert(put_result != MAP_NULL_ARGUMENT);
  pokedexEntryDestroy(entry);
  if (MAP_OUT_OF_MEMORY == put_result) return POKEDEX_OUT_OF_MEMORY;
  return POKEDEX_SUCCESS;
}

PokedexEntry pokedexGetPokemonInfo(Pokedex pokedex, char* species) {
  if (NULL == pokedex || NULL == species) return NULL;
  return mapGet(pokedex, species);
}

char* pokedexEntryGetSpecies(PokedexEntry entry) {
  if (NULL == entry) return NULL;
  return entry->species;
}

int pokedexEntryGetCp(PokedexEntry entry) {
  if (NULL == entry) return -1;
  return entry->cp;
}

Set pokedexEntryGetTypes(PokedexEntry entry) {
  if (NULL == entry) return NULL;
  return entry->types;
}
