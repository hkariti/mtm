#include <string.h>
#include <stdlib.h>
#include "pokedex.h"
#include "map.h"
#include "utils.h"

static void pokedexEntryDestroy(PokedexEntry entry) {
  if (NULL == entry) return;
  setDestroy(entry->types);
  free(entry->species);
  free(entry);
}

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

static PokedexEntry pokedexEntryCopy(PokedexEntry original_entry) {
  if (NULL == original_entry) return NULL;
  PokedexEntry new_entry;
  new_entry = pokedexEntryCreate(original_entry->species, original_entry->cp,
                                 original_entry->types);
  return new_entry;
}

Pokedex createPokedex() {
  Map pokedex;
  pokedex = mapCreate((copyMapKeyElements)stringCopy,
                      (copyMapDataElements)pokedexEntryCopy,
                      (freeMapKeyElements)free,
                      (freeMapDataElements)pokedexEntryDestroy,
                      (compareMapKeyElements)strcmp);
  return pokedex;
}

void destroyPokedex(Pokedex pokedex) {
  mapDestroy(pokedex);
}

MapResult pokedexAddPokemon(Pokedex pokedex, char* species, int cp,
                            Set types) {
  if (NULL == pokedex || NULL == species || NULL == types) {
    return MAP_NULL_ARGUMENT;
  }
  PokedexEntry entry;
  MapResult put_result;
  entry = pokedexEntryCreate(species, cp, types);
  if (NULL == entry) return MAP_OUT_OF_MEMORY;
  put_result = mapPut(pokedex, species, entry);
  pokedexEntryDestroy(entry);
  return put_result;
}

PokedexEntry pokedexGetPokemonInfo(Pokedex pokedex, char* species) {
  if (NULL == pokedex || NULL == species) return NULL;
  return mapGet(pokedex, species);
}

char* pokedexEntryGetSpecies(PokedexEntry entry) {
  if (NULL == entry) return NULL;
  return stringCopy(entry->species);
}

int pokedexEntryGetCp(PokedexEntry entry) {
  if (NULL == entry) return -1;
  return entry->cp;
}

Set pokedexEntryGetTypes(PokedexEntry entry) {
  if (NULL == entry) return NULL;
  return setCopy(entry->types);
}
