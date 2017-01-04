#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "parser.h"
#include "pokemonGo.h"

#define ERR_CHANNEL stderr

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

void cleanUp(Pokedex pokedex, Evolutions evolutions, Map locations,
             PokemonGo game) {
  pokedexDestroy(pokedex);
  evolutionsDestroy(evolutions);
  mapDestroy(locations);
  pokemongoDestroy(game);
}

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
  if (output_filename) *output_file = fopen(output_filename, "w"); //TODO: check with appending
  if (NULL == *input_file || NULL == *output_file || NULL == *locations_file ||
      NULL == *pokedex_file || NULL == *evolutions_file) {
    cleanUpFiles(*input_file, *output_file, *pokedex_file, *evolutions_file,
                 *locations_file);
    mtmPrintErrorMessage(ERR_CHANNEL, MTM_CANNOT_OPEN_FILE);
    return false;
  }
  return true;
}

MtmErrorCode convertErrorToMtm(PokemonGoErrorCode pokemon_go_code) {
  switch (pokemon_go_code) {
  case POKEMONGO_SUCCESS:
    return MTM_SUCCESS;
  case POKEMONGO_OUT_OF_MEMORY:
    return MTM_OUT_OF_MEMORY;
  case POKEMONGO_INVALID_ARGUMENT:
    return MTM_INVALID_ARGUMENT;
  case POKEMONGO_TRAINER_NAME_ALREADY_EXISTS:
    return MTM_TRAINER_NAME_ALREADY_EXISTS;
  case POKEMONGO_TRAINER_DOES_NOT_EXIST:
    return MTM_TRAINER_DOES_NOT_EXIST;
  case POKEMONGO_LOCATION_DOES_NOT_EXIST:
    return MTM_LOCATION_DOES_NOT_EXIST;
  case POKEMONGO_POKEMON_DOES_NOT_EXIST:
    return MTM_POKEMON_DOES_NOT_EXIST;
  case POKEMONGO_ITEM_OUT_OF_STOCK:
    return MTM_ITEM_OUT_OF_STOCK;
  case POKEMONGO_BUDGET_IS_NOT_SUFFICIENT:
    return MTM_BUDGET_IS_NOT_SUFFICIENT;
  case POKEMONGO_HP_IS_AT_MAX:
    return MTM_HP_IS_AT_MAX;
  case POKEMONGO_NO_AVAILABLE_ITEM_FOUND:
    return MTM_NO_AVAILABLE_ITEM_FOUND;
  case POKEMONGO_LOCATION_IS_NOT_REACHABLE:
    return MTM_LOCATION_IS_NOT_REACHABLE;
  case POKEMONGO_TRAINER_ALREADY_IN_LOCATION:
    return MTM_TRAINER_ALREADY_IN_LOCATION;
  }
  // This should never happen
  assert(false);
  return MTM_SUCCESS;
}

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

int main(int argc, char** argv) {

  FILE* locations_file;
  FILE* pokedex_file;
  FILE* evolutions_file;
  FILE* input = stdin;
  FILE* output = stdout;

  if (!openFilesFromArgs(argc, argv, &pokedex_file, &evolutions_file,
                 &locations_file, &input, &output)) {
    return 0;
  }

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
  playGame(game, input);
  cleanUp(pokedex, evolutions, locations, game);
  cleanUpFiles(input, output, pokedex_file, evolutions_file, locations_file);
  return 0;
}
