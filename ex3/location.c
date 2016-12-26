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
	location->neighbors = mapCreate(stringCopy, copyLocation, free, destroyLocation, strcmp);
	if (NULL == location->neighbors) {
		free(location->name);
		free(location);
		return NULL;
	}
	location->pokemons = listCreate(copyPokemon, destroyPokemon);
	if (NULL == location->pokemons) {
		free(location->name);
		free(location->neighbors);
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


void printLocation(Location location, FILE * output_channel)
{
	assert(location);
	assert(output_channel);

	Pokemon first_pokemon = listGetFirst(location->pokemons);
	char* pokemon_species = NULL;;
	if (NULL != first_pokemon) {
		char* pokemon_species = pokemonGetSpecies(first_pokemon); //what happens if copy fails?
	}
	mtmPrintLocation(output_channel, location->name, pokemon_species);
	free(pokemon_species);
}

char* locationGetName(Location location) //what happened when malloc fails? no name?
{
	assert(location);

	return stringCopy(location->name);
}

ListResult locationPushPokemon(Location location, Pokemon pokemon) {
	assert(location);
	assert(pokemon);

	return listInsertFirst(location->pokemons, pokemon);
}

Pokemon locationPopPokemon(Location location)
{
	assert(location);

	Pokemon pokemon = listGetFirst(location->pokemons);
	if (pokemon != NULL) {
		listRemoveCurrent(location->pokemons);
	}
	return pokemon;
}

Location locationGetNeighour(Location location, char * neighour_name)
{
	assert(location);
	assert(neighour_name);

	return mapGet(location->neighbors, neighour_name);
}

MapResult locationAddNeighbor(Location location, Location neighbor)
{
	assert(location);
	assert(neighbor);

	return mapPut(location->neighbors, neighbor->name, neighbor);
}





