#ifndef POKEDEX_H_
#define POKEDEX_H_

#include "map.h"
#include "set.h"

typedef enum {
	POKEDEX_SUCCESS,
	POKEDEX_INVALID_ARGUMENT,
	POKEDEX_OUT_OF_MEMORY,
	POKEDEX_POKEMON_ALREADY_EXISTS,
	POKEDEX_POKEMON_NOT_EXIST,
} PokedexErrorCode;

typedef struct PokedexEntry_t *PokedexEntry;

typedef Map Pokedex;

Pokedex createPokedex();
void destroyPokedex(Pokedex pokedex);
// errors: POKEDEX_INVALID_ARGUMENT, OUT_OF_MEMORY
PokedexErrorCode pokedexAddPokemon(Pokedex pokedex, char* species, int cp,
                            Set types);
// return NULL if null arg
PokedexEntry pokedexGetPokemonInfo(Pokedex pokedex, char* species);

// pokedexEntry funcs return -1 if null_arg
char* pokedexEntryGetSpecies(PokedexEntry entry);
int pokedexEntryGetCp(PokedexEntry entry);
Set pokedexEntryGetTypes(PokedexEntry entry);

#endif
