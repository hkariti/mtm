#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "pokemonGo.h"

#define ERR_CHANNEL stderr

/**
 * cleanUpFiles - Close all of the game's files.
 *
 * @param input_file        - File for input commands.
 * @param output_file       - File for program output.
 * @param pokedex_file      - File with the game's input pokedex.
 * @param evolutions_file   - File with the game's input evolution map.
 * @param locations_file    - File with the game's input locations.
 *
 * All arguments can be NULL. If so, they're skipped.
 */
void cleanUpFiles(FILE* input_file, FILE* output_file, FILE* pokedex_file,
                  FILE* evolutions_file, FILE* locations_file) {
	if (input_file != NULL) {
		fclose(input_file);
	}
	if (output_file != NULL) {
		fclose(output_file);
	}
	if (pokedex_file != NULL) {
		fclose(pokedex_file);
	}
	if (evolutions_file != NULL) {
		fclose(evolutions_file);
	}
	if (locations_file != NULL) {
		fclose(locations_file);
	}
}

/**
 * cleanUp - destroy all game objects
 *
 * @param pokedex   - The game's Pokedex
 * @param evolutions- The game's evolutions map.
 * @param locations - The game's locations map.
 * @param game      - The game object itself.
 */
void cleanUp(Pokedex pokedex, Evolutions evolutions, Map locations,
             PokemonGo game) {
  pokedexDestroy(pokedex);
  evolutionsDestroy(evolutions);
  mapDestroy(locations);
  pokemongoDestroy(game);
}

/**
 * openFilesFromArgs - Open game files based on command line arguments
 *
 * @param argc  - number of cli arguments
 * @param argv  - an array of words, containig the cli arguments
 * @param pokedex_file      - Will be filled with the open file object for the
 *                            pokedex.
 * @param evolutions_file   - Will be filled with the open file object for the
 *                            evolutions map.
 * @param locations_file    - Will be filled with the open file object for the
 *                            locations map.
 * @param input_file        - Will be filled with the open file object for the
 *                            input commands.
 * @param output_file       - Will be filled with the open file object for the
 *                            program output.
 *
 * @return
 *  false - Invalid cli arguments or failure to open one of the files.
 *  true  - Successful run
 */
bool openFilesFromArgs(int argc, char** argv, FILE** pokedex_file,
                       FILE** evolutions_file, FILE** locations_file,
                       FILE** input_file, FILE** output_file) {
  char* pokedex_filename;
  char* evolutions_filename;
  char* locations_filename;
  char* input_filename;
  char* output_filename;
  if (!parseArgs(argc, argv, &pokedex_filename, &evolutions_filename,
                 &locations_filename, &input_filename, &output_filename)) {
    mtmPrintErrorMessage(ERR_CHANNEL, MTM_INVALID_COMMAND_LINE_PARAMETERS);
    return false;
  }

  *locations_file = fopen(locations_filename, "r");
  *pokedex_file = fopen(pokedex_filename, "r");
  *evolutions_file = fopen(evolutions_filename, "r");
  if (input_filename) *input_file = fopen(input_filename, "r");
  if (output_filename) *output_file = fopen(output_filename, "w");
  if (NULL == *input_file || NULL == *output_file || NULL == *locations_file ||
      NULL == *pokedex_file || NULL == *evolutions_file) {
    cleanUpFiles(*input_file, *output_file, *pokedex_file, *evolutions_file,
                 *locations_file);
    mtmPrintErrorMessage(ERR_CHANNEL, MTM_CANNOT_OPEN_FILE);
    return false;
  }
  return true;
}

/**
 * convertErrorToMtm - Convert PokemonGo error codes to Mtm error codes.
 *
 * @param pokemon_go_code - PokemonGo error code to convert
 *
 * @return
 *  The relevant Mtm error code for this error
 */
