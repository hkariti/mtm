#ifndef EVOLUTIONS_MOCKS_H
#define EVOLUTIONS_MOCKS_H

#include "../../evolutions.h"
#include "pokedex_mocks.h"

Evolutions demoEvolutions();

#define EVOLUTIONS_SET_UP() \
  Pokedex pokedex = demoPokedex();\
  Evolutions evolutions = demoEvolutions(pokedex);

#define EVOLUTIONS_TEAR_DOWN() \
  destroyEvolutions(evolutions);\
  destroyPokedex(pokedex);
#endif
