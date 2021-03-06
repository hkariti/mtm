#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "parser.h"
#include "pokemon.h"
#include "utils.h"

// Macro for iterating over all the words in the line, split by any whitespace
#define WORD_FOREACH(str, iterator)                          \
  for (char* iterator = strtok(str, " \t\r\n"); iterator != NULL;\
       iterator = strtok(NULL, " \t\r\n"))

/**
 * isLineEmpty - check whether a given input line is empty or has only spaces
 *
 * @param line - The line to check
 *
 * @return
 *  true    - line is empty or spaces-only, or NULL argument
 *  false   - line is not empty.
 */
static bool isLineEmpty(char* line) {
    int length;
    if (NULL == line) return true;
    length = strlen(line);
    for (int i = 0; i < length; i++) {
        if (!isspace(line[i])) {
            return false;
        }
    }
  return true;
}

/**
 * pokedexAddFromLine - Parse a single line from the pokedex input file
 *
 * @param pokedex   - A pokedex type to add the parsed line to
 * @param line      - The line to parse
 *
 * @return
 *  false   - memory error or NULL argument.
 *  true    - successful parse and add to pokedex
 */
static bool pokedexAddFromLine(Pokedex pokedex, char* line) {
  if (NULL == pokedex || NULL == line) return false;
  char *species, *initial_cp_str, *types_part;
  int initial_cp;
  Set types = setCreate((copySetElements)stringCopy, (freeSetElements)free,
                        (compareSetElements)stringCompare);
  PokedexErrorCode pokedex_add_result;
  if (NULL == types) return false;

  species = strtok(line, " \t");
  initial_cp_str = strtok(NULL, " \t");
  types_part = strtok(NULL, "\r\n");
  WORD_FOREACH(types_part, type) {
    if (SET_OUT_OF_MEMORY == setAdd(types, type)) {
      setDestroy(types);
      return false;
    }
  }
  initial_cp = atoi(initial_cp_str);
  pokedex_add_result = pokedexAddPokemon(pokedex, species, initial_cp, types);
  setDestroy(types);
  if (SET_SUCCESS != pokedex_add_result) return false;
  return true;
}

Pokedex pokedexCreateFromFile(FILE* file) {
  if (NULL == file) return NULL;
  Pokedex pokedex = pokedexCreate();
  char line[MAX_STR_LENGTH + 1];
  if (NULL == pokedex) {
    pokedexDestroy(pokedex);
    return NULL;
  }
  while (fgets(line, MAX_STR_LENGTH, file) != NULL) {
  if (isLineEmpty(line)) continue;
    if (!pokedexAddFromLine(pokedex, line)) {
      pokedexDestroy(pokedex);
      return NULL;
    }
  }
  assert(feof(file));
  return pokedex;
}

Evolutions evolutionsCreateFromFile(FILE* file, Pokedex pokedex) {
  if (NULL == file || NULL == pokedex) return NULL;
  Evolutions evolutions = evolutionsCreate();
  if (NULL == evolutions) return NULL;
  char line[MAX_STR_LENGTH + 1];
  char pokemon[MAX_STR_LENGTH], evolution_name[MAX_STR_LENGTH];
  int level;
  PokedexEntry evolution;
  EvolutionsErrorCode add_result;
  while (fgets(line, MAX_STR_LENGTH, file) != NULL) {
  if (isLineEmpty(line)) continue;
  sscanf(line, "%s %s %d", pokemon, evolution_name, &level);
    evolution = pokedexGetPokemonInfo(pokedex, evolution_name);
    assert(evolution);
    add_result = evolutionsAddEntry(evolutions, pokemon, level, evolution);
    if (EVOLUTIONS_OUT_OF_MEMORY == add_result) {
      evolutionsDestroy(evolutions);
      return NULL;
    }
  }
  assert(feof(file));
  return evolutions;
}

/**
 * locationAddPokemonFromLine - Parse a locaiton's line's pokemon part
 *
 * @param location      - A Location type.
 * @param pokemon_part  - The pokemon part of the line being parsed.
 * @param pokedex       - A valid Pokedex.
 * @param evolutions    - A valid Evolutions map.
 *
 * @return
 *  false - NULL argument or memory error
 *  true  - Successful parsing.
 */
static bool locationAddPokemonFromLine(Location location, char* pokemon_part,
                                       Pokedex pokedex, Evolutions evolutions) {
  if (NULL == location || NULL == pokedex || NULL == evolutions) return false;
  if (NULL == pokemon_part) return true;
  
  PokedexEntry pokemon_info;
  Pokemon pokemon;
  LocationErrorCode add_result;
  WORD_FOREACH(pokemon_part, species) {
    pokemon_info = pokedexGetPokemonInfo(pokedex, species);
    assert(pokemon_info);
    pokemon = pokemonCreate(pokemon_info, evolutions);
    if (NULL == pokemon) return false;
    add_result = locationAppendPokemon(location, pokemon);
    pokemonDestroy(pokemon);
    if (LOCATION_OUT_OF_MEMORY == add_result) return false;
  }
  return true;
}

