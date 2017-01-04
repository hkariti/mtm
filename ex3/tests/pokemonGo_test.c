
#include "../pokemonGo.h"
#include "../utils.h"
#include "mocks/evolutions_mocks.h"
#include "mocks/pokedex_mocks.h"
#include "mocks/location_mocks.h"
#include "mocks/pokemon_mocks.h"
#include "mocks/trainer_mocks.h"
#include "test_utilities.h"

#define POKEMONGO_SET_UP()                              \
  Pokedex pokedex = demoPokedex();                    \
  Evolutions evolutions = demoEvolutions(pokedex);           \
  Map locations = demoLocations(pokedex, evolutions);

#define POKEMONGO_TEAR_DOWN()                     \
  evolutionsDestroy(evolutions);                \
  pokedexDestroy(pokedex);                      \
  mapDestroy(locations);


bool testCreatePokemonGo() {
	POKEMONGO_SET_UP();
	PokemonGo pokemonGo;

	// Invalid inputs
	pokemonGo = pokemonGoCreate(NULL, NULL, NULL, NULL);
	ASSERT_TEST(NULL == pokemonGo);
	pokemonGo = pokemonGoCreate(NULL, evolutions, locations, stdout);
	ASSERT_TEST(NULL == pokemonGo);
	pokemonGo = pokemonGoCreate(pokedex, NULL, locations, stdout);
	ASSERT_TEST(NULL == pokemonGo);
	pokemonGo = pokemonGoCreate(pokedex, evolutions, NULL, stdout);
	ASSERT_TEST(NULL == pokemonGo);
	pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, NULL);
	ASSERT_TEST(NULL == pokemonGo);

	// Valid inputs
	pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, stdout);
	ASSERT_TEST(pokemonGo != NULL);

	pokemongoDestroy(pokemonGo);
	POKEMONGO_TEAR_DOWN();
	return true;
}

bool testDestroyPokemonGo() {
	POKEMONGO_SET_UP();

	// NULL Input
	pokemongoDestroy(NULL);

	// Valid input
	PokemonGo pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, stdout);
	pokemongoDestroy(pokemonGo);

	POKEMONGO_TEAR_DOWN();
	return true;
}

bool testPokemonGoAddTrainer() {
	POKEMONGO_SET_UP();
	PokemonGoErrorCode result;
	PokemonGo pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, stdout);

	// invalid budget 
	result = pokemongoTrainerAdd(pokemonGo, "Ash", -100, "madagascar");
	ASSERT_TEST(result == POKEMONGO_INVALID_ARGUMENT);

	// invalid location
	result = pokemongoTrainerAdd(pokemonGo, "Ash", 0, "madagascar");
	ASSERT_TEST(result == POKEMONGO_LOCATION_DOES_NOT_EXIST);

	// success
	result = pokemongoTrainerAdd(pokemonGo, "Ash", 0, "paris");
	ASSERT_TEST(result == POKEMONGO_SUCCESS);

	// trainer already exist
	result = pokemongoTrainerAdd(pokemonGo, "Ash", 0, "madagascar");
	ASSERT_TEST(result == POKEMONGO_TRAINER_NAME_ALREADY_EXISTS);
	
	pokemongoDestroy(pokemonGo);
	POKEMONGO_TEAR_DOWN();
	return true;
} 

bool testPokemonGoTrainerGo() {
	POKEMONGO_SET_UP();
	PokemonGoErrorCode result;
	PokemonGo pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, stdout);
	result = pokemongoTrainerAdd(pokemonGo, "ash", 0, "paris");

	result = pokemongoTrainerGo(pokemonGo, "bibi", "madagascar");
	ASSERT_TEST(result == POKEMONGO_TRAINER_DOES_NOT_EXIST);
	result = pokemongoTrainerGo(pokemonGo, "ash", "madagascar");
	ASSERT_TEST(result == POKEMONGO_LOCATION_DOES_NOT_EXIST);
	result = pokemongoTrainerGo(pokemonGo, "ash", "jerusalem");
	ASSERT_TEST(result == POKEMONGO_LOCATION_IS_NOT_REACHABLE);
	result = pokemongoTrainerGo(pokemonGo, "ash", "paris");
	ASSERT_TEST(result == POKEMONGO_TRAINER_ALREADY_IN_LOCATION);
	result = pokemongoTrainerGo(pokemonGo, "ash", "london");
	ASSERT_TEST(result == POKEMONGO_SUCCESS);

	// more deeper & complex tests can be found in trainer_test

	pokemongoDestroy(pokemonGo);
	POKEMONGO_TEAR_DOWN();
	return true;
}

