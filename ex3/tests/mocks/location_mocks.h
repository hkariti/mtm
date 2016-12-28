#ifndef LOCATION_MOCKS_H
#define LOCATION_MOCKS_H

#include "../../location.h"
#include "../../pokedex.h"
#include "../../evolutions.h"
#include "pokemon_mocks.h"

Location demoLocationWithPokemon(char* name, int num_of_pokemon, Pokedex pokedex,
                                 Evolutions evolutions);
#endif