/**
 * locationAddNeighborsFromLine - Parse a location's line's neighbors part 
 *
 * @param location       - A Location type.
 * @param neighbors_part - The neighbors part of the line being parsed.
 *
 * @return
 *  false   - NULL argument or memory error
 *  true    - Parsing was successful.
 */
static bool locationAddNeighborsFromLine(Location location,
                                         char* neighbors_part) {
  if (NULL == location) return false;
  if (NULL == neighbors_part) return true;

  LocationErrorCode add_result;
  WORD_FOREACH(neighbors_part, neighbor) {
    add_result = locationAddNeighbor(location, neighbor);
    if (LOCATION_OUT_OF_MEMORY == add_result) return false;
  }
  return true;
}

/**
 * locationFromLine - Create a Location type from a single input line
 *
 * @param line      - A line from a locations input file.
 * @param pokedex   - A valid Pokedex.
 * @param evolutions- A valid Evolutions map.
 *
 * @return
 *  NULL    - NULL arg or memory allocation error
 *  A valid Location in case of success.
 */
static Location locationFromLine(char* line, Pokedex pokedex,
                                    Evolutions evolutions) {
  if (NULL == line || NULL == pokedex || NULL == evolutions) return NULL;

  char *name_and_pokemon_part, *neighbors_part, *location_name, *pokemon_part;
  Location location;
  bool add_pokemon_result, add_neighbors_result;

  // Basic line parts
  name_and_pokemon_part = strtok(line, ";");
  neighbors_part = strtok(NULL, "\r\n");
  // Finer splitting
  location_name = strtok(name_and_pokemon_part, " \t\r\n");
  assert(location_name);
  pokemon_part = strtok(NULL, ";");
  // Now create the object.
  location = locationCreate(location_name);
  add_pokemon_result = locationAddPokemonFromLine(location, pokemon_part,
                                                  pokedex, evolutions);
  add_neighbors_result = locationAddNeighborsFromLine(location,
                                                      neighbors_part);
  // NULL in locationCreate will also be detected here, because the above
  // functions check for NULL argument.
  if (!add_pokemon_result || !add_neighbors_result) {
    locationDestroy(location);
    return NULL;
  }
  return location;
}

Map locationsCreateMapFromFile(FILE* file, Pokedex pokedex,
                               Evolutions evolutions) {
  if (NULL == file || NULL == pokedex || NULL == evolutions) return NULL;
  Map locations = mapCreate((copyMapKeyElements)stringCopy,
                            (copyMapDataElements)locationCopy,
                            (freeMapKeyElements)free,
                            (freeMapDataElements)locationDestroy,
                            (compareMapKeyElements)stringCompare);
  if (NULL == locations) return NULL;
  char line[MAX_STR_LENGTH + 1];
  Location location;
  MapResult add_location_result;
  while (fgets(line, MAX_STR_LENGTH, file) != NULL) {
  if (isLineEmpty(line)) continue;
    location = locationFromLine(line, pokedex, evolutions);
    if (NULL == location) {
      mapDestroy(locations);
      return NULL;
    }
    add_location_result = mapPut(locations, locationGetName(location),
                                 location);
    locationDestroy(location);
    if (MAP_OUT_OF_MEMORY == add_location_result) {
      mapDestroy(locations);
      return NULL;
    }
  }
  assert(feof(file));
  return locations;
}

bool parseArgs(int argc, char** argv, char** pokedex_filename,
               char** evolutions_filename, char** locations_filename,
               char** input_filename, char** output_filename) {
  if (argc < 7 || argc > 11 || argc % 2 == 0) return false;
  if (strcmp(argv[1], "-p") || strcmp(argv[3], "-e") || strcmp(argv[5], "-l")) {
    return false;
  }
  *input_filename = *output_filename = NULL;
  if (argc == 11) {
    if (strcmp(argv[7], "-i") || strcmp(argv[9], "-o")) {
      return false;
    }
    *input_filename = argv[8];
    *output_filename = argv[10];
  } else if (argc == 9) {
    if (strcmp(argv[7], "-i") == 0) {
      *input_filename = argv[8];
    } else if (strcmp(argv[7], "-o") == 0) {
      *output_filename = argv[8];
    } else {
      return false;
    }
  }
  *pokedex_filename = argv[2];
  *evolutions_filename = argv[4];
  *locations_filename = argv[6];
  return true;
}

void tokenizeCommand(char* line, char** command, char** subcommand,
                     char** args) {
  char *args_part;
  if (isLineEmpty(line)) {
    *command = NULL;
    return;
  }
  *command = strtok(line, " \t");
  *subcommand = strtok(NULL, " \t\r\n");
  args_part = strtok(NULL, "\r\n");
  int arg_num = 0;
  WORD_FOREACH(args_part, arg) {
    args[arg_num] = arg;
    arg_num++;
  }
}
