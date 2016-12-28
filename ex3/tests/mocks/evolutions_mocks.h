#include <evolutions.h>
#include "pokedex_mocks.h"

Evolutions demoEvolutions();

#define EVOLUTIONS_SET_UP() \
  Pokedex pokedex = demoPokedex();\
  Evolutions evolutions = demoEvolutions(pokedex);

#define EVOLUTIONS_TEAR_DOWN() \
  destroyEvolutions(evolutions);\
  destroyPokedex(pokedex);
