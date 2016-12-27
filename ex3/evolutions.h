#ifndef EVOLUTIONS_H_
#define EVOLUTIONS_H_

#include "map.h"
#include "pokedex.h"

typedef enum {
	EVOLUTIONS_SUCCESS,
	EVOLUTIONS_INVALID_ARGUMENT,
	EVOLUTIONS_OUT_OF_MEMORY,
	EVOLUTIONS_ALREADY_EXISTS
} EvolutionsErrorCode;

typedef struct EvolutionEntry_t *EvolutionEntry;

typedef Map Evolutions;

// Returns NULL if alloc error
Evolutions evolutionsCreate();

void evolutionsDestroy(Evolutions evolutions);

// errors: EVOLUTIONS_INVALID_ARGUMENT if null arg, OUT_OF_MEM
EvolutionsErrorCode evolutionsAddEntry(Evolutions evolutions, char* pokemon_to_evolve,
                        int level, PokedexEntry evolved_pokemon);

// return NULL if no evolution exist or level is too low
PokedexEntry getEvolution(Evolutions evolutions, char* pokemon_to_evolve,
                          int level);

#endif
