#include "trainer_mocks.h"

Map demoLocations(Pokedex pokedex, Evolutions evolutions) {
  Map locations = mapCreate((copyMapKeyElements)stringCopy,
                            (copyMapDataElements)copyLocation,
                            (freeMapKeyElements)free,
                            (freeMapDataElements)destroyLocation,
                            (compareMapKeyElements)strcmp);
  Location empty_location, neighbor, full_location;
  empty_location = demoLocationWithPokemon("empty_location", 1, pokedex, evolutions);
  neighbor = demoLocationWithPokemon("neighbor", 0, pokedex, evolutions);
  full_location = demoLocationWithPokemon("full_location", 10, pokedex, evolutions);
  locationAddNeighbor(empty_location, "neighbor");

  mapPut(locations, "empty_location", empty_location);
  mapPut(locations, "neighbor", neighbor);
  mapPut(locations, "full_location", full_location);

  destroyLocation(empty_location);
  destroyLocation(neighbor);
  destroyLocation(full_location);
  return locations;
}

Trainer demoTrainerEmpty(char* name, Map locations) {
  Location location = mapGet(locations, "empty_location");
  Trainer trainer = createTrainer(name, 0, location);

  return trainer;
}

Trainer demoTrainerFull(char* name, Map locations) {
  FILE* devnull = fopen("/dev/null", "r");
  Location location = mapGet(locations, "full_location");
  Store store = demoStore();
  Trainer trainer = createTrainer(name, 1000, location);
  trainerHunt(trainer, devnull);
  trainerHunt(trainer, devnull);
  trainerHunt(trainer, devnull);
  trainerHunt(trainer, devnull);
  trainerBuyItem(trainer, store, "candy", 1);
  trainerBuyItem(trainer, store, "candy", 1);
  trainerBuyItem(trainer, store, "potion", 1);
  trainerBuyItem(trainer, store, "potion", 1);

  storeDestroy(store);
  fclose(devnull);
  return trainer;
}

Trainer demoTrainerWithItems(char* name, Map locations) {
  Location location = mapGet(locations, "empty_location");
  Store store = demoStore();
  Trainer trainer = createTrainer(name, 1000, location);

  trainerBuyItem(trainer, store, "candy", 1);
  trainerBuyItem(trainer, store, "candy", 1);
  trainerBuyItem(trainer, store, "potion", 1);
  trainerBuyItem(trainer, store, "potion", 1);

  storeDestroy(store);
  return trainer;
}

Trainer demoTrainerWithPokemon(char* name, Map locations) {
  FILE* devnull = fopen("/dev/null", "r");
  Location location = mapGet(locations, "full_location");
  Trainer trainer = createTrainer(name, 0, location);

  trainerHunt(trainer, devnull);
  trainerHunt(trainer, devnull);
  trainerHunt(trainer, devnull);
  trainerHunt(trainer, devnull);

  fclose(devnull);
  return trainer;
}

Trainer demoTrainerWithMoney(char* name, int money, Map locations) {
  Location location = mapGet(locations, "empty_location");
  Trainer trainer = createTrainer(name, money, location);

  return trainer;
}
