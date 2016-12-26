#ifndef POKEMON_GO_H_
#define POKEMON_GO_H_

#include "location.h"
#include "trainer.h"
#include "print_utils.h"
#include "pokedex.h"
#include "evolutions.h"

typedef struct PokemonGo_t *PokemonGo;

struct PokemonGo_t { //TODO: move to .c file
	Evolutions evolutions;
	Pokedex pokedex;
	Map locations;
	Map trainers;
	Store store;
	FILE* output_channel;
};

PokemonGo createPokemonGo(Pokedex pokedex, Evolutions evolutions, Map locations, FILE* output_channel);
MtmErrorCode trainerAdd(PokemonGo pokemon_go, char* trainer_name, int budget, char* start_location);
MtmErrorCode trainerGo(char* trainer_name, char* new_location);
MtmErrorCode storeAdd(char* item_type, int value, int quantity);
MtmErrorCode trainerPurchase(char* trainer_name, char* item_type, int value);
MtmErrorCode battleFight(char* trainer1_name, char* trainer2_name, int pokemon1_id, int pokemon2_id);
MtmErrorCode pokemonHeal(char* trainer_name, int pokemon_id);
MtmErrorCode pokemonTrain(char* trainer_name, int pokemon_id);
MtmErrorCode reportTrainer(char* trainer_name);
MtmErrorCode reportLocations();
MtmErrorCode reportStock();

#endif