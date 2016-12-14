#ifndef POKEMON_H_
#define POKEMON_H_

#include "set.h"
#include "list.h"
#include "item.h"
#include "trainer.h"
#include "print_utils.h"
#include "pokedex.h"
#include "evolutions.h"

typedef struct Pokemon_t *Pokemon;

struct Pokemon_t { //TODO: move to .c file
	Map evolutions;
	PokedexEntry pokemon_info;
	double hp;
	int cp_extra;
	int level;
	int id;
};

Pokemon createPokemon(char* specie, int cp, Set types, List* evolutions_list); //evolutions may be set?
void destroyPokemon(Pokemon pokemon);
void printPokemon(Pokemon pokemon);

int pokemonCompareByID(Pokemon pokemon_1, Pokemon pokemon_2);

int getPokemonCP(Pokemon pokemon);
double getPokemonHP(Pokemon pokemon);
int getPokemonLevel(Pokemon pokemon);
char* getPokemonSpecie(Pokemon pokemon);
double getPokemonXPValue(Pokemon pokemon);

void pokemonGiveCandy(Pokemon pokemon, Item candy);
void pokemonGivePotion(Pokemon pokemon, Item potion);

bool isPokemonDead(Pokemon pokemon);

void BattlePokemon(Trainer trainer_opponent, Pokemon pokemon_opponent); //dont think we can include trainer, maybe just get xp as param

#endif