#ifndef POKEMON_GO_H_
#define POKEMON_GO_H_

#include "location.h"
#include "trainer.h"
#include "print_utils.h"
#include "pokedex.h"
#include "evolutions.h"

typedef enum {
	POKEMONGO_SUCCESS,
	POKEMONGO_OUT_OF_MEMORY,
	POKEMONGO_INVALID_ARGUMENT,
	POKEMONGO_TRAINER_NAME_ALREADY_EXISTS,
	POKEMONGO_TRAINER_DOES_NOT_EXIST,
	POKEMONGO_LOCATION_DOES_NOT_EXIST,
	POKEMONGO_POKEMON_DOES_NOT_EXIST,
	POKEMONGO_ITEM_OUT_OF_STOCK,
	POKEMONGO_BUDGET_IS_NOT_SUFFICIENT,
	POKEMONGO_HP_IS_AT_MAX,
	POKEMONGO_NO_AVAILABLE_ITEM_FOUND,
	POKEMONGO_LOCATION_IS_NOT_REACHABLE,
	POKEMONGO_TRAINER_ALREADY_IN_LOCATION
} PokemonGoErrorCode;

typedef struct PokemonGo_t *PokemonGo;

PokemonGo createPokemonGo(Pokedex pokedex, Evolutions evolutions, Map locations, FILE* output_channel);
void pokemongoDestroy(PokemonGo pokemon_go);

PokemonGoErrorCode pokemongoTrainerAdd(PokemonGo pokemon_go, char* trainer_name, int budget, char* start_location);
PokemonGoErrorCode pokemongoTrainerGo(PokemonGo pokemon_go, char* trainer_name, char* new_location);
PokemonGoErrorCode pokemongoStoreAdd(PokemonGo pokemon_go, char* item_type, int value, int quantity);
PokemonGoErrorCode pokemongoTrainerPurchase(PokemonGo pokemon_go, char* trainer_name, char* item_type, int value);
PokemonGoErrorCode pokemongoBattleFight(PokemonGo pokemon_go, char* trainer1_name, char* trainer2_name, int pokemon1_id, int pokemon2_id);
PokemonGoErrorCode pokemongoPokemonHeal(PokemonGo pokemon_go, char* trainer_name, int pokemon_id);
PokemonGoErrorCode pokemongoPokemonTrain(PokemonGo pokemon_go, char* trainer_name, int pokemon_id);
PokemonGoErrorCode pokemongoReportTrainer(PokemonGo pokemon_go, char* trainer_name);
PokemonGoErrorCode pokemongoReportLocations(PokemonGo pokemon_go);
PokemonGoErrorCode pokemongoReportStock(PokemonGo pokemon_go);

#endif