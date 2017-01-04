#ifndef LOCATION_H_
#define LOCATION_H_

#include "list.h"
#include "set.h"
#include "pokemon.h"
#include "print_utils.h"

/** Type used for returning error codes from location functions */
typedef enum {
	LOCATION_SUCCESS,
	LOCATION_INVALID_ARGUMENT,
	LOCATION_OUT_OF_MEMORY,
} LocationErrorCode;

/** Type for defining the Location */
typedef struct Location_t *Location;


/**
* locationCreate: Allocates a new open natural location somewhere worldwide,
*				  The locations will have no neighbours and no pokemons in it.
* @param name - The new created location's name
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new Location in case of success.
*/
Location createLocation(char* name);


/**
* pokemonDestroy: Deallocates an existing location. Clears all elements 
*				  by using the stored free & destroy functions.
* @param location - Target location to be deallocated. 
*					If location is NULL nothing will be done
*/
void destroyLocation(Location location);

/**
* locationPrint: prints the current location information to the output channel.
*				 print is done using mtmPrintLocation function.
*
* @param location - Target location to be printed. 
					If location is NULL nothing will be done
* @param output_channel - pointer to a FILE* output stream.
*/
void printLocation(Location location, FILE* output_channel);


/**
* locationCopy: Creates a new copy instance of target location.
*
* @param location - Target location.
* @return
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A newly allocated Location containing the same fields as
*	location otherwise.
*/
Location copyLocation(Location location);


/**
* int locationCompare: Compares 2 locations by their names
*
* @param location_1 - first location to compare.
* @param location_2 - second location to compare.
* @return
* 	A positive integer if the first location's name is greater (using strcmp)
* 	0 if they're equal.
*	A negative integer if the second location's name is greater.
*/
int locationCompare(Location location_1, Location location_2);


/**
* locationGetName: Returns the name of location
* @param location - The target location.
* @return
* 	NULL if a NULL pointer was sent.
* 	Otherwise the name of the location.
*	Notice the pointer returned is to the same as in Location struct
*	and not a copy of it.
*/
char* locationGetName(Location location);

/**
* locationAppendPokemon: Adds a new wild pokemon to the list,
*			    		 the new pokemon will be the last pokemon
*
* @param location -  The location for which to append the pokemon
* @param pokemon - The pokemon to append. A copy of the pokemon will be added.
* @return
* LOCATION_INVALID_ARGUMENT if a NULL was sent as location
* LOCATION_OUT_OF_MEMORY if an allocation failed 
* LOCATION_SUCCESS the pokemon appended successfully
*/
LocationErrorCode locationAppendPokemon(Location location, Pokemon pokemon);

/**
* Returns the first pokemon in the location's pokemon's list,
*				and removes it from the list.
*
* @param location - The location for which the first pokemon will be popped
* @return
* NULL if location is NULL, the location doesn't contain any pokemons,
*	or if allocation failed.
* otherwise - The pokemon popped from the location
*/
Pokemon locationPopPokemon(Location location);

/**
* locationIsEmpty: Checks if the location doesn't contain any wild  pokemons
* @param location - The target location
* @return
* 	true if the location doesn't contain any pokemons.
* 	false if the location contains wild pokemons.
*/
bool locationIsEmpty(Location location);

/**
* locationIsNeighour: Checks if the the 2 locations are neighbours
* @param location - The target location
* @param neighbour - another location to check if is a neighbour to location
* @return
* 	true if the two locations are neighbours.
* 	false if the two locations are not neighbours
*		or one of the arguments is NULL.
*/
bool locationIsNeighour(Location location, Location neighbour);

/**
* locationAddNeighbor: Adds a new neighbour to the location
*
* @param location -  The location for which to add the neighbour
* @param neighbour_name - The neighbour name to add to the location's
				neighbour's set. A copy of the location's name will be added.
* @return
* LOCATION_INVALID_ARGUMENT if a NULL was sent as location
* LOCATION_OUT_OF_MEMORY if the allocation failed
* LOCATION_SUCCESS the neighbour added successfully
*/
LocationErrorCode locationAddNeighbor(Location location, char* neighbour_name);

#endif
