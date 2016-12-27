#ifndef POKEMON_H_
#define POKEMON_H_

#include "set.h"
#include "list.h"
#include "trainer.h"
#include "print_utils.h"
#include "pokedex.h"
#include "evolutions.h"

#define DOUBLE_STAR_TYPE_POKECOINS	30
#define ONE_STAR_TYPE_POKECOINS		20
#define REGULAR_TYPE_POKECOINS		10

#define MAX_POKEMON_HP				100
#define UNASSIGNED_POKEMON_ID		-1
#define START_POKEMON_LEVEL			1

typedef enum {
	POKEMON_SUCCESS,
	POKEMON_INVALID_ARGUMENT,
	POKEMON_OUT_OF_MEMORY,
	POKEMON_HP_IS_AT_MAX,
	POKEMON_POKEMON_NOT_EXIST,
} PokemonErrorCode;

typedef struct Pokemon_t * Pokemon;

Pokemon createPokemon(PokedexEntry pokemon_info, Evolutions evolutions_map);
void destroyPokemon(Pokemon pokemon);
void printPokemon(Pokemon pokemon, FILE* output_channel);
Pokemon copyPokemon(Pokemon pokemon);

int pokemonCompareByID(Pokemon pokemon_1, Pokemon pokemon_2);

// return -1 if null arg
int pokemonGetCP(Pokemon pokemon);
double pokemonGetHP(Pokemon pokemon);
int pokemonGetLevel(Pokemon pokemon);
// return null if null arg
char* pokemonGetSpecies(Pokemon pokemon);

// return POKEMON_INVALID_ARGUMENT if pokemon is null or candy_value<=0
PokemonErrorCode pokemonGiveCandy(Pokemon pokemon, int candy_value);
// return POKEMON_INVALID_ARGUMENT if pokemon is null or potion_value<=0
PokemonErrorCode pokemonGivePotion(Pokemon pokemon, int potion_value);

// output is written to isDead bool pointer
// errors: POKEMON_INVALID_ARGUMENT if pokemon or isDead bool are NULL
PokemonErrorCode isPokemonDead(Pokemon pokemon, bool* is_dead);

// return -1 if null arg
int pokemonCaught(Pokemon pokemon, int new_pokemon_id); //returns how many pokecoins earned

// return battleDelta calculated, return -1 if arg is null
double pokemonBattle(Pokemon pokemon, Pokemon opponent_pokemon, double opponent_trainer_xp);

#endif
