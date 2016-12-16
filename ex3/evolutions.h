#ifndef EVOLUTIONS_H_
#define EVOLUTIONS_H_

#include "map.h"
#include "pokedex.h"

typedef struct EvolutionEntry_t *EvolutionEntry;

typedef Map Evolutions;

Evolutions createEvolutions();
void destroyEvolutions(Evolutions evolutions);
MapResult evolutionsAddEntry(Evolutions evolutions, char* pokemon_to_evolved,
                        unsigned int level, PokedexEntry evolved_pokemon);
// return NULL if no evolution exist or level is too low
PokedexEntry getEvolution(Evolutions evolutions, char* pokemon_to_evolved,
                          unsigned int level);

#endif
