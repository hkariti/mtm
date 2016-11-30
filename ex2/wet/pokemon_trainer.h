#ifndef POKEMON_TRAINER_H_
#define POKEMON_TRAINER_H_

#include <stdio.h>
#include <stdbool.h>
#include "pokemon.h"

typedef enum {
	POKEMON_TRAINER_SUCCESS,
	POKEMON_TRAINER_NULL_ARG,
	POKEMON_TRAINER_OUT_OF_MEM,
	POKEMON_TRAINER_INVALID_INDEX,
	POKEMON_TRAINER_PARTY_FULL,
	POKEMON_TRAINER_REMOVE_LAST,
	POKEMON_TRAINER_DEPOSIT_LAST,
	POKEMON_TRAINER_DEPOSIT_FULL
} PokemonTrainerResult;

/*
 * The pokemon list structure
*/
typedef struct pokemon_list_t {
    Pokemon* list;
    int length;
    int max_length;
    int min_length;
} *PokemonList;

/*
 * The pokemon trainer structure
*/
typedef struct pokemon_trainer_t {
    char* name;
    PokemonList local_pokemon;
    PokemonList remote_pokemon;
} *PokemonTrainer;

/**
* Creates a new trainer.
*
* The name parameter sets the name property.
* initial_pokemon will be copied and put as the first element in the
*   local_pokemon list.
* max_num_local and max_num_remote will be used as the max_length for the
*   local_pokemon and remote_pokemon lists, respectfully.
*
* @return
* 	A new allocated trainer.
* 	If name is NULL or empty; initial_pokemon is NULL; max_num_local or
* 	  max_num_remote is not positive; or in case of a memory allocation failure
* 	  - return NULL.
**/
PokemonTrainer pokemonTrainerCreate(char* name, Pokemon initial_pokemon,
	int max_num_local, int max_num_remote);
/**
* Destroy a trainer.
*
* All of the given trainer's memory will be freed, including all of the local
* and remote pokemon objects.
*
* If trainer is NULL, no action is taken.
**/
void pokemonTrainerDestroy(PokemonTrainer trainer);
PokemonTrainer pokemonTrainerCopy(PokemonTrainer trainer);
PokemonTrainerResult pokemonTrainerAddPokemon(PokemonTrainer trainer,
	Pokemon pokemon);
Pokemon pokemonTrainerGetPokemon(PokemonTrainer trainer, int pokemon_index);
PokemonTrainerResult pokemonTrainerRemovePokemon(
	PokemonTrainer trainer, int pokemon_index);
PokemonTrainerResult pokemonTrainerDepositPokemon(
	PokemonTrainer trainer, int pokemon_index);
PokemonTrainerResult pokemonTrainerWithdrawPokemon(
	PokemonTrainer trainer, int pokemon_index);
int pokemonTrainerGetNumberOfPokemons(PokemonTrainer trainer);
Pokemon pokemonTrainerGetMostRankedPokemon(PokemonTrainer trainer);
PokemonTrainerResult pokemonTrainerMakeMostRankedParty(PokemonTrainer trainer);
PokemonTrainerResult pokemonTrainerPrintEnumeration(
	PokemonTrainer trainer, FILE* file);
#endif // POKEMON_TRAINER_H_
