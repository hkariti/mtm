#ifndef POKEMON_TRAINER_H_
#define POKEMON_TRAINER_H_

#include <stdio.h>
#include <stdbool.h>
#include "pokemon.h"

#define POKEMON_TRAINER_MIN_LENGTH_LOCAL 1
#define POKEMON_TRAINER_MIN_LENGTH_REMOTE 0

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

typedef struct pokemon_list_t {
    Pokemon* list;
    int length;
    int max_length;
    int min_length;
} *PokemonList;

typedef struct pokemon_trainer_t {
    char* name;
    PokemonList local_pokemon;
    PokemonList remote_pokemon;
} *PokemonTrainer;

PokemonTrainer pokemonTrainerCreate(char* name, Pokemon initial_pokemon,
	int max_num_local, int max_num_remote);
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
