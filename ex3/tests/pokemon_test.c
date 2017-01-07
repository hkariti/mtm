#include "test_utilities.h"
#include "../pokemon.h"
#include "../utils.h"
#include "mocks/evolutions_mocks.h"
#include "mocks/pokedex_mocks.h"
#include "mocks/pokemon_mocks.h"
#include <string.h>
#include <stdlib.h>

bool testCreatePokemon() {
  Pokedex pokedex = demoPokedex();
  PokedexEntry pokemon_info = pokedexGetPokemonInfo(pokedex, "pikachu");
  Evolutions evolutions = demoEvolutions(pokedex);
  Pokemon pokemon;

  // test null args
  pokemon = pokemonCreate(NULL, evolutions);
  ASSERT_TEST(NULL == pokemon);
  pokemon = pokemonCreate(NULL, NULL);
  ASSERT_TEST(NULL == pokemon);
  pokemon = pokemonCreate(pokemon_info, NULL);
  ASSERT_TEST(NULL == pokemon);

  // check valid args
  pokemon = pokemonCreate(pokemon_info, evolutions);
  ASSERT_TEST(NULL != pokemon);
  pokemonDestroy(pokemon);

  evolutionsDestroy(evolutions);
  pokedexDestroy(pokedex);
  return true;
}

bool testDestroyPokemon() {
  Pokedex pokedex = demoPokedex();
  Evolutions evolutions = demoEvolutions(pokedex);
  Pokemon pokemon;

  // Test NULL argument
  pokemonDestroy(NULL);

  // Test valid pokemon
  pokemon = demoPokemon(pokedex, evolutions);
  pokemonDestroy(pokemon);

  evolutionsDestroy(evolutions);
  pokedexDestroy(pokedex);
  return true;
}

bool testPokemonCompareByID() {
  int compare_result;
  Pokedex pokedex = demoPokedex();
  Evolutions evolutions = demoEvolutions(pokedex);

  Pokemon pokemon_1 = demoPokemon(pokedex, evolutions);
  Pokemon pokemon_2 = demoPokemon(pokedex, evolutions);

  // Test equal pokemon
  compare_result = pokemonCompareByID(pokemon_1, pokemon_2);
  ASSERT_TEST(0 == compare_result);

  // Test unequal pokemon
  pokemonCaught(pokemon_1, 100);
  pokemonCaught(pokemon_2, 10);
  compare_result = pokemonCompareByID(pokemon_1, pokemon_2);
  ASSERT_TEST(1 == compare_result);

  pokemonDestroy(pokemon_1);
  pokemonDestroy(pokemon_2);

  evolutionsDestroy(evolutions);
  pokedexDestroy(pokedex);

  return true;
}

bool testPokemonGivePotion() {
  Pokedex pokedex = demoPokedex();
  Evolutions evolutions = demoEvolutions(pokedex);
  double hp, prev_hp;
  PokemonErrorCode result;
  Pokemon pokemon = demoPokemon(pokedex, evolutions);
  Pokemon pokemon_2 = demoPokemon(pokedex, evolutions);

  // Test already at 100 hp
  result = pokemonGivePotion(pokemon, 1);
  hp = pokemonGetHP(pokemon);
  ASSERT_TEST(100 == hp);
  ASSERT_TEST(POKEMON_HP_IS_AT_MAX == result);

  // Test working scenario
  pokemonBattle(pokemon, pokemon_2, 10, 10);
  prev_hp = pokemonGetHP(pokemon);
  result = pokemonGivePotion(pokemon, 1);
  hp = pokemonGetHP(pokemon);
  ASSERT_TEST(prev_hp + 1 == hp);
  ASSERT_TEST(POKEMON_SUCCESS == result);

  // Test over-heal
  result = pokemonGivePotion(pokemon, 1000);
  hp = pokemonGetHP(pokemon);
  ASSERT_TEST(100 == hp);
  ASSERT_TEST(POKEMON_SUCCESS == result);

  pokemonDestroy(pokemon);
  pokemonDestroy(pokemon_2);
  evolutionsDestroy(evolutions);
  pokedexDestroy(pokedex);

  return true;
}

bool testPokemonCaught() {
  Pokedex pokedex = demoPokedex();
  Evolutions evolutions = demoEvolutions(pokedex);
  Set types = setCreate((copySetElements)stringCopy, (freeSetElements)free,
    (compareSetElements)stringCompare);
  int pokecoins;

  // Test regular types
  setAdd(types, "BUG");
  setAdd(types, "GROUND");
  pokedexAddPokemon(pokedex, "weedle", 1000, types);
  Pokemon weedle = pokemonCreate(pokedexGetPokemonInfo(pokedex, "weedle"),
                                   evolutions);
  pokecoins = pokemonCaught(weedle, 10);
  pokemonDestroy(weedle);
  ASSERT_TEST(pokecoins == REGULAR_TYPE_POKECOINS);

  // Test 2 one star types
  setAdd(types, "WATER");
  setAdd(types, "FAIRY");
  pokedexAddPokemon(pokedex, "tinkertoice", 1000, types);
  Pokemon tinkertoice;
    tinkertoice = pokemonCreate(pokedexGetPokemonInfo(pokedex, "tinkertoice"),
                                evolutions);
  pokecoins = pokemonCaught(tinkertoice, 10);
  pokemonDestroy(tinkertoice);
  ASSERT_TEST(pokecoins == ONE_STAR_TYPE_POKECOINS);


  // Test 2 double star types
  setAdd(types, "FIRE");
  setAdd(types, "LYING");
  pokedexAddPokemon(pokedex, "moltres", 1000, types);
  Pokemon moltres = pokemonCreate(pokedexGetPokemonInfo(pokedex, "moltres"),
                                    evolutions);
  pokecoins = pokemonCaught(moltres, 10);
  pokemonDestroy(moltres);
  ASSERT_TEST(pokecoins == DOUBLE_STAR_TYPE_POKECOINS);

  setDestroy(types);
  evolutionsDestroy(evolutions);
  pokedexDestroy(pokedex);
  return true;
}

bool testPokemonBattle() {
  Pokedex pokedex = demoPokedex();
  Evolutions evolutions = demoEvolutions(pokedex);
  Pokemon mew = pokemonCreate(pokedexGetPokemonInfo(pokedex, "mew"),
                                evolutions);
  Pokemon mewtwo = pokemonCreate(pokedexGetPokemonInfo(pokedex, "mewtwo"),
                                   evolutions);
  double mew_hp, mewtwo_hp;
  int mew_level, mewtwo_level;

  // Test mewtwo on mew battle
  pokemonBattle(mew, mewtwo, 90, 42);
  mew_hp = pokemonGetHP(mew);
  mew_level = pokemonGetLevel(mew);
  ASSERT_TEST(10 == mew_hp);
  ASSERT_TEST(2 == mew_level);
  mewtwo_hp = pokemonGetHP(mewtwo);
  mewtwo_level = pokemonGetLevel(mewtwo);
  ASSERT_TEST(58 == mewtwo_hp);
  ASSERT_TEST(2 == mewtwo_level);

  pokemonDestroy(mew);
  pokemonDestroy(mewtwo);
  evolutionsDestroy(evolutions);
  pokedexDestroy(pokedex);

  return true;
}

int main() {
  RUN_TEST(testCreatePokemon);
  RUN_TEST(testDestroyPokemon);
  RUN_TEST(testPokemonCompareByID);
  RUN_TEST(testPokemonGivePotion);
  RUN_TEST(testPokemonCaught);
  RUN_TEST(testPokemonBattle);
  return 0;
}

