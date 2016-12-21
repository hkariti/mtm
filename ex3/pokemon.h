#ifndef POKEMON_H_
#define POKEMON_H_

#define DOUBLE_STAR_TYPE_POKECOINS	30
#define ONE_STAR_TYPE_POKECOINS		20
#define REGULAR_TYPE_POKECOINS		10

#define MAX_POKEMON_HP				100
#define UNASSIGNED_POKEMON_ID		-1
#define START_POKEMON_LEVEL			1

#include "set.h"
#include "list.h"
#include "trainer.h"
#include "print_utils.h"
#include "pokedex.h"
#include "evolutions.h"

typedef struct Pokemon_t *Pokemon;

Pokemon createPokemon(PokedexEntry pokemon_info, Evolutions evolutions_list); //evolutions may be set?
void destroyPokemon(Pokemon pokemon);
void printPokemon(Pokemon pokemon, FILE* output_file);
Pokemon copyPokemon(Pokemon pokemon);

int pokemonCompareByID(Pokemon pokemon_1, Pokemon pokemon_2);

int pokemonGetCP(Pokemon pokemon);
double pokemonGetHP(Pokemon pokemon);
int pokemonGetLevel(Pokemon pokemon);
char* pokemonGetSpecies(Pokemon pokemon);

void pokemonGiveCandy(Pokemon pokemon, int candy_value);
MtmErrorCode pokemonGivePotion(Pokemon pokemon, int potion_value);

bool isPokemonDead(Pokemon pokemon);

int pokemonCaught(Pokemon pokemon, int new_pokemon_id); //returns how many pokecoins earned

double calculateBattleDelta(Pokemon pokemon, Pokemon opponent_pokemon, double opponent_trainer_xp);
void pokemonBattle(Pokemon pokemon, Pokemon opponent_pokemon, double opponent_trainer_xp);

#endif