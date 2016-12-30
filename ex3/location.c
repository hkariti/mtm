#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "location.h"
#include "utils.h"

struct Location_t {
	char* name;
	Set neighbors;
	List pokemons;
};

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
  location->neighbors = setCreate((copySetElements)stringCopy,
                                  (freeSetElements)free,
                                  (compareSetElements)strcmp);
  if (NULL == location->neighbors) {
		free(location->name);
		free(location);
		return NULL;
	}
	location->pokemons = listCreate((CopyListElement)copyPokemon,
                                  (FreeListElement)destroyPokemon);
  if (NULL == location->pokemons) {
		free(location->name);
		setDestroy(location->neighbors);
		free(location);
		return NULL;
	}
	return location;
}

void destroyLocation(Location location)
{
	if (NULL == location) return;
	setDestroy(location->neighbors);
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
	new_location->neighbors = setCopy(location->neighbors);
	if (NULL == new_location->neighbors) {
		free(new_location->name);
		return NULL;
	}
	new_location->pokemons = listCopy(location->pokemons);
	if (NULL == new_location->pokemons) {
		free(new_location->name);
		setDestroy(new_location->neighbors);
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

	char* pokemon_species = NULL;;
    if (listGetSize(location->pokemons) > 0) {
        Pokemon first_pokemon = listGetFirst(location->pokemons);
		pokemon_species = pokemonGetSpecies(first_pokemon);
        assert(pokemon_species);
    }
	mtmPrintLocation(output_channel, location->name, pokemon_species);
}

char* locationGetName(Location location)
{
	if (NULL == location) return NULL;

	return location->name;
}

LocationErrorCode locationAppendPokemon(Location location, Pokemon pokemon) {
	if (NULL == location) return LOCATION_INVALID_ARGUMENT;
	if (NULL == pokemon) return LOCATION_INVALID_ARGUMENT;

	ListResult result = listInsertLast(location->pokemons, pokemon);

	if (result == LIST_NULL_ARGUMENT) return LOCATION_INVALID_ARGUMENT;
	if (result == LIST_OUT_OF_MEMORY) return LOCATION_OUT_OF_MEMORY;

	return LOCATION_SUCCESS;
}

Pokemon locationPopPokemon(Location location)
{
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


bool locationIsNeighour(Location location, Location neighbour)
{
	assert(location);
	assert(neighbour);

	if (NULL == location || NULL == neighbour) return NULL;

	return setIsIn(location->neighbors, neighbour->name);
}

LocationErrorCode locationAddNeighbor(Location location, char* neighbour_name)
{
	if (NULL == location || NULL == neighbour_name) return LOCATION_INVALID_ARGUMENT;

	SetResult result = setAdd(location->neighbors, neighbour_name);
	if (result == SET_NULL_ARGUMENT) return LOCATION_INVALID_ARGUMENT;
	if (result == SET_OUT_OF_MEMORY) return LOCATION_OUT_OF_MEMORY;

	return LOCATION_SUCCESS;
}





