#include <stdbool.h>
#include "aux_macros.h"
#include "pokemon_trainer.h"

static bool testCombo() {
	bool result = true;
	Pokemon pikachu = pokemonCreate("Pikachu", TYPE_ELECTRIC, 20, 4);
	PokemonTrainer trainer = pokemonTrainerCreate("Ash", pikachu, 6, 20);
	TEST_DIFFERENT(result, trainer, NULL);
	pokemonDestroy(pikachu);
	PokemonTrainer trainer_copy = pokemonTrainerCopy(trainer);
	TEST_DIFFERENT(result, trainer_copy, NULL);
	pokemonTrainerDestroy(trainer_copy);
	Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 10, 4);
	TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, squirtle),
		POKEMON_TRAINER_SUCCESS);
	pokemonDestroy(squirtle);
	TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 2);
	TEST_DIFFERENT(result, pokemonTrainerGetPokemon(trainer, 2), NULL);
	TEST_EQUALS(result, pokemonTrainerDepositPokemon(trainer, 2),
		POKEMON_TRAINER_SUCCESS);
	TEST_EQUALS(result, pokemonTrainerWithdrawPokemon(trainer, 1),
		POKEMON_TRAINER_SUCCESS);
	TEST_EQUALS(result, pokemonTrainerRemovePokemon(trainer, 2),
		POKEMON_TRAINER_SUCCESS);
	TEST_EQUALS(result, pokemonTrainerGetMostRankedPokemon(trainer),
		pokemonTrainerGetPokemon(trainer, 1));
	TEST_EQUALS(result, pokemonTrainerMakeMostRankedParty(trainer),
		POKEMON_TRAINER_SUCCESS);
	pokemonTrainerDestroy(trainer);
	return result;
}

int main() {
	RUN_TEST(testCombo);
	// TODO: add more tests:
	// RUN_TEST(...);
	return 0;
}
