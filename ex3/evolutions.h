#ifndef EVOLUTIONS_H_
#define EVOLUTIONS_H_

#include "map.h"
#include "pokedex.h"

typedef struct EvolutionEntry_t *EvolutionEntry;
struct EvolutionEntry_t {  //TODO: move to .c file
	PokedexEntry evolved_pokemon;
	int level;
};

typedef Map *Evolutions;

Evolutions createEvolutions();
void destroyEvolutions(Evolutions evolutions);
void evolutionsAddEntry(Evolutions evolutions, char* pokemon_to_evolved, int level, PokedexEntry evolved_pokemon);
PokedexEntry getEvolution(Evolutions evolutions, char* pokemon_to_evolved, int level); // return NULL if no evolution exist or level is too low

#endif