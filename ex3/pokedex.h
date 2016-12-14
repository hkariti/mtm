#ifndef POKEDEX_H_
#define POKEDEX_H_

#include "map.h"
#include "set.h"

typedef struct PokedexEntry_t *PokedexEntry;
struct PokedexEntry_t {  //TODO: move to .c file
	char* species;
	int cp;
	Set types;
};

typedef Map Pokedex;

Pokedex createPokedex();
void destroyPokedex(Pokedex pokedex);
void pokedexAddPokemon(Pokedex pokedex, char* species, int cp, Set types);
PokedexEntry pokedexGetPokemonInfo(Pokedex pokedex, char* species);

#endif