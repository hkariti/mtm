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
	pokemonDestroy(charizard);
	charizard = pokemonCreate("Charizard", (PokemonType)-1, 9900, 10);
	TEST_EQUALS(result, charizard, NULL);
	pokemonDestroy(charizard);
	charizard = pokemonCreate("Charizard", TYPE_FIRE, 9902, 10);
	TEST_EQUALS(result, charizard, NULL);
	pokemonDestroy(charizard);
	charizard = pokemonCreate("Charizard", TYPE_FIRE, -1, 10);
	TEST_EQUALS(result, charizard, NULL);
	pokemonDestroy(charizard);
	charizard = pokemonCreate("", TYPE_FIRE, 9900, 10);
	TEST_EQUALS(result, charizard, NULL);
	pokemonDestroy(charizard);
	charizard = pokemonCreate(NULL, TYPE_FIRE, 9900, 10);
	TEST_EQUALS(result, charizard, NULL);
	pokemonDestroy(charizard);
	charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, -1);
	TEST_EQUALS(result, charizard, NULL);
	pokemonDestroy(charizard);
	charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, 10);
	TEST_DIFFERENT(result, charizard, NULL);
	pokemonDestroy(charizard);

	return result;
}

static bool testPokemonDestroy() {
	bool result = true;

	Pokemon charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, 10);
	Pokemon charizardClone = pokemonCopy(charizard);
	pokemonTeachMove(charizard, "Fireball", TYPE_FIRE, 10, 5);

	pokemonDestroy(NULL);
	pokemonDestroy(charizard);
	pokemonDestroy(charizardClone);

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

	pokemonDestroy(charizard);
	pokemonDestroy(charizardClone);


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

	pokemonDestroy(charizard);

	return result;
}

static bool testPokemonUnteachMove() {
	bool result = true;
	PokemonResult pokemon_result;

	Pokemon charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, 10);
	pokemonTeachMove(charizard, "Fireball", TYPE_FIRE, 10, 5);

	pokemon_result = pokemonUnteachMove(NULL, "");
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);
	pokemon_result = pokemonUnteachMove(charizard, NULL);
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);
	pokemon_result = pokemonUnteachMove(charizard, "");
	TEST_EQUALS(result, pokemon_result, POKEMON_INVALID_MOVE_NAME);
	pokemon_result = pokemonUnteachMove(charizard, "Firestorm");
	TEST_EQUALS(result, pokemon_result, POKEMON_MOVE_DOES_NOT_EXIST);
	pokemon_result = pokemonUnteachMove(charizard, "Fireball");
	TEST_EQUALS(result, pokemon_result, POKEMON_SUCCESS);
	pokemon_result = pokemonUnteachMove(charizard, "Fireball");
	TEST_EQUALS(result, pokemon_result, POKEMON_MOVE_DOES_NOT_EXIST);

	pokemonDestroy(charizard);

	return result;
}

static bool testPokemonGetLevel() {
	bool result = true;
	int level;

	Pokemon charizard = pokemonCreate("Charizard", TYPE_FIRE, 9900, 10);
	level = pokemonGetLevel(charizard);
	TEST_EQUALS(result, level, 99);
	pokemonDestroy(charizard);

	charizard = pokemonCreate("Charizard", TYPE_FIRE, 9901, 10);
	level = pokemonGetLevel(charizard);
	TEST_EQUALS(result, level, 100);
	pokemonDestroy(charizard);

	return result;
}

static bool testPokemonGetRank() {
	bool result = true;
	int rank;

	Pokemon charizard = pokemonCreate("Charizard", TYPE_FIRE, 1000, 10);
	rank = pokemonGetRank(charizard);
	TEST_EQUALS(result, rank, 0);
	pokemonTeachMove(charizard, "Fireball", TYPE_FIRE, 2, 10);
	rank = pokemonGetRank(charizard);
	TEST_EQUALS(result, rank, 20);
	pokemonTeachMove(charizard, "Firestorm", TYPE_FIRE, 2, 20);
	rank = pokemonGetRank(charizard);
	TEST_EQUALS(result, rank, 25);

	pokemonDestroy(charizard);

	return result;
}

static bool testPokemonUseMove() {
	bool result = true;
	PokemonResult pokemon_result = 0;

	Pokemon charizard = pokemonCreate("Charizard", TYPE_FIRE, 20, 10);
	Pokemon pidgey = pokemonCreate("Pidgey", TYPE_NORMAL, 10, 10);
	pokemonTeachMove(charizard, "Tackle", TYPE_NORMAL, 100, 10);
	pokemonTeachMove(charizard, "Tail-Whip", TYPE_NORMAL, 1, 10);

	pokemon_result = pokemonUseMove(charizard, pidgey, "Bite");
	TEST_EQUALS(result, pokemon_result, POKEMON_MOVE_DOES_NOT_EXIST);
	pokemon_result = pokemonUseMove(charizard, pidgey, "Tail-Whip");
	TEST_EQUALS(result, pokemon_result, POKEMON_SUCCESS);
	pokemon_result = pokemonUseMove(charizard, pidgey, "Tail-Whip");
	TEST_EQUALS(result, pokemon_result, POKEMON_NO_POWER_POINTS);
	for (int i = 0; i <= 50; i++) { // poor pidgey (:
		pokemon_result = pokemonUseMove(charizard, pidgey, "Tackle");
		TEST_EQUALS(result, pokemon_result, POKEMON_SUCCESS);
	}
	pokemon_result = pokemonUseMove(charizard, pidgey, "Tackle");
	TEST_EQUALS(result, pokemon_result, POKEMON_NO_HEALTH_POINTS);

	pokemonDestroy(charizard);
	pokemonDestroy(pidgey);

	return result;
}

static bool testPokemonHeal() {
	bool result = true;
	PokemonResult pokemon_result = 0;
	Pokemon charizard = pokemonCreate("Charizard", TYPE_FIRE, 20, 10);
	Pokemon blastoise = pokemonCreate("Blastoise", TYPE_WATER, 20, 10);
	pokemonTeachMove(blastoise, "Water-Gun", TYPE_WATER, 1000, 5000);

	pokemon_result = pokemonHeal(NULL);
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);
	pokemon_result = pokemonHeal(charizard);
	TEST_EQUALS(result, pokemon_result, POKEMON_SUCCESS);
	pokemonUseMove(blastoise, charizard, "Water-Gun");
	pokemon_result = pokemonHeal(charizard);
	TEST_EQUALS(result, pokemon_result, POKEMON_SUCCESS);
	pokemon_result = pokemonUseMove(blastoise, charizard, "Water-Gun");
	TEST_EQUALS(result, pokemon_result, POKEMON_SUCCESS);

	pokemonDestroy(charizard);

	return result;
}

static bool testPokemonEvolve() {
	bool result = true;
	PokemonResult pokemon_result;
	int level;

	Pokemon charmander = pokemonCreate("charmander", TYPE_FIRE, 1000, 10);
	pokemon_result = pokemonEvolve(NULL, "");
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);
	pokemon_result = pokemonEvolve(charmander, "");
	TEST_EQUALS(result, pokemon_result, POKEMON_INVALID_NAME);
	pokemon_result = pokemonEvolve(charmander, NULL);
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);
	for (int i = 0; i < 90; i++) {
		level = pokemonGetLevel(charmander);
		pokemon_result = pokemonEvolve(charmander, "Charmeleon");
		TEST_EQUALS(result, pokemon_result, POKEMON_SUCCESS);
		TEST_EQUALS(result, pokemonGetLevel(charmander), level + 1);
	}
	pokemon_result = pokemonEvolve(charmander, "Charmeleon");
	TEST_EQUALS(result, pokemon_result, POKEMON_CANNOT_EVOLVE);

	pokemonDestroy(charmander);
	return result;
}
static bool testPokemonPrintName() {
	bool result = true;
	PokemonResult pokemon_result;

	Pokemon charmander = pokemonCreate("charmander", TYPE_FIRE, 1000, 10);
	pokemon_result = pokemonPrintName(charmander, NULL);
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);
	pokemon_result = pokemonPrintName(NULL, NULL);
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);

	return result;
}

static bool testPokemonPrintVoice() {
	bool result = true;
	PokemonResult pokemon_result;

	Pokemon charmander = pokemonCreate("charmander", TYPE_FIRE, 1000, 10);
	pokemon_result = pokemonPrintVoice(charmander, NULL);
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);
	pokemon_result = pokemonPrintVoice(NULL, NULL);
	TEST_EQUALS(result, pokemon_result, POKEMON_NULL_ARG);

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
