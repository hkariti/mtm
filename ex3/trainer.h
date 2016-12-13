#ifndef TRAINER_H_
#define TRAINER_H_

#include "list.h"
#include "item.h"
#include "pokemon.h"
#include "location.h"
#include "print_utils.h"

typedef struct Trainer_t *Trainer;

struct Trainer_t { //TODO: move to .c file
	char* name;
	Location current_location;
	List pokemons;
	List items;
	int money;
	double xp;
};

Trainer createTrainer(char* name, int budget, Location start_location);
void destroyTrainer(Trainer trainer);
void printTrainer(Trainer trainer);

Pokemon getTrainerPokemon(Trainer trainer, int pokemon_id); //do we really need it?

MtmErrorCode trainerHealPokemon(Trainer trainer, int pokemon_id);
MtmErrorCode trainerTrainPokemon(Trainer trainer, int pokemon_id);
MtmErrorCode trainersBattle(Trainer trainer_1, int pokemon_1_id, Trainer trainer_2, int pokemon_2_id);
MtmErrorCode toToLocation(Trainer trainer, char* location_name);
MtmErrorCode trainerBuyItem(Trainer trainer, ItemType type, int value); // who should convert type string to enum?

char* getTrainerName(Trainer trainer);
double getTrainerXP(Trainer trainer);

#endif