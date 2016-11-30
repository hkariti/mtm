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
*   local_pokemon (local party) and remote_pokemon (remote/professor party)
*   lists, respectfully.
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
* All of the given trainer's memory will be freed, including all of the local
* and remote pokemon objects.
*
* @return
*   A new trainer object. If given trainer is NULL, return NULL.
**/
void pokemonTrainerDestroy(PokemonTrainer trainer);

/**
 * Create a new trainer and copy its properties and pokemon from the given
 * trainer object. Both local and remote pokemon will be copied.
 */
PokemonTrainer pokemonTrainerCopy(PokemonTrainer trainer);

/**
 * Copy the given pokemon and add it to the local pokemon of the given trainer.
 *
 * The index of the new pokemon will be the highest available (e.g. if the
 * trainer has 2 pokemon already, the index of the new one will be 3).
 *
 * @return
 *  POKEMON_TRAINER_SUCCESS for a successful run
 *  POKEMON_TRAINER_NULL_ARG if one of trainer of pokemon is NULL
 *  POKEMON_TRAINER_PARTY_FULL if the trainer's local pokemon list is full
 *  POKEMON_TRAINER_OUT_OF_MEM if there was a memory allocation error
 */
PokemonTrainerResult pokemonTrainerAddPokemon(PokemonTrainer trainer,
	Pokemon pokemon);

/**
 * Return a pokemon from the trainer's local party, given its index.
 *
 * @return
 *   A pokemon object.
 *   If trainer is NULL or if the index is invalid (non-positive or
 *     more than the number or pokemon in the list), return NULL.
 */
Pokemon pokemonTrainerGetPokemon(PokemonTrainer trainer, int pokemon_index);

/**
 * Remove a pokemon from the trainer's local party. The pokemon object is
 * destroyed. All of the pokemon whose index was after the given pokemon_index
 * will have their index decreased by one.
 *
 * @return
 *   POKEMON_TRAINER_SUCCESS - successful run
 *   POKEMON_TRAINER_NULL_ARG - trainer is NULL
 *   POKEMON_TRAINER_BAD_INDEX - pokemon_index is non-positive or more than
 *      the number of pokemon in the local party
 *   POKEMON_TRAINER_REMOVE_LAST - trying to remove the last pokemon in the
 *      local party
 */
PokemonTrainerResult pokemonTrainerRemovePokemon(
	PokemonTrainer trainer, int pokemon_index);

/**
 * Move the given pokemon from the local party to the professor (remote party).
 * Any pokemon with indexes larger than the selected pokemon in the local party
 * will have their indexes decreased by one. The selected pokemon will have the
 * next available (highest) index in the remote party.
 *
 * The function will refuse to empty the local party or deposit to a full
 * remote party.
 *
 * @returns:
 *  POKEMON_TRAINER_SUCCESS - normal run
 *  POKEMON_TRAINER_NULL_ARG - trainer is null
 *  POKEMON_TRAINER_BAD_INDEX - pokemon_index is non-positve or more than
 *      the number of pokemon in the local party
 *  POKEMON_TRAINER_DEPOSIT_LAST - trying to deposit the last pokemon in the
 *      local party
 *  POKEMON_TRAINER_DEPOSIT_FULL - trying to deposit when the remote party
 *      is full
 */
PokemonTrainerResult pokemonTrainerDepositPokemon(
	PokemonTrainer trainer, int pokemon_index);

/**
 * Move a pokemon from the professor (remote party) to the local party. Any
 * pokemon with indexes higher than the requested index in the remote party
 * will have their indexes decreased by one. The selected pokemon will be given
 * the next available (highest) index in the local party.
 *
 * The function will refuse to withdraw the pokemon if the local party is full.
 *
 * @returns:
 *   POKEMON_TRAINER_SUCCESS - normal run
 *   POKEMON_TRAINER_NULL_ARG - trainer is NULL
 *   POKEMON_TRAINER_BAD_INDEX - pokemon_index is non-positive or higher than
 *     the number of pokemon in the remote party.
 *   POKEMON_TRAINER_PARTY_FULL - trying to withdraw a pokemon when the local
 *     party is full.
 */
PokemonTrainerResult pokemonTrainerWithdrawPokemon(
	PokemonTrainer trainer, int pokemon_index);

/**
 * Return the total number of pokemons, both in the local and remote party.
 *
 * The function assumes trainer is valid. Careful. 

 * @returns:
 *   number of local pokemon + number of remote pokemon
 */
int pokemonTrainerGetNumberOfPokemons(PokemonTrainer trainer);

/**
 * Return the pokemon with the highest rank, from both the local and remote
 * party. The function will prefer local to remote pokemon, and low index to
 * high index.
 *
 * @returns:
 *  a Pokemon object from the local/remote party. If the trainer is NULL
 *    returns NULL.
 */
Pokemon pokemonTrainerGetMostRankedPokemon(PokemonTrainer trainer);

/**
 * Sort the trainer's pokemon according to rank, cross both local and remote
 * parties. Local pokemon are preferred to remote pokemon, and pokemon with
 * lower indexes are preferred to pokemon with higher indexes.
 *
 * @returns:
 *  POKEMON_TRAINER_SUCCESS - successful run
 *  POKEMON_TRAINER_NULL_ARG - trainer is NULL
 *  POKEMON_TRAINER_OUT_OF_MEM - memor allocation error
 */
PokemonTrainerResult pokemonTrainerMakeMostRankedParty(PokemonTrainer trainer);

/**
 * For each pokemon from the local party, print its name and voice along with
 * the trainer's summon. The print will be done to the given file.
 *
 * Example output:
 *   Menashe: Pikachu
 *   Pikachu: Pika-Pika
 *
 * @returns:
 *  POKEMON_TRAINER_SUCCESS - successful run
 *  POKEMON_TRAINER_NULL_ARG - trainer or file are NULL
 */ 
PokemonTrainerResult pokemonTrainerPrintEnumeration(
	PokemonTrainer trainer, FILE* file);
#endif // POKEMON_TRAINER_H_
