#include "location.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Location_t {
	char* name;
	Map neighbors;
	List pokemons;
};

Location copyLocationPointer(Location location) {
	assert(location);
	return location;
}

void destroyLocationPointer(Location location) {
	return;
}

Location createLocation(char * name)
{
	if (NULL == name) return NULL;
	Location location = malloc(sizeof(*location));
	if (NULL == location) return NULL;
	location->name = stringCopy(name);
	if (NULL == location->name) {
		free(location);
		return NULL;
	}
	location->neighbors = mapCreate(stringCopy, copyLocationPointer, free, destroyLocationPointer, strcmp);
	if (NULL == location->neighbors) {
		free(location->name);
		free(location);
		return NULL;
	}
	location->pokemons = listCreate(copyPokemon, destroyPokemon);
	if (NULL == location->pokemons) {
		free(location->name);
		mapDestroy(location->neighbors);
		free(location);
		return NULL;
	}
	return location;
}

void destroyLocation(Location location)
{
	if (NULL == location) return;
	mapDestroy(location->neighbors);
	listDestroy(location->pokemons);
	free(location->name);
	free(location);
}

Location copyLocation(Location location)
{
	if (NULL == location) return NULL;
	Location new_location = malloc(sizeof(*new_location));
	if (NULL == new_location) return NULL;
	new_location->name = stringCopy(location->name);
	if (NULL == new_location->name) {
		return NULL;
	}
	new_location->neighbors = mapCopy(location->neighbors);
	if (NULL == new_location->neighbors) {
		free(new_location->name);
		return NULL;
	}
	new_location->pokemons = listCopy(location->pokemons);
	if (NULL == new_location->pokemons) {
		free(new_location->name);
		mapDestroy(new_location->neighbors);
		return NULL;
	}
	return new_location;
}

int locationCompare(Location location_1, Location location_2) {
	assert(location_1);
	assert(location_2);

	return strcmp(location_1->name, location_2->name);
}

void printLocation(Location location, FILE * output_channel)
{
	assert(location);
	assert(output_channel);

	Pokemon first_pokemon = listGetFirst(location->pokemons);
	char* pokemon_species = NULL;;
	if (NULL != first_pokemon) {
		pokemon_species = pokemonGetSpecies(first_pokemon);
	}
	mtmPrintLocation(output_channel, location->name, pokemon_species);
}

char* locationGetName(Location location)
{
	if (NULL == location) return NULL;

	return location->name;
}

LocationErrorCode locationPushPokemon(Location location, Pokemon pokemon) {
	if (NULL == location) return LOCATION_INVALID_ARGUMENT;
	if (NULL == pokemon) return LOCATION_INVALID_ARGUMENT;

	ListResult result = listInsertFirst(location->pokemons, pokemon);

	if (result == LIST_NULL_ARGUMENT) return LOCATION_INVALID_ARGUMENT;
	if (result == LIST_OUT_OF_MEMORY) return LOCATION_OUT_OF_MEMORY;

	return LOCATION_SUCCESS;
}

Pokemon locationPopPokemon(Location location)
{
	assert(location);
	if (NULL == location) return NULL;

	Pokemon pokemon = listGetFirst(location->pokemons);
	if (NULL == pokemon) return NULL;

	pokemon = copyPokemon(pokemon);
	if (NULL == pokemon) return NULL;

	listRemoveCurrent(location->pokemons);
	return pokemon;
}

bool locationIsEmpty(Location location) {
	assert(location);

	return listGetSize(location->pokemons) == 0;
}


Location locationGetNeighour(Location location, char * neighour_name)
{
	assert(location);
	assert(neighour_name);

	if (NULL == location || NULL == neighour_name) return NULL;

	return mapGet(location->neighbors, neighour_name);
}

LocationErrorCode locationAddNeighbor(Location location, Location neighbor)
{
	assert(location);
	assert(neighbor);

	if (NULL == location || NULL == neighbor) return LOCATION_INVALID_ARGUMENT;

	MapResult result = mapPut(location->neighbors, neighbor->name, neighbor);
	if (result == MAP_NULL_ARGUMENT) return LOCATION_INVALID_ARGUMENT;
	if (result == MAP_OUT_OF_MEMORY) return LOCATION_OUT_OF_MEMORY;

	return LOCATION_SUCCESS;
}





