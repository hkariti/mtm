#include <string.h>
#include <stdlib.h>
#include <pokedex.h>
#include <utils.h>
#include "test_utilities.h"

Set demoTypes() {
  Set types;

  types = setCreate((copySetElements)stringCopy, (freeSetElements)free,
                    (compareSetElements)strcmp);
  setAdd(types, "Fire");
  setAdd(types, "Electric");

  return types;
}

Pokedex demoPokedex() {
  Pokedex pokedex = createPokedex();
  Set types = demoTypes();
  pokedexAddPokemon(pokedex, "pikachu", 10, types);
  pokedexAddPokemon(pokedex, "Raichu", 12, types);
  setAdd(types, "Ghost");
  pokedexAddPokemon(pokedex, "Bulbasaur", 11, types);
  setDestroy(types);

  return pokedex;
}

bool testCreatePokedex() {
  Pokedex pokedex;

  pokedex = createPokedex();
  ASSERT_TEST(NULL != pokedex);

  destroyPokedex(pokedex);
  return true;
}

bool testDestroyPokedex() {
  Pokedex pokedex;

  // Test NULL argument
  destroyPokedex(NULL);

  // Test empty pokedex
  pokedex = createPokedex();
  destroyPokedex(pokedex);

  // Test with entries
  pokedex = demoPokedex();
  destroyPokedex(pokedex);

  return true;
}

bool testPokedexAddPokemon() {
  Pokedex pokedex = createPokedex();
  MapResult add_result;
  Set types = demoTypes();

  // Test NULL argument
  add_result = pokedexAddPokemon(NULL, "pikachu", 1, types);
  ASSERT_TEST(MAP_NULL_ARGUMENT == add_result);
  add_result = pokedexAddPokemon(pokedex, NULL, 1, types);
  ASSERT_TEST(MAP_NULL_ARGUMENT == add_result);
  add_result = pokedexAddPokemon(pokedex, "pikachu", 1, NULL);
  ASSERT_TEST(MAP_NULL_ARGUMENT == add_result);

  // Test new entry
  add_result = pokedexAddPokemon(pokedex, "pikachu", 1, types);
  ASSERT_TEST(MAP_SUCCESS == add_result);
  add_result = pokedexAddPokemon(pokedex, "raichu", 1, types);
  ASSERT_TEST(MAP_SUCCESS == add_result);

  // Test existing entry
  add_result = pokedexAddPokemon(pokedex, "pikachu", 2, types);
  ASSERT_TEST(MAP_SUCCESS == add_result);

  setDestroy(types);
  destroyPokedex(pokedex);
  return true;
}

bool testPokedexGetPokemonInfo() {
  Pokedex pokedex = demoPokedex();
  PokedexEntry ret;

  // Test NULL argument
  ret = pokedexGetPokemonInfo(NULL, "pikachu");
  ASSERT_TEST(NULL == ret);
  ret = pokedexGetPokemonInfo(pokedex, NULL);
  ASSERT_TEST(NULL == ret);

  // Test non-existing entry
  ret = pokedexGetPokemonInfo(pokedex, "articuno");
  ASSERT_TEST(NULL == ret);

  // Test existing entry
  ret = pokedexGetPokemonInfo(pokedex, "pikachu");
  ASSERT_TEST(NULL != ret);

  destroyPokedex(pokedex);
  return true;
}

bool testPokedexEntryGetSpecies() {
  Pokedex pokedex = demoPokedex();
  PokedexEntry entry = pokedexGetPokemonInfo(pokedex, "pikachu");
  char* species;

  // Test NULL argument
  species = pokedexEntryGetSpecies(NULL);
  ASSERT_TEST(NULL == species);


  // Valid input
  species = pokedexEntryGetSpecies(entry);
  ASSERT_TEST(NULL != species);
  free(species);

  destroyPokedex(pokedex);
  return true;
}

bool testPokedexEntryGetCp() {
  Pokedex pokedex = demoPokedex();
  PokedexEntry entry = pokedexGetPokemonInfo(pokedex, "pikachu");
  int cp;

  // Test NULL argument
  cp = pokedexEntryGetCp(NULL);
  ASSERT_TEST(-1 == cp);

  // Valid input
  cp = pokedexEntryGetCp(entry);
  ASSERT_TEST(cp >= 0);

  destroyPokedex(pokedex);
  return true;
}

bool testPokedexEntryGetTypes() {
  Pokedex pokedex = demoPokedex();
  PokedexEntry entry = pokedexGetPokemonInfo(pokedex, "pikachu");
  Set types;

  // Test NULL argument
  types = pokedexEntryGetTypes(NULL);
  ASSERT_TEST(NULL == types);

  // Valid input
  types = pokedexEntryGetTypes(entry);
  ASSERT_TEST(NULL != types);
  setDestroy(types);

  destroyPokedex(pokedex);
  return true;
}

int main() {
  RUN_TEST(testCreatePokedex);
  RUN_TEST(testDestroyPokedex);
  RUN_TEST(testPokedexAddPokemon);
  RUN_TEST(testPokedexGetPokemonInfo);
  RUN_TEST(testPokedexEntryGetSpecies);
  RUN_TEST(testPokedexEntryGetCp);
  RUN_TEST(testPokedexEntryGetTypes);

  return 0;
}
