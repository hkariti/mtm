#include "trainer_mocks.h"

Map demoLocations(Pokedex pokedex, Evolutions evolutions) {
  Map locations = mapCreate((copyMapKeyElements)stringCopy,
                            (copyMapDataElements)copyLocation,
                            (freeMapKeyElements)free,
                            (freeMapDataElements)destroyLocation,
                            (compareMapKeyElements)stringCompare);
  Location empty_location, neighbor, full_location;
  empty_location = demoLocationWithPokemon("empty_location", 1, pokedex, evolutions);
  neighbor = demoLocationWithPokemon("neighbor", 0, pokedex, evolutions);
  full_location = demoLocationWithPokemon("full_location", 10, pokedex, evolutions);
  locationAddNeighbor(empty_location, "neighbor");
  Location paris = demoLocationWithPokemon("paris", 1, pokedex, evolutions);
  Location london = demoLocationWithPokemon("london", 2, pokedex, evolutions);
  Location madrid = demoLocationWithPokemon("madrid", 10, pokedex, evolutions);
  Location beijing = demoLocationWithPokemon("beijing", 10, pokedex, evolutions);
  Location telaviv = demoLocationWithPokemon("tel aviv", 1, pokedex, evolutions);
  Location jerusalem = demoLocationWithPokemon("jerusalem", 0, pokedex, evolutions);
  Location palem = demoLocationWithPokemon("palem", 10, pokedex, evolutions);
  locationAddNeighbor(paris, "london");
  locationAddNeighbor(paris, "madrid");
  locationAddNeighbor(london, "paris");
  locationAddNeighbor(london, "madrid");
  locationAddNeighbor(madrid, "paris");
  locationAddNeighbor(madrid, "london");
  locationAddNeighbor(jerusalem, "tel aviv");
  locationAddNeighbor(telaviv, "jerusalem");
  mapPut(locations, "empty_location", empty_location);
  mapPut(locations, "neighbor", neighbor);
  mapPut(locations, "full_location", full_location);
  mapPut(locations, "paris", paris);
  mapPut(locations, "london", london);
  mapPut(locations, "madrid", madrid);
  mapPut(locations, "jerusalem", jerusalem);
  mapPut(locations, "tel aviv", telaviv);
  mapPut(locations, "beijing", beijing);
  mapPut(locations, "palem", palem);

  destroyLocation(empty_location);
  destroyLocation(neighbor);
  destroyLocation(full_location);
  destroyLocation(paris);
  destroyLocation(london);
  destroyLocation(madrid);
  destroyLocation(telaviv); // O__O
  destroyLocation(jerusalem);
  destroyLocation(beijing);
  destroyLocation(palem);

  return locations;
}

Trainer demoTrainerEmpty(char* name, Map locations) {
  Location location = mapGet(locations, "empty_location");
  Trainer trainer = trainerCreate(name, 0, location);

  return trainer;
}

Trainer demoTrainerFullAtLocation(char* name, Map locations, char* location_name) {
  Location location = mapGet(locations, location_name);
  Store store = demoStore();
  Trainer trainer = trainerCreate(name, 1000, location);
  trainerHunt(trainer, stdout);
  trainerHunt(trainer, stdout);
  trainerHunt(trainer, stdout);
  trainerHunt(trainer, stdout);
  trainerBuyItem(trainer, store, "candy", 1);
  trainerBuyItem(trainer, store, "candy", 1);
  trainerBuyItem(trainer, store, "potion", 1);
  trainerBuyItem(trainer, store, "potion", 1);

  storeDestroy(store);
  return trainer;
}

Trainer demoTrainerFull(char* name, Map locations) {
	return demoTrainerFullAtLocation(name, locations, "full_location");
}

Trainer demoTrainerWithItems(char* name, Map locations) {
  Location location = mapGet(locations, "empty_location");
  Store store = demoStore();
  Trainer trainer = trainerCreate(name, 1000, location);

  trainerBuyItem(trainer, store, "candy", 1);
  trainerBuyItem(trainer, store, "candy", 1);
  trainerBuyItem(trainer, store, "potion", 1);
  trainerBuyItem(trainer, store, "potion", 1);

  storeDestroy(store);
  return trainer;
}

Trainer demoTrainerWithPokemon(char* name, Map locations) {
  Location location = mapGet(locations, "full_location");
  Trainer trainer = trainerCreate(name, 0, location);

  trainerHunt(trainer, stdout);
  trainerHunt(trainer, stdout);
  trainerHunt(trainer, stdout);
  trainerHunt(trainer, stdout);

  return trainer;
}

Trainer demoTrainerWithMoney(char* name, int money, Map locations) {
  Location location = mapGet(locations, "empty_location");
  Trainer trainer = trainerCreate(name, money, location);

  return trainer;
}
