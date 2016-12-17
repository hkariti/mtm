#include <string.h>
#include <stdlib.h>
#include <pokedex.h>
#include <utils.h>
#include "test_utilities.h"

Set demoTypes() {
  Set types;

  types = setCreate((copySetElements)stringCopy, (freeSetElements)free, (compareSetElements)strcmp);
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

bool testPokedexEntryStruct() {
  Pokedex pokedex = createPokedex();
  char* name = "magikarp";
  int cp = 0;
  Set types = demoTypes();
  PokedexEntry entry;

  pokedexAddPokemon(pokedex, name, cp, types);
  entry = pokedexGetPokemonInfo(pokedex, name);

  ASSERT_TEST(strcmp(entry->species, name) == 0);
  ASSERT_TEST(entry->cp == cp);
  ASSERT_TEST(setGetSize(types) == setGetSize(entry->types));
  SET_FOREACH(char*, type, types) {
    ASSERT_TEST(setIsIn(entry->types, type));
  }

  setDestroy(types);
  destroyPokedex(pokedex);
  return true;
}

int main() {
  RUN_TEST(testCreatePokedex);
  RUN_TEST(testDestroyPokedex);
  RUN_TEST(testPokedexAddPokemon);
  RUN_TEST(testPokedexGetPokemonInfo);
  RUN_TEST(testPokedexEntryStruct);

  return 0;
}