bool testPokemonGoStoreAdd() {
	POKEMONGO_SET_UP();
	PokemonGoErrorCode result;
	PokemonGo pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, stdout);

	// invalid args
	result = pokemongoStoreAdd(pokemonGo, "snack", 10, 10);
	ASSERT_TEST(result == POKEMONGO_INVALID_ARGUMENT);
	result = pokemongoStoreAdd(pokemonGo, "potion", 0, 10);
	ASSERT_TEST(result == POKEMONGO_INVALID_ARGUMENT);
	result = pokemongoStoreAdd(pokemonGo, "potion", 10, 0);
	ASSERT_TEST(result == POKEMONGO_INVALID_ARGUMENT);

	// valid args
	result = pokemongoStoreAdd(pokemonGo, "candy", 10, 10);
	ASSERT_TEST(result == POKEMONGO_SUCCESS);
	result = pokemongoStoreAdd(pokemonGo, "potion", 10, 10);
	ASSERT_TEST(result == POKEMONGO_SUCCESS);

	// more deeper & complex tests can be found in store_test

	pokemongoDestroy(pokemonGo);
	POKEMONGO_TEAR_DOWN();
	return true;
}


bool testPokemonGoTrainerPurchase() {
	POKEMONGO_SET_UP();
	PokemonGoErrorCode result;
	PokemonGo pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, stdout);

	result = pokemongoTrainerAdd(pokemonGo, "ash", 10000, "paris");
	result = pokemongoTrainerAdd(pokemonGo, "trump", 0, "jerusalem");

	// Invalid argument
	result = pokemongoTrainerPurchase(pokemonGo, "putin", "missle", 1);
	ASSERT_TEST(result == POKEMONGO_INVALID_ARGUMENT);
	result = pokemongoTrainerPurchase(pokemonGo, "putin", "potion", 0);
	ASSERT_TEST(result == POKEMONGO_INVALID_ARGUMENT);

	// trainer doesn't exist
	result = pokemongoTrainerPurchase(pokemonGo, "obama", "potion", 1);
	ASSERT_TEST(result == POKEMONGO_TRAINER_DOES_NOT_EXIST);

	// out of stock
	result = pokemongoTrainerPurchase(pokemonGo, "trump", "potion", 1);
	ASSERT_TEST(result == POKEMONGO_ITEM_OUT_OF_STOCK);

	// budget not sufficient
	result = pokemongoStoreAdd(pokemonGo, "potion", 1, 10);
	result = pokemongoStoreAdd(pokemonGo, "candy", 3, 10);
	result = pokemongoTrainerPurchase(pokemonGo, "trump", "potion", 1);
	ASSERT_TEST(result == POKEMONGO_BUDGET_IS_NOT_SUFFICIENT);

	// clean the store
	for (int i = 0; i < 10; i++) {
		result = pokemongoTrainerPurchase(pokemonGo, "ash", "potion", 1);
		result = pokemongoTrainerPurchase(pokemonGo, "ash", "candy", 3);
	}
	ASSERT_TEST(result == POKEMONGO_SUCCESS);

	// out of stock again
	result = pokemongoTrainerPurchase(pokemonGo, "ash", "potion", 1);
	ASSERT_TEST(result == POKEMONGO_ITEM_OUT_OF_STOCK);
	result = pokemongoTrainerPurchase(pokemonGo, "ash", "candy", 3);
	ASSERT_TEST(result == POKEMONGO_ITEM_OUT_OF_STOCK);

	// more deeper & complex tests can be found in store_test, trainer_test

	pokemongoDestroy(pokemonGo);
	POKEMONGO_TEAR_DOWN();
	return true;
}


