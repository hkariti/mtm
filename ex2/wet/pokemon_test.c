#include <stdbool.h>
#include "aux_macros.h"
#include "pokemon.h"

static bool testCombo() {
	bool result = true;
	Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	TEST_DIFFERENT(result, pikachu, NULL);
	Pokemon pikachu_copy = pokemonCopy(pikachu);
	TEST_DIFFERENT(result, pikachu_copy, NULL);
	pokemonDestroy(pikachu_copy);
	TEST_EQUALS(result, pokemonTeachMove(
		pikachu, "Thunder", TYPE_ELECTRIC, 10, 110), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonUnteachMove(pikachu, "Thunder"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonGetLevel(pikachu), 1);
	TEST_EQUALS(result, pokemonGetRank(pikachu), 0);
	Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 10, 4);
	pokemonTeachMove(squirtle, "Bubble", TYPE_WATER, 30, 40);
	TEST_EQUALS(result, pokemonUseMove(
		squirtle, pikachu, "Bubble"), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonHeal(pikachu), POKEMON_SUCCESS);
	TEST_EQUALS(result, pokemonEvolve(squirtle, "Wartortle"), POKEMON_SUCCESS);
	pokemonDestroy(pikachu);
	pokemonDestroy(squirtle);
	return result;
}

static bool testPokemonCreate() {
	bool result = true;

	Pokemon charizard;
	charizard = pokemonCreate("Charizard", (PokemonType)5, 9900, 10);
	TEST_EQUALS(result, charizard, NULL);
	charizard = pokemonCreate("Charizard", (PokemonType)-1, 9900, 10);
	TEST_EQUALS(result, charizard, NULL);
	charizard = pokemonCreate("Charizard", TYPE_FIRE, 9902, 10);
	TEST_EQUALS(result, charizard, NULL);
	charizard = pokemonCreate("Charizard", TYPE_FIRE, -1, 10);
	TEST_EQUALS(result, charizard, NULL);
	charizard = pokemonCreate("", TYPE_FIRE, 9900, 10);
	TEST_EQUALS(result, charizard, NULL);
	charizard = pokemonCreate(NULL, TYPE_FIRE, 9900, 10);
	TEST_EQUALS(result, charizard, NULL);
	charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, -1);
	TEST_EQUALS(result, charizard, NULL);

	charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, 10);
	TEST_DIFFERENT(result, charizard, NULL);

	return result;
}

static bool testPokemonDestroy() {
	bool result = true;

	free(NULL);
	Pokemon charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, 10);
	Pokemon charizardClone = pokemonCopy(charizard);
	pokemonTeachMove(charizard, "Fireball", TYPE_FIRE, 10, 5);

	free(charizard);
	free(charizardClone);

	return result;
}

static bool testPokemonCopy() {
	bool result = true;
	PokemonResult pokemon_result;

	Pokemon charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, 10);
	pokemonTeachMove(charizard, "Fireball", TYPE_FIRE, 10, 5);
	Pokemon charizardClone = pokemonCopy(charizard);
	TEST_DIFFERENT(result, charizardClone, NULL);
	pokemon_result = pokemonUnteachMove(charizardClone, "Fireball");
	TEST_EQUALS(result, pokemon_result, POKEMON_SUCCESS);

	return result;
}

static bool testPokemonTeachMove() {
	bool result = true;
	PokemonResult pokemon_result;
	Pokemon charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, 10);
	pokemon_result = pokemonTeachMove(charizard, "Fireball", TYPE_FIRE, 10, 5);
	TEST_EQUALS(result, pokemon_result, POKEMON_SUCCESS);
	pokemon_result = pokemonTeachMove(NULL, "", TYPE_GRASS, -10, -5);
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);
	pokemon_result = pokemonTeachMove(charizard, NULL, TYPE_GRASS, -10, -5);
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);
	pokemon_result = pokemonTeachMove(charizard, "", TYPE_GRASS, -10, -5);
	TEST_EQUALS(result, pokemon_result, POKEMON_INVALID_MOVE_NAME);
	pokemon_result = pokemonTeachMove(charizard, "Fireball", -1, -10, -5);
	TEST_EQUALS(result, pokemon_result, POKEMON_INVALID_TYPE);
	pokemon_result = pokemonTeachMove(charizard, "Fireball", TYPE_GRASS, 10, 5);
	TEST_EQUALS(result, pokemon_result, POKEMON_INCOMPATIBLE_MOVE_TYPE);
	pokemon_result = pokemonTeachMove(charizard, "Fireball", TYPE_NORMAL, -1, -5);
	TEST_EQUALS(result, pokemon_result, POKEMON_INVALID_POWER_POINTS);
	pokemon_result = pokemonTeachMove(charizard, "Fireball", TYPE_NORMAL, 10, -1);
	TEST_EQUALS(result, pokemon_result, POKEMON_INVALID_STRENGTH);
	pokemon_result = pokemonTeachMove(charizard, "Fireball", TYPE_FIRE, 10, 5);
	TEST_EQUALS(result, pokemon_result, POKEMON_MOVE_ALREADY_EXISTS);

	return result;
}

static bool testPokemonUnteachMove() {
	bool result = true;

	// Complete your code here...

	return result;
}

static bool testPokemonGetLevel() {
	bool result = true;

	// Complete your code here...

	return result;
}

static bool testPokemonGetRank() {
	bool result = true;

	// Complete your code here...

	return result;
}

static bool testPokemonUseMove() {
	bool result = true;

	// Complete your code here...

	return result;
}

static bool testPokemonHeal() {
	bool result = true;

	// Complete your code here...

	return result;
}

static bool testPokemonEvolve() {
	bool result = true;

	// Complete your code here...

	return result;
}

static bool testPokemonPrintName() {
	bool result = true;

	// Complete your code here...

	return result;
}

static bool testPokemonPrintVoice() {
	bool result = true;

	// Complete your code here...

	return result;
}

int main() {
	RUN_TEST(testCombo);
	RUN_TEST(testPokemonCreate);
	RUN_TEST(testPokemonDestroy);
	RUN_TEST(testPokemonCopy);
	RUN_TEST(testPokemonTeachMove);
	RUN_TEST(testPokemonUnteachMove);
	RUN_TEST(testPokemonGetLevel);
	RUN_TEST(testPokemonGetRank);
	RUN_TEST(testPokemonUseMove);
	RUN_TEST(testPokemonHeal);
	RUN_TEST(testPokemonEvolve);
	RUN_TEST(testPokemonPrintName);
	RUN_TEST(testPokemonPrintVoice);
	return 0;
}
