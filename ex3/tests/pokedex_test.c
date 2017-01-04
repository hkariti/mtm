#include <string.h>
#include <stdlib.h>
#include "../pokedex.h"
#include "../utils.h"
#include "mocks/pokedex_mocks.h"
#include "test_utilities.h"

bool testCreatePokedex() {
  Pokedex pokedex;

  pokedex = pokedexCreate();
  ASSERT_TEST(NULL != pokedex);

  pokedexDestroy(pokedex);
  return true;
}

bool testDestroyPokedex() {
  Pokedex pokedex;

  // Test NULL argument
  pokedexDestroy(NULL);

  // Test empty pokedex
  pokedex = pokedexCreate();
  pokedexDestroy(pokedex);

  // Test with entries
  pokedex = demoPokedex();
  pokedexDestroy(pokedex);

  return true;
}

bool testPokedexAddPokemon() {
  Pokedex pokedex = pokedexCreate();
  PokedexErrorCode add_result;
  Set types = demoTypes();

  // Test NULL argument
  add_result = pokedexAddPokemon(NULL, "pikachu", 1, types);
  ASSERT_TEST(POKEDEX_INVALID_ARGUMENT == add_result);
  add_result = pokedexAddPokemon(pokedex, NULL, 1, types);
  ASSERT_TEST(POKEDEX_INVALID_ARGUMENT == add_result);
  add_result = pokedexAddPokemon(pokedex, "pikachu", 1, NULL);
  ASSERT_TEST(POKEDEX_INVALID_ARGUMENT == add_result);

  // Test new entry
  add_result = pokedexAddPokemon(pokedex, "pikachu", 1, types);
  ASSERT_TEST(POKEDEX_SUCCESS == add_result);
  add_result = pokedexAddPokemon(pokedex, "raichu", 1, types);
  ASSERT_TEST(POKEDEX_SUCCESS == add_result);

  // Test existing entry
  add_result = pokedexAddPokemon(pokedex, "pikachu", 2, types);
  ASSERT_TEST(POKEDEX_SUCCESS == add_result);

  setDestroy(types);
  pokedexDestroy(pokedex);
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

  pokedexDestroy(pokedex);
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

  pokedexDestroy(pokedex);
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

  pokedexDestroy(pokedex);
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

  pokedexDestroy(pokedex);
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