bool testPokemonGoHealPokemon() {
	POKEMONGO_SET_UP();
	PokemonGoErrorCode result;
	PokemonGo pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, stdout);

	result = pokemongoTrainerAdd(pokemonGo, "ash", 10000, "full_location");
	result = pokemongoTrainerAdd(pokemonGo, "putin", 0, "full_location");
	result = pokemongoTrainerGo(pokemonGo, "ash", "empty_location");
	result = pokemongoTrainerGo(pokemonGo, "ash", "full_location");

	// Invalid input
	result = pokemongoPokemonHeal(pokemonGo, "obama", 10);
	ASSERT_TEST(POKEMONGO_TRAINER_DOES_NOT_EXIST == result);
	result = pokemongoPokemonHeal(pokemonGo, "ash", 10);
	ASSERT_TEST(POKEMONGO_POKEMON_DOES_NOT_EXIST == result);
	result = pokemongoPokemonHeal(pokemonGo, "ash", 1);
	ASSERT_TEST(POKEMONGO_HP_IS_AT_MAX == result);
	result = pokemongoBattleFight(pokemonGo, "ash", "putin", 1, 1);
	result = pokemongoPokemonHeal(pokemonGo, "ash", 1);
	ASSERT_TEST(POKEMONGO_NO_AVAILABLE_ITEM_FOUND == result);

	// valid input
	result = pokemongoStoreAdd(pokemonGo, "potion", 10, 1);
	result = pokemongoTrainerPurchase(pokemonGo, "ash", "potion", 10);
	result = pokemongoPokemonHeal(pokemonGo, "ash", 1);
	ASSERT_TEST(POKEMONGO_SUCCESS == result);

	// more deeper & complex tests can be found in pokemon_test, trainer_test

	pokemongoDestroy(pokemonGo);
	POKEMONGO_TEAR_DOWN();
	return true;
}

bool testPokemonGoTrainPokemon() {
	POKEMONGO_SET_UP();
	PokemonGoErrorCode result;
	PokemonGo pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, stdout);

	result = pokemongoTrainerAdd(pokemonGo, "ash", 10000, "full_location");

	// Invalid input
	result = pokemongoPokemonTrain(pokemonGo, "obama", 2);
	ASSERT_TEST(POKEMONGO_TRAINER_DOES_NOT_EXIST == result);
	result = pokemongoPokemonTrain(pokemonGo, "ash", 2);
	ASSERT_TEST(POKEMONGO_POKEMON_DOES_NOT_EXIST == result);
	result = pokemongoPokemonTrain(pokemonGo, "ash", 1);
	ASSERT_TEST(POKEMONGO_NO_AVAILABLE_ITEM_FOUND == result);

	// valid input
	result = pokemongoStoreAdd(pokemonGo, "candy", 10, 1);
	result = pokemongoTrainerPurchase(pokemonGo, "ash", "candy", 10);
	result = pokemongoPokemonTrain(pokemonGo, "ash", 1);
	ASSERT_TEST(POKEMONGO_SUCCESS == result);

	// more deeper & complex tests can be found in pokemon_test, trainer_test

	pokemongoDestroy(pokemonGo);
	POKEMONGO_TEAR_DOWN();
	return true;
}

bool testPokemonGosBattleFight() {
	POKEMONGO_SET_UP();
	PokemonGoErrorCode result;
	PokemonGo pokemonGo = pokemonGoCreate(pokedex, evolutions, locations, stdout);

	result = pokemongoTrainerAdd(pokemonGo, "obama", 100000, "full_location");
	result = pokemongoTrainerAdd(pokemonGo, "putin", 100000, "full_location");

	// Invalid input
	result = pokemongoBattleFight(pokemonGo, "obama", "trump", 1, 1);
	ASSERT_TEST(POKEMONGO_TRAINER_DOES_NOT_EXIST == result);
	result = pokemongoBattleFight(pokemonGo, "obama", "putin", 1, 2);
	ASSERT_TEST(POKEMONGO_POKEMON_DOES_NOT_EXIST == result);
	result = pokemongoBattleFight(pokemonGo, "trump", "trump", 10, 10);
	ASSERT_TEST(POKEMONGO_INVALID_ARGUMENT == result);

	//valid arguments
	result = pokemongoBattleFight(pokemonGo, "obama", "putin", 1, 1);
	ASSERT_TEST(POKEMONGO_SUCCESS == result);

	// more deeper & complex tests can be found in pokemon_test, trainer_test

	pokemongoDestroy(pokemonGo);
	POKEMONGO_TEAR_DOWN();
	return true;
}

int main() {
	RUN_TEST(testCreatePokemonGo);
	RUN_TEST(testDestroyPokemonGo);
	RUN_TEST(testPokemonGoAddTrainer);
	RUN_TEST(testPokemonGoTrainerGo);
	RUN_TEST(testPokemonGoStoreAdd);
	RUN_TEST(testPokemonGoTrainerPurchase);
	RUN_TEST(testPokemonGoHealPokemon);
	RUN_TEST(testPokemonGoTrainPokemon);
	RUN_TEST(testPokemonGosBattleFight);
	return 0;
}