MtmErrorCode convertErrorToMtm(PokemonGoErrorCode pokemon_go_code) {
  // A switch clause is more appropriate, but is passing the 30 lines limit.
  if (POKEMONGO_SUCCESS == pokemon_go_code) {
      return MTM_SUCCESS;
  } else if (POKEMONGO_OUT_OF_MEMORY == pokemon_go_code) {
      return MTM_OUT_OF_MEMORY;
  } else if (POKEMONGO_INVALID_ARGUMENT == pokemon_go_code) {
      return MTM_INVALID_ARGUMENT;
  } else if (POKEMONGO_TRAINER_NAME_ALREADY_EXISTS == pokemon_go_code) {
      return MTM_TRAINER_NAME_ALREADY_EXISTS;
  } else if (POKEMONGO_TRAINER_DOES_NOT_EXIST == pokemon_go_code) {
      return MTM_TRAINER_DOES_NOT_EXIST;
  } else if (POKEMONGO_LOCATION_DOES_NOT_EXIST == pokemon_go_code) {
      return MTM_LOCATION_DOES_NOT_EXIST;
  } else if (POKEMONGO_POKEMON_DOES_NOT_EXIST == pokemon_go_code) {
      return MTM_POKEMON_DOES_NOT_EXIST;
  } else if (POKEMONGO_ITEM_OUT_OF_STOCK == pokemon_go_code) {
      return MTM_ITEM_OUT_OF_STOCK;
  } else if (POKEMONGO_BUDGET_IS_NOT_SUFFICIENT == pokemon_go_code) {
      return MTM_BUDGET_IS_NOT_SUFFICIENT;
  } else if (POKEMONGO_HP_IS_AT_MAX == pokemon_go_code) {
      return MTM_HP_IS_AT_MAX;
  } else if (POKEMONGO_NO_AVAILABLE_ITEM_FOUND == pokemon_go_code) {
      return MTM_NO_AVAILABLE_ITEM_FOUND;
  } else if (POKEMONGO_LOCATION_IS_NOT_REACHABLE == pokemon_go_code) {
      return MTM_LOCATION_IS_NOT_REACHABLE;
  } else if (POKEMONGO_TRAINER_ALREADY_IN_LOCATION == pokemon_go_code) {
      return MTM_TRAINER_ALREADY_IN_LOCATION;
  }
  assert(false); // we should never reach here
  return MTM_SUCCESS;
}

/**
 * runTrainerCommand - Run the relevant trainer command, based on subcommand
 *
 * @param game          - A pokemonGo game
 * @param subcommand    - The subcommand
 * @aram args           - An array of args to the subcommand
 *
 * The args are assumed to be valid
 */
MtmErrorCode runTrainerCommand(PokemonGo game, char* subcommand, char** args) {
  PokemonGoErrorCode command_result = POKEMONGO_SUCCESS; 
  if (strcmp(subcommand, "add") == 0) {
    command_result = pokemongoTrainerAdd(game, args[0], atoi(args[1]), args[2]);
  } else if (strcmp(subcommand, "go") == 0) {
    command_result = pokemongoTrainerGo(game, args[0], args[1]);
  } else if (strcmp(subcommand, "purchase") == 0) {
    command_result = pokemongoTrainerPurchase(game, args[0], args[1],
                                              atoi(args[2]));
  } else {
    // This should never happen
    assert(false);
  }
  return convertErrorToMtm(command_result);
}

/**
 * runStoreCommand - Run the relevant store command, based on subcommand
 *
 * @param game          - A pokemonGo game
 * @param subcommand    - The subcommand
 * @aram args           - An array of args to the subcommand
 *
 * The args are assumed to be valid
 */
MtmErrorCode runStoreCommand(PokemonGo game, char* subcommand, char** args) {
  PokemonGoErrorCode command_result = POKEMONGO_SUCCESS; 
  if (strcmp(subcommand, "add") == 0) {
    command_result = pokemongoStoreAdd(game, args[0], atoi(args[1]),
                                       atoi(args[2]));
  } else {
    // This should never happen
    assert(false);
  }
  return convertErrorToMtm(command_result);
}

/**
 * runPokemonCommand - Run the relevant pokemon command, based on subcommand
 *
 * @param game          - A pokemonGo game
 * @param subcommand    - The subcommand
 * @aram args           - An array of args to the subcommand
 *
 * The args are assumed to be valid
 */
MtmErrorCode runPokemonCommand(PokemonGo game, char* subcommand, char** args) {
  PokemonGoErrorCode command_result = POKEMONGO_SUCCESS; 
  if (strcmp(subcommand, "heal") == 0) {
    command_result = pokemongoPokemonHeal(game, args[0], atoi(args[1]));
  } else if (strcmp(subcommand, "train") == 0) {
    command_result = pokemongoPokemonTrain(game, args[0], atoi(args[1]));
  } else {
    // This should never happen
    assert(false);
  }
  return convertErrorToMtm(command_result);
}

/**
 * runBattleCommand - Run the relevant battle command, based on subcommand
 *
 * @param game          - A pokemonGo game
 * @param subcommand    - The subcommand
 * @aram args           - An array of args to the subcommand
 *
 * The args are assumed to be valid
 */
