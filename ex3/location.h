#ifndef LOCATION_H_
#define LOCATION_H_

#include "list.h"
#include "map.h"
#include "pokemon.h"
#include "print_utils.h"

typedef struct Location_t *Location;

Location createLocation(char* name);
void destroyLocation(Location location);
void printLocation(Location location, FILE* output_file);
Location copyLocation(Location location);

char* locationGetName(Location location);

ListResult locationPushPokemon(Location location, Pokemon pokemon);
Pokemon locationPopPokemon(Location location);

Location locationGetNeighour(Location location, char* neighour_name);
MapResult locationAddNeighbor(Location location, Location neighbor);

#endif
