#ifndef EVOLUTIONS_H_
#define EVOLUTIONS_H_

#include "map.h"
#include "pokedex.h"

/**
 * Pokemon Evolutions Map
 *
 * Implements a map-like type that allows to fetch a pokemon's evolved species
 * object (see the Pokedex type in pokedex.h) given its species.
 *
 * The following functions are available:
 *  evolutionsCreate    Creates a new empty evolutions map
 *  evolutionsDestroy   Deletes an existing map. Pokedex entries are *NOT*
 *                      freed.
 *  evolutionsAddEntry  Adds an entry to the map, if an entry alredy exists
 *                      it's overwritten.
 *  getEvolution        Fetches the Pokedex entry for the evoloved form of
 *                      the given species, if the given level is high enough.
 */

// Type definition
typedef Map Evolutions;

// Possible error codes for this type
typedef enum {
  EVOLUTIONS_SUCCESS,
  EVOLUTIONS_INVALID_ARGUMENT,
  EVOLUTIONS_OUT_OF_MEMORY,
  EVOLUTIONS_ALREADY_EXISTS
} EvolutionsErrorCode;

/**
 * evolutionsCreate: create a new empty evolutions map.
 *
 * @return
 *  NULL - allocation failed.
 *  A new Evolutions type in case of success.
 */
Evolutions evolutionsCreate();

/**
 * evolutionsDestroy: Deallocates an existing map. The corresponding Pokedex
 *  entries are assumed to be shared with the Pokedex type, and so will
 *  NOT be destroyed.
 *
 *  @param evolutions   Target map to be deallocated. If evolutions is NULL
 *                      nothing will be done
 */
void evolutionsDestroy(Evolutions evolutions);

/**
 * evolutionsAddEntry: Add an evolution entry to the map
 *
 * @param evolutions        The evolutions type to add the entry to.
 * @param pokemon_to_evolve The species name to store evolution info on.
 * @param level             The level at which pokemon_to_evolve evolves.
 * @param evolved_pokemon   A Pokedex entry describing the evolved form.
 *
 * evolved_pokemon is assumed to be shared with the Pokedex type, so it will
 * NOT be copied. pokemon_to_evolve will be copied before being used.
 *
 * If an entry for pokemon_to_evolve already exists, it's overwritten.
 *
 * @return
 *  EVOLUTIONS_INVALID_ARGUMENT if a NULL arg was passed as of of the arguments.
 *  EVOLUTIONS_OUT_OF_MEM if a allocation error occured.
 *  EVOLUTIONS_SUCCESS the entry was inserted to the map successfully.
 */
EvolutionsErrorCode evolutionsAddEntry(Evolutions evolutions,
                                       char* pokemon_to_evolve, int level,
                                       PokedexEntry evolved_pokemon);

// return NULL if no evolution exist or level is too low
/**
 * getEvolutions: Fetch the Pokedex entry of the evolved form, if the given
 *      pokemon is ready to evolve.
 *
 *  @param evolutions           The evolutions type to add the entry to.
 *  @param pokemon_to_evolve    The species name of the pokemon we're trying
 *                              to evolve.
 *  @param level                The current level of pokemon_to_evolve.
 *
 *  @return
 *      NULL    a NULL argument was sent, or the given level is below the
 *              evolution level of pokemon_to_evolve
 *      The Pokdex entry for the next form will be return if pokemon_to_evolve
 *      is ready to evolve (its level is high enough).
 */
PokedexEntry evolutionsGet(Evolutions evolutions, char* pokemon_to_evolve,
                          int level);

#endif