MtmErrorCode runBattleCommand(PokemonGo game, char* subcommand, char** args) {
  PokemonGoErrorCode command_result = POKEMONGO_SUCCESS; 
  if (strcmp(subcommand, "fight") == 0) {
    command_result = pokemongoBattleFight(game, args[0], args[1],
                                          atoi(args[2]), atoi(args[3]));
  } else {
    // This should never happen
    assert(false);
  }
  return convertErrorToMtm(command_result);
}

/**
 * runReportCommand - Run the relevant report command, based on subcommand
 *
 * @param game          - A pokemonGo game
 * @param subcommand    - The subcommand
 * @aram args           - An array of args to the subcommand
 *
 * The args are assumed to be valid
 */
MtmErrorCode runReportCommand(PokemonGo game, char* subcommand, char** args) {
  PokemonGoErrorCode command_result = POKEMONGO_SUCCESS; 
  if (strcmp(subcommand, "trainer") == 0) {
    command_result = pokemongoReportTrainer(game, args[0]);
  } else if (strcmp(subcommand, "locations") == 0) {
    command_result = pokemongoReportLocations(game);
  } else if (strcmp(subcommand, "stock") == 0) {
    command_result = pokemongoReportStock(game);
  } else {
    // This should never happen
    assert(false);
  }
  return convertErrorToMtm(command_result);
}

/**
 * playGame - the game flow.
 *
 * @param game  - a pokemonGo type
 * @param input - the input file to read commands from
 *
 * The flow is to read an input command, parse it and call the relevant
 * pokemonGo function to handle it. The flow is ended if there's a memory error
 * or if EOF is reached in the input file.
 */
void playGame(PokemonGo game, FILE* input) {
  char *command, *subcommand, *args[10];
  char line[MAX_STR_LENGTH];
  MtmErrorCode command_result;
  while (fgets(line, MAX_STR_LENGTH, input)) {
    tokenizeCommand(line, &command, &subcommand, args);
	if (NULL == command) {
		continue;
	} else if (strcmp(command, "trainer") == 0) {
      command_result = runTrainerCommand(game, subcommand, args);
    } else if (strcmp(command, "store") == 0) {
      command_result = runStoreCommand(game, subcommand, args);
    } else if (strcmp(command, "pokemon") == 0) {
      command_result = runPokemonCommand(game, subcommand, args);
    } else if (strcmp(command, "battle") == 0) {
      command_result = runBattleCommand(game, subcommand, args);
    } else if (strcmp(command, "report") == 0) {
      command_result = runReportCommand(game, subcommand, args);
    } else {
      // This should never happen
      assert(false);
    }
    if (command_result != MTM_SUCCESS) {
      mtmPrintErrorMessage(ERR_CHANNEL, command_result);
      if (MTM_OUT_OF_MEMORY == command_result) return;
    }
  }
  assert(feof(input));
}

/**
 * main - Basic init and start and game flow
 *
 * @params in the form of cli arguments:
 *  -p POKEDEX_FILE -e EVOLUTIONS_FILE -l LOCATIONS_FILE [-i INPUT_FILE]
 *      [-o OUTPUT_FILE]
 *
 * cli arguments MUST be passed in the given order. Arguments in brackets are
 * optional.
 */
int main(int argc, char** argv) {
  FILE* locations_file;
  FILE* pokedex_file;
  FILE* evolutions_file;
  FILE* input = stdin;
  FILE* output = stdout;
  // Open input files
  if (!openFilesFromArgs(argc, argv, &pokedex_file, &evolutions_file,
                 &locations_file, &input, &output)) {
    return 0;
  }
  // Parse input files and create the game object
  Pokedex pokedex = pokedexCreateFromFile(pokedex_file);
  Evolutions evolutions = evolutionsCreateFromFile(evolutions_file, pokedex);
  Map locations = locationsCreateMapFromFile(locations_file, pokedex,
                                             evolutions);
  PokemonGo game = pokemongoCreate(pokedex, evolutions, locations, output);
  if (NULL == game || NULL == pokedex || NULL == evolutions ||
      NULL == locations) {
    cleanUp(pokedex, evolutions, locations, game);
    cleanUpFiles(input, output, pokedex_file, evolutions_file, locations_file);
    mtmPrintErrorMessage(ERR_CHANNEL, MTM_OUT_OF_MEMORY);
    return 0;
  }
  // Play! :D
  playGame(game, input);
  cleanUp(pokedex, evolutions, locations, game);
  cleanUpFiles(input, output, pokedex_file, evolutions_file, locations_file);
  return 0;
}
