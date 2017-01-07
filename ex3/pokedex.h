#ifndef POKEDEX_H_
#define POKEDEX_H_

#include "map.h"
#include "set.h"

/**
 * Pokemon Species Container
 *
 * Implements a map-like type that stores information about the known pokemon
 * species in the game.
 *
 * The info is stored in a pokedexEntry type, which stores the following info:
 *  - Pokemon species
 *  - Initial CP
 *  - A set of the pokemon types
 *
 * The following functions are available:
 *  pokedexCreate   - Creates a new empty pokedex
 *  pokedexDestroy  - Deletes a pokedex, freeing all resources
 *  pokedexAddPokemon   - Adds info about a pokemon to the pokedex
 *  pokedexGetPokemonInfo   - Fetches the pokedexEntry about a given species
 *  pokedexEntryGetSpecies  - Gets the species from an entry
 *  pokedexEntryGetCp       - Gets the intial cp value from an entry
 *  pokedexEntryGetTypes    - Gets a set of types from an entry
 */

// Types definitions
typedef Map Pokedex;
typedef struct PokedexEntry_t *PokedexEntry;

// Possible error codes
typedef enum {
  POKEDEX_SUCCESS,
  POKEDEX_INVALID_ARGUMENT,
  POKEDEX_OUT_OF_MEMORY,
  POKEDEX_POKEMON_ALREADY_EXISTS,
  POKEDEX_POKEMON_NOT_EXIST,
} PokedexErrorCode;

/**
 * pokedexCreate - creates a new empty pokedex
 *
 * @return
 *  NULL - allocation error
 *  A new empty pokedex in case of success
 */
Pokedex pokedexCreate();

/**
 * pokedexDestroy - Deallocates an existing pokedex and frees all entries
 *
 * @param pokedex - Existing pokedex. If pokedex is NULL nothing will be done.
 */
void pokedexDestroy(Pokedex pokedex);

/**
 * pokedexAddPokemon - Adds info about a pokemon species to the pokedex
 *
 * @param pokedex   - Existing pokedex.
 * @param species   - Pokemon species to add.
 * @param cp        - Initial CP of the added species.
 * @param types     - a set of types for the species. Types is expected to be
 *                    a set of uppercase strings.
 *
 * @return
 *  POKEDEX_INVALID_ARGUMENT - a NULL arg was passed
 *  POKEDEX_OUT_OF_MEMORY    - allocation error
 *  POKEDEX_SUCCESS          - Pokedex entry was added successfully
 */
PokedexErrorCode pokedexAddPokemon(Pokedex pokedex, char* species, int cp,
                            Set types);

/**
 * pokedexGetPokemonInfo - Returns an existing pokedex entry for a species
 *
 * @param pokedex - Existing pokedex.
 * @param species   - The pokemon species to fetch information on.
 *
 * Note that the returned entry isn't copied before being fetched, so there's
 * no need to free its memory yourself.
 *
 * @return
 *  NULL - a NULL arg was passed, or there was no entry found for the species
 *  A pokedexEntry type will be returned on success.
 */
PokedexEntry pokedexGetPokemonInfo(Pokedex pokedex, char* species);

/**
 * pokedexEntryGetSpecies - Returns the species from a pokedex entry
 *
 * @param entry - a pokedex entry, returned by pokedexGetPokemonInfo.
 *
 * @return
 *  NULL - if a NULL arg was passed
 *  The pokemon species will be returned on sucess.
 */
char* pokedexEntryGetSpecies(PokedexEntry entry);

/**
 * pokedexEntryGetCp - Returns the intial CP value from a pokedex entry
 *
 * @param entry - a pokedex entry, returned by pokedexGetPokemonInfo.
 *
 * @return
 *  -1 - if a NULL arg was passed
 *  The pokemon initial cp value will be returned on sucess.
 */
int pokedexEntryGetCp(PokedexEntry entry);

/**
 * pokedexEntryGetTypes - Returns the pokemon types from a pokedex entry
 *
 * @param entry - a pokedex entry, returned by pokedexGetPokemonInfo.
 *
 * @return
 *  NULL - if a NULL arg was passed
 *  A set of uppercase strings, representing the pokemon types will be
 *  returned on success.
 */
Set pokedexEntryGetTypes(PokedexEntry entry);

#endif
