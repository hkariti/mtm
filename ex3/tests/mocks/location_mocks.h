#ifndef LOCATION_MOCKS_H
#define LOCATION_MOCKS_H

#include "../../location.h"
#include "../../pokedex.h"
#include "../../evolutions.h"
#include "pokemon_mocks.h"

Location demoLocationWithPokemon(char* name, int num_of_pokemon, Pokedex pokedex,
                                 Evolutions evolutions);

#define LOCATION_SET_UP()                              \
  Pokedex pokedex = demoPokedex();                     \
  Evolutions evolutions = demoEvolutions(pokedex);     \
  Location location = demoLocationWithPokemon("Hong Kong", 15, pokedex, evolutions);

#define LOCATION_TEAR_DOWN()                     \
  evolutionsDestroy(evolutions);                \
  pokedexDestroy(pokedex);                      \
  destroyLocation(location);


#endif
