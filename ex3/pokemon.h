#ifndef POKEMON_H_
#define POKEMON_H_

#include "set.h"
#include "list.h"
#include "print_utils.h"
#include "pokedex.h"
#include "evolutions.h"

// definitions
#define DOUBLE_STAR_TYPE_POKECOINS	30
#define ONE_STAR_TYPE_POKECOINS		20
#define REGULAR_TYPE_POKECOINS		10

#define MAX_POKEMON_HP				100
#define UNASSIGNED_POKEMON_ID		-1
#define START_POKEMON_LEVEL			1

/** Type used for returning error codes from pokemon functions */
typedef enum {
	POKEMON_SUCCESS,
	POKEMON_INVALID_ARGUMENT,
	POKEMON_OUT_OF_MEMORY,
	POKEMON_HP_IS_AT_MAX,
	POKEMON_POKEMON_NOT_EXIST,
} PokemonErrorCode;

/** Type for defining the Pokemon */
typedef struct Pokemon_t *Pokemon;

/**
* pokemonCreate: Allocates a new wild pokemon lives in the wild 
*				 (doesn't belong to any trainer)
* @param pokemon_info - pointer to a PokedexEntry struct which contains the 
						requested pokemon information.
* @param evolutions_map - pointer to an Evolutions struct which contains all
						  the game's evolutions info/
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Pokemon in case of success.
*/
Pokemon createPokemon(PokedexEntry pokemon_info, Evolutions evolutions_map);

/**
* pokemonDestroy: Deallocates an existing pokemon. Clears all elements by using the
* stored free functions.
*
* @param pokemon - Target pokemon to be deallocated. If ppokemon is NULL nothing will be
* 		done
*/
void destroyPokemon(Pokemon pokemon);

/**
* pokemonPrint: prints the current pokemon information to the output channel.
*				print is done using mtmPrintPokemon function.
*
* @param pokemon - Target pokemon to be printed. If ppokemon is NULL nothing will be
* 		 done
*		 output_channel - pointer to a FILE* output stream.
*/
void printPokemon(Pokemon pokemon, FILE* output_channel);

/**
* pokemonCopy: Creates a copy of target pokemon.
*
* @param pokemon - Target pokemon.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A newly allocated Pokemon containing the same fields as
*	elements otherwise.
*/
Pokemon copyPokemon(Pokemon pokemon);

/**
* int pokemonCompareByID: Compares 2 pokemons by their id's
*
* @param pokemon_1 - first pokemon to compare.
* @param pokemon_2 - second pokemon to compare.
* @return
* 	A positive integer if the first pokemon's id is greater
* 	0 if they're equal.
*	A negative integer if the second pokemon's id is greater.
*/
int pokemonCompareByID(Pokemon pokemon_1, Pokemon pokemon_2);

/**
* pokemonGetCP: Returns the total cp of pokemon
* @param pokemon - The target pokemon.
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the total cp of the pokemon.
*/
int pokemonGetCP(Pokemon pokemon);

/**
* pokemonGetHP: Returns the current health points of pokemon
* @param pokemon - The target pokemon.
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the total hp of the pokemon.
*/
double pokemonGetHP(Pokemon pokemon);

/**
* pokemonGetLevel: Returns the current level points of pokemon
* @param pokemon - The target pokemon.
* @return
* 	-1 if a NULL pointer was sent.
* 	Otherwise the current level of the pokemon.
*/
int pokemonGetLevel(Pokemon pokemon);

/**
* pokemonGetSpecies: Returns the species of pokemon
* @param pokemon - The target pokemon.
* @return
* 	NULL if a NULL pointer was sent.
* 	Otherwise the species of the pokemon.
*	Notice the pointer returned is to the same as in pokedexEntry 
*	and not a copy of it.
*/
char* pokemonGetSpecies(Pokemon pokemon);

/**
* pokemonGiveCandy: Give a candy to the target pokemon, 
*					raising it's cp accordingly.
* @param pokemon - The target pokemon.
* @param candy_value - The value of the candy to feed the pokemon
* @return
* 	POKEMON_INVALID_ARGUMENT -	if pokemon provided is NULL 
								or candy value is not positive.
* 	POKEMON_SUCCESS - the candy was given successfully
*/
PokemonErrorCode pokemonGiveCandy(Pokemon pokemon, int candy_value);

/**
* pokemonGiveCandy: Give a potion to the target pokemon,
*					raising it's hp accordingly.
* @param pokemon - The target pokemon.
* @param potion_value - The value of the potion to feed the pokemon
* @return
* 	POKEMON_INVALID_ARGUMENT -	if pokemon provided is NULL
						or potion value is not positive.
* 	POKEMON_SUCCESS - the potion was given successfully
*/
PokemonErrorCode pokemonGivePotion(Pokemon pokemon, int potion_value);

/**
* isPokemonDead: check if the pokemon is dead (hp is 0)
* @param pokemon - The target pokemon.
* @param is_dead - Pointer to the a bool which the result will be written to
					if the pokemon is dead true will be written,
					if the pokemon is alive false will be written
* @return
* 	POKEMON_INVALID_ARGUMENT -	if pokemon provided is NULL or is_dead NULL
* 	POKEMON_SUCCESS - if args are not NULL
*/
PokemonErrorCode isPokemonDead(Pokemon pokemon, bool* is_dead);

/**
* pokemonCaught: catch the current pokemon and associate it with a trainer
* @param pokemon - The target pokemon.
* @param new_pokemon_id - the trainer's pokemon_id 
			to be associate to thhe pokemon
* @return
* 	-1 - if pokemon provided is NULL
* 	otherwise - returns the amount pokecoins the trainer earned from the catch
*/
int pokemonCaught(Pokemon pokemon, int new_pokemon_id);

/**
* pokemonBattle: perform battle between the 2 pokemons
				 changing their level & hp  accordingly
				 if necessary also evolve them
* @param pokemon1 - The first pokemon in battle.
* @param pokemon2 - The second pokemon in battle.
* @param battle_delta1 - the battle delta as calculated by pokemon2
						 hp, cp and trainer2 level
* @param battle_delta2 - the battle delta as calculated by pokemon1
						 hp, cp and trainer1 level
* @return
* 	POKEMON_INVALID_ARGUMENT - if one of the arguments is NULL
* 	POKEMON_SUCCESS - when battle perforemed successfully
*/
PokemonErrorCode pokemonBattle(Pokemon pokemon1, Pokemon pokemon2,
                               double battle_delta1, double battle_delta2);

#endif
