#ifndef LOCATION_H_
#define LOCATION_H_

#include "list.h"
#include "map.h"
#include "pokemon.h"
#include "print_utils.h"

typedef enum {
	LOCATION_SUCCESS,
	LOCATION_INVALID_ARGUMENT,
	LOCATION_OUT_OF_MEMORY,
} LocationErrorCode;

typedef struct Location_t *Location;

Location createLocation(char* name);
void destroyLocation(Location location);
void printLocation(Location location, FILE* output_channel);
Location copyLocation(Location location);

// return null if null arg
char* locationGetName(Location location);

LocationErrorCode locationPushPokemon(Location location, Pokemon pokemon);

// return null if null arg or if empty list
Pokemon locationPopPokemon(Location location);

// return null if null arg or neighbour doesn't exist
Location locationGetNeighour(Location location, char* neighour_name);
LocationErrorCode locationAddNeighbor(Location location, Location neighbor);

#endif
