#ifndef POKEDEX_H_
#define POKEDEX_H_

#include "map.h"
#include "set.h"

struct PokedexEntry_t {
	char* species;
	unsigned int cp;
	Set types; //TODO: Abstract this in a TypesSet?
};

typedef struct PokedexEntry_t *PokedexEntry;

typedef Map Pokedex;

Pokedex createPokedex();
void destroyPokedex(Pokedex pokedex);
MapResult pokedexAddPokemon(Pokedex pokedex, char* species, unsigned int cp,
                            Set types);
PokedexEntry pokedexGetPokemonInfo(Pokedex pokedex, char* species);

#endif
