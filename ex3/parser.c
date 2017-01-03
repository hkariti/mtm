#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "parser.h"
#include "pokemon.h"
#include "utils.h"

#define WORD_FOREACH(str, iterator)                          \
  for (char* iterator = strtok(str, " \t\r\n"); iterator != NULL;\
       iterator = strtok(NULL, " \t\r\n"))

static bool isLineEmpty(char* line) {
	char* line_copy;
	char* trimmed_line;
	line_copy = stringCopy(line);
	if (NULL == line_copy) return true;
	trimmed_line = strtok(line_copy, " \t\r\n");
	free(line_copy);
	if (NULL == trimmed_line || 0 == strlen(trimmed_line)) return true;
	return false;
}

static bool pokedexAddFromLine(Pokedex pokedex, char* line) {
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

Pokedex createPokedexFromFile(FILE* file) {
  if (NULL == file) return NULL;
  Pokedex pokedex = createPokedex();
  char line[MAX_STR_LENGTH + 1];
  if (NULL == pokedex) {
    destroyPokedex(pokedex);
    return NULL;
  }
  while (fgets(line, MAX_STR_LENGTH, file) != NULL) {
	if (isLineEmpty(line)) continue;
    if (!pokedexAddFromLine(pokedex, line)) {
      destroyPokedex(pokedex);
      return NULL;
    }
  }
  assert(feof(file));
  return pokedex;
}

Evolutions createEvolutionsFromFile(FILE* file, Pokedex pokedex) {
  if (NULL == file || NULL == pokedex) return NULL;
  Evolutions evolutions = createEvolutions();
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
      destroyEvolutions(evolutions);
      return NULL;
    }
  }
  assert(feof(file));
  return evolutions;
}

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
    pokemon = createPokemon(pokemon_info, evolutions);
    if (NULL == pokemon) return false;
    add_result = locationAppendPokemon(location, pokemon);
    destroyPokemon(pokemon);
    if (LOCATION_OUT_OF_MEMORY == add_result) return false;
  }
  return true;
}

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
  location_name = strtok(name_and_pokemon_part, " \r\n");
  assert(location_name);
  pokemon_part = strtok(NULL, ";");
  // Now create the object
  location = createLocation(location_name);
  add_pokemon_result = locationAddPokemonFromLine(location, pokemon_part,
                                                  pokedex, evolutions);
  add_neighbors_result = locationAddNeighborsFromLine(location,
                                                      neighbors_part);
  if (!add_pokemon_result || !add_neighbors_result) {
    destroyLocation(location);
    return NULL;
  }
  return location;
}

Map createLocationsMapFromFile(FILE* file, Pokedex pokedex,
                               Evolutions evolutions) {
  if (NULL == file || NULL == pokedex || NULL == evolutions) return NULL;
  Map locations = mapCreate((copyMapKeyElements)stringCopy,
                            (copyMapDataElements)copyLocation,
                            (freeMapKeyElements)free,
                            (freeMapDataElements)destroyLocation,
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
    add_location_result = mapPut(locations, locationGetName(location), location);
    destroyLocation(location);
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
  *command = strtok(line, " \t");
  *subcommand = strtok(NULL, " \t\r\n");
  args_part = strtok(NULL, "\r\n");
  int arg_num = 0;
  WORD_FOREACH(args_part, arg) {
    args[arg_num] = arg;
    arg_num++;
  }
}
