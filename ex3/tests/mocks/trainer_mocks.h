#ifndef TRAINER_MOCKS_H
#define TRAINER_MOCKS_H

#include "location_mocks.h"
#include "pokedex_mocks.h"
#include "evolutions_mocks.h"
#include "store_mocks.h"
#include "pokemon_mocks.h"
#include <trainer.h>
#include <location.h>
#include <utils.h>

Map demoLocations(Pokedex pokedex, Evolutions evolutions);
Trainer demoTrainerEmpty(char* name, Map locations);
Trainer demoTrainerFull(char* name, Map locations);
Trainer demoTrainerWithItems(char* name, Map locations);
Trainer demoTrainerWithPokemon(char* name, Map locations);
Trainer demoTrainerWithMoney(char* name, int money, Map locations);

#define TRAINER_SET_UP()                              \
  Evolutions evolutions = demoEvolutions();           \
  Pokedex pokedex = demoPokedex();                    \
  Map locations = demoLocations(pokedex, evolutions);

#define TRAINER_TEAR_DOWN()                     \
  destroyEvolutions(evolutions);                \
  destroyPokedex(pokedex);                      \
  mapDestroy(locations);
#endif
