#ifndef POKEDEX_H_
#define POKEDEX_H_

#include "map.h"
#include "set.h"

struct PokedexEntry_t {
	char* species;
	int cp;
	Set types;
};

typedef struct PokedexEntry_t *PokedexEntry;

typedef Map Pokedex;

Pokedex createPokedex();
void destroyPokedex(Pokedex pokedex);
MapResult pokedexAddPokemon(Pokedex pokedex, char* species, int cp,
                            Set types);
PokedexEntry pokedexGetPokemonInfo(Pokedex pokedex, char* species);

#endif
