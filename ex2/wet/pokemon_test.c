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

	// Complete your code here...

	return result;
}

static bool testPokemonDestroy() {
	bool result = true;

	// Complete your code here...

	return result;
}

static bool testPokemonCopy() {
	bool result = true;

	// Complete your code here...

	return result;
}

static bool testPokemonTeachMove() {
	bool result = true;

	// Complete your code here...

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
