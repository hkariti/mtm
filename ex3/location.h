#ifndef LOCATION_H_
#define LOCATION_H_

#include "list.h"
#include "map.h"
#include "pokemon.h"
#include "print_utils.h"

typedef struct Location_t *Location;

struct Location_t { //TODO: move to .c file
	char* name;
	Map neighbors;
	List pokemons;
};

Location createLocation(char* name);
void destroyLocation(Location location);
void printLocation(Location location);

char* locationGetName(Location location);

void locationPushPokemon(Location location, Pokemon pokemon);
Pokemon locationPopPokemon(Location location);

Location locationGetNeighour(Location location, char* neighour_name);
void locationAddNeighbor(Location location, Location neighbor);

#endif
