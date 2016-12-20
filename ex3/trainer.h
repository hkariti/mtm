#ifndef TRAINER_H_
#define TRAINER_H_

#include "list.h"
#include "pokemon.h"
#include "location.h"
#include "store.h"
#include "print_utils.h"

typedef struct Trainer_t *Trainer;

struct Trainer_t { //TODO: move to .c file
	char* name;
	Location current_location;
	List pokemons;
	Inventory potions;
	Inventory candies;
	int money;
	double xp;
	int total_pokemon_caught;
};

Trainer createTrainer(char* name, int budget, Location start_location);
void destroyTrainer(Trainer trainer);
void printTrainer(Trainer trainer);

Pokemon getTrainerPokemon(Trainer trainer, int pokemon_id); //do we really need it?

MtmErrorCode trainerHealPokemon(Trainer trainer, int pokemon_id);
MtmErrorCode trainerTrainPokemon(Trainer trainer, int pokemon_id);
MtmErrorCode trainersBattle(Trainer trainer_1, int pokemon_1_id, Trainer trainer_2, int pokemon_2_id);
MtmErrorCode tarinerGoToLocation(Trainer trainer, Location location);
MtmErrorCode trainerBuyItem(Trainer trainer, Store store, char* type, int value); // who should convert type string to enum?

char* getTrainerName(Trainer trainer);
double getTrainerXP(Trainer trainer);

#endif