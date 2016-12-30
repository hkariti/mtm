#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "pokemonGo.h"
#include "utils.h"
#include "print_utils.h"

struct PokemonGo_t {
	Evolutions evolutions;
	Pokedex pokedex;
	Map locations;
	Map trainers;
	Store store;
	FILE* output_channel;
};

PokemonGo createPokemonGo(Pokedex pokedex, Evolutions evolutions, Map locations,
                          FILE* output_channel) {
	if (NULL == pokedex || NULL == evolutions || NULL == locations ||
      NULL == output_channel) {
    return NULL; 
  }

	PokemonGo pokemon_go = malloc(sizeof(*pokemon_go));
	pokemon_go->pokedex = pokedex;
	pokemon_go->evolutions = evolutions;
	pokemon_go->locations = locations;
	pokemon_go->output_channel = output_channel;
	pokemon_go->trainers = mapCreate((copyMapKeyElements)stringCopy,
                                   (copyMapDataElements)trainerCopy,
                                   (freeMapKeyElements)free,
                                   (freeMapDataElements)destroyTrainer,
                                   (compareMapKeyElements)strcmp);
	if (NULL == pokemon_go->trainers) {
		free(pokemon_go);
		return NULL;
	}
	pokemon_go->store = storeCreate();
	if (NULL == pokemon_go->store) {
		mapDestroy(pokemon_go->trainers);
		free(pokemon_go);
		return NULL;
	}
	return pokemon_go;
}

void pokemongoDestroy(PokemonGo pokemon_go) {
	if (NULL == pokemon_go) return;
	mapDestroy(pokemon_go->trainers);
	storeDestroy(pokemon_go->store);
	free(pokemon_go);
}

PokemonGoErrorCode pokemongoTrainerAdd(PokemonGo pokemon_go, char* trainer_name,
                                       int budget, char* start_location) {
	if (NULL == pokemon_go || NULL == trainer_name ||
      NULL == start_location) {
    return POKEMONGO_INVALID_ARGUMENT; 
  }

	if (budget < 0) return POKEMONGO_INVALID_ARGUMENT;
	
	Trainer trainer = mapGet(pokemon_go->trainers, trainer_name);
	if (trainer != NULL) return POKEMONGO_TRAINER_NAME_ALREADY_EXISTS;

	Location location = mapGet(pokemon_go->locations, start_location);
	if (NULL == location) return POKEMONGO_LOCATION_DOES_NOT_EXIST;

	trainer = createTrainer(trainer_name, budget, location);
	if (NULL == trainer) return POKEMONGO_OUT_OF_MEMORY;
    TrainerErrorCode hunt_result = trainerHunt(trainer,
                                               pokemon_go->output_channel);
    if (hunt_result == TRAINER_OUT_OF_MEMORY) {
        destroyTrainer(trainer);
        return POKEMONGO_OUT_OF_MEMORY; 
    }

	MapResult map_result = mapPut(pokemon_go->trainers, trainer_name, trainer);
    destroyTrainer(trainer);
	if (map_result == MAP_OUT_OF_MEMORY) return POKEMONGO_OUT_OF_MEMORY;
	// we checked all mapPut arg, so out of mem is the only possible error

	return POKEMONGO_SUCCESS;
}

PokemonGoErrorCode pokemongoTrainerGo(PokemonGo pokemon_go, char* trainer_name,
                                      char* new_location) {
	if (NULL == pokemon_go || NULL == trainer_name || NULL == new_location) {
    return POKEMONGO_INVALID_ARGUMENT; 
  }

	Trainer trainer = mapGet(pokemon_go->trainers, trainer_name);
	if (NULL == trainer) return POKEMONGO_TRAINER_DOES_NOT_EXIST;

	Location location = mapGet(pokemon_go->locations, new_location);
	if (NULL == location) return POKEMONGO_LOCATION_DOES_NOT_EXIST;
	
	TrainerErrorCode result = trainerGoToLocation(trainer, location);
	if (result == TRAINER_LOCATION_IS_NOT_REACHABLE) {
    return POKEMONGO_LOCATION_IS_NOT_REACHABLE; 
  }
	if (result == TRAINER_ALREADY_IN_LOCATION) {
    return POKEMONGO_TRAINER_ALREADY_IN_LOCATION; 
  }

	result = trainerHunt(trainer, pokemon_go->output_channel);
	if (result == TRAINER_OUT_OF_MEMORY) return POKEMONGO_OUT_OF_MEMORY;

	return POKEMONGO_SUCCESS;
}

PokemonGoErrorCode pokemongoStoreAdd(PokemonGo pokemon_go, char* item_type,
                                     int value, int quantity) {
	if (NULL == pokemon_go || NULL == item_type) return POKEMONGO_INVALID_ARGUMENT;

	if (quantity <= 0) return POKEMONGO_INVALID_ARGUMENT;

	StoreErrorCode result;
	for (int i = 0; i < quantity; i++) {
		result = storeAddItem(pokemon_go->store, item_type, value);
		if (result == STORE_OUT_OF_MEMORY) return POKEMONGO_OUT_OF_MEMORY;
		if (result == STORE_INVALID_ARGUMENT) return POKEMONGO_INVALID_ARGUMENT;
	}

	return POKEMONGO_SUCCESS;
}

PokemonGoErrorCode pokemongoTrainerPurchase(PokemonGo pokemon_go,
                                            char* trainer_name,
                                            char* item_type, int value) {
	if (NULL == pokemon_go || NULL == trainer_name || NULL == item_type) {
    return POKEMONGO_INVALID_ARGUMENT; 
  }

	if (strcmp(item_type, "potion") != 0 &&
		strcmp(item_type, "candy") != 0) return POKEMONGO_INVALID_ARGUMENT;
	if (value <= 0) return POKEMONGO_INVALID_ARGUMENT;

	Trainer trainer = mapGet(pokemon_go->trainers, trainer_name);
	if (NULL == trainer) return POKEMONGO_TRAINER_DOES_NOT_EXIST;

	TrainerErrorCode result = trainerBuyItem(trainer, pokemon_go->store,
                                           item_type, value);
	if (result == TRAINER_STORE_ITEM_OUT_OF_STOCK) {
    return POKEMONGO_ITEM_OUT_OF_STOCK;
  }
  if (result == TRAINER_BUDGET_IS_INSUFFICIENT) {
    return POKEMONGO_BUDGET_IS_NOT_SUFFICIENT; 
  }
	if (result == TRAINER_OUT_OF_MEMORY) {
    return POKEMONGO_OUT_OF_MEMORY; 
  }

	return POKEMONGO_SUCCESS;
}

PokemonGoErrorCode pokemongoBattleFight(PokemonGo pokemon_go,
                                        char* trainer1_name, char* trainer2_name,
                                        int pokemon1_id, int pokemon2_id) {
	if (NULL == pokemon_go || NULL == trainer1_name || NULL == trainer2_name) {
    return POKEMONGO_INVALID_ARGUMENT; 
  }

	if (strcmp(trainer1_name, trainer2_name) == 0) {
    return POKEMONGO_INVALID_ARGUMENT; 
  }

	Trainer trainer_1 = mapGet(pokemon_go->trainers, trainer1_name);
	Trainer trainer_2 = mapGet(pokemon_go->trainers, trainer2_name);
	if (NULL == trainer_1 || NULL == trainer_2) {
    return POKEMONGO_TRAINER_DOES_NOT_EXIST; 
  }

	Pokemon pokemon_1 = getTrainerPokemon(trainer_1, pokemon1_id);
	Pokemon pokemon_2 = getTrainerPokemon(trainer_2, pokemon2_id);
	if (NULL == pokemon_1 || NULL == pokemon_2) {
    return POKEMONGO_POKEMON_DOES_NOT_EXIST; 
  }
	char* pokemon1_name = pokemonGetSpecies(pokemon_1);
	char* pokemon2_name = pokemonGetSpecies(pokemon_2);
	int cp_1 = pokemonGetCP(pokemon_1);
	int cp_2 = pokemonGetCP(pokemon_2);
	double old_hp_1 = pokemonGetHP(pokemon_1);
	double old_hp_2 = pokemonGetHP(pokemon_2);
	int old_level_1 = pokemonGetLevel(pokemon_1);
	int old_level_2 = pokemonGetLevel(pokemon_2);
	double old_xp_1 = getTrainerXP(trainer_1);
	double old_xp_2 = getTrainerXP(trainer_2);

	trainersBattle(trainer_1, pokemon1_id, trainer_2, pokemon2_id);

	double new_hp_1 = pokemonGetHP(pokemon_1);
	double new_hp_2 = pokemonGetHP(pokemon_2);
	int new_level_1 = pokemonGetLevel(pokemon_1);
	int new_level_2 = pokemonGetLevel(pokemon_2);
	double new_xp_1 = getTrainerXP(trainer_1);
	double new_xp_2 = getTrainerXP(trainer_2);
	bool is_dead_1, is_dead_2;
	isPokemonDead(pokemon_1, &is_dead_1);
	isPokemonDead(pokemon_2, &is_dead_2);
	mtmPrintBattle(pokemon_go->output_channel, trainer1_name, trainer2_name,
                 pokemon1_name, pokemon2_name, cp_1, cp_2, old_hp_1, old_hp_2,
                 new_hp_1, new_hp_2, old_level_1, old_level_2, new_level_1,
                 new_level_2, old_xp_1, old_xp_2, new_xp_1, new_xp_2, is_dead_1,
                 is_dead_2);
	if (is_dead_1) trainerRemovePokemon(trainer_1, pokemon1_id);
	if (is_dead_2) trainerRemovePokemon(trainer_2, pokemon2_id);
	return POKEMONGO_SUCCESS;
}

PokemonGoErrorCode pokemongoPokemonHeal(PokemonGo pokemon_go, char* trainer_name,
                                        int pokemon_id) {
	if (NULL == pokemon_go || NULL == trainer_name) {
    return POKEMONGO_INVALID_ARGUMENT; 
  }

	Trainer trainer = mapGet(pokemon_go->trainers, trainer_name);
	if (NULL == trainer) return POKEMONGO_TRAINER_DOES_NOT_EXIST;

	TrainerErrorCode result = trainerHealPokemon(trainer, pokemon_id);
	if (result == TRAINER_POKEMON_DOESNT_EXIST) {
    return POKEMONGO_POKEMON_DOES_NOT_EXIST; 
  }
	if (result == TRAINER_POKEMON_HP_IS_AT_MAX) {
    return POKEMONGO_HP_IS_AT_MAX; 
  }
	if (result == TRAINER_NO_AVAILABLE_ITEM_FOUND) {
    return POKEMONGO_NO_AVAILABLE_ITEM_FOUND; 
  }

	return POKEMONGO_SUCCESS;
}

PokemonGoErrorCode pokemongoPokemonTrain(PokemonGo pokemon_go,
                                         char* trainer_name, int pokemon_id) {
	if (NULL == pokemon_go || NULL == trainer_name) {
    return POKEMONGO_INVALID_ARGUMENT; 
  }

	Trainer trainer = mapGet(pokemon_go->trainers, trainer_name);
	if (NULL == trainer) return POKEMONGO_TRAINER_DOES_NOT_EXIST;

	TrainerErrorCode result = trainerTrainPokemon(trainer, pokemon_id);
	if (result == TRAINER_POKEMON_DOESNT_EXIST) {
    return POKEMONGO_POKEMON_DOES_NOT_EXIST; 
  }
	if (result == TRAINER_NO_AVAILABLE_ITEM_FOUND) {
    return POKEMONGO_NO_AVAILABLE_ITEM_FOUND; 
  }

	return POKEMONGO_SUCCESS;
}

PokemonGoErrorCode pokemongoReportTrainer(PokemonGo pokemon_go,
                                          char* trainer_name) {
	if (NULL == pokemon_go || NULL == trainer_name) {
    return POKEMONGO_INVALID_ARGUMENT; 
  }

	Trainer trainer = mapGet(pokemon_go->trainers, trainer_name);
	if (NULL == trainer) return POKEMONGO_TRAINER_DOES_NOT_EXIST;

	printTrainer(trainer, pokemon_go->output_channel);
	return POKEMONGO_SUCCESS;
}

PokemonGoErrorCode pokemongoReportLocations(PokemonGo pokemon_go) {
	assert(pokemon_go);

	mtmPrintLocationsHeader(pokemon_go->output_channel);
	MAP_FOREACH(char*, location_name, pokemon_go->locations) { //TODO: need to check if print with lexicographic order
    Location location = mapGet(pokemon_go->locations, location_name);
		printLocation(location, pokemon_go->output_channel);
	}
	return POKEMONGO_SUCCESS;
}

PokemonGoErrorCode pokemongoReportStock(PokemonGo pokemon_go) {
	assert(pokemon_go);

	storePrintStock(pokemon_go->store, pokemon_go->output_channel);
	return POKEMONGO_SUCCESS;
}

