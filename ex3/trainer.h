#ifndef TRAINER_H_
#define TRAINER_H_

#include "map.h"
#include "pokemon.h"
#include "location.h"
#include "store.h"
#include "print_utils.h"

typedef struct Trainer_t *Trainer;

typedef enum {
  TRAINER_SUCCESS,
  TRAINER_OUT_OF_MEMORY,
  TRAINER_INVALID_ARGUMENT,
  TRAINER_LOCATION_IS_NOT_REACHABLE,
  TRAINER_ALREADY_IN_LOCATION,
  TRAINER_BUDGET_IS_INSUFFICIENT,
  TRAINER_POKEMON_HP_IS_AT_MAX,
  TRAINER_NO_AVAILABLE_ITEM_FOUND,
  TRAINER_POKEMON_DOESNT_EXIST,
  TRAINER_STORE_ITEM_OUT_OF_STOCK,
} TrainerErrorCode;

Trainer createTrainer(char* name, int budget, Location start_location);
void destroyTrainer(Trainer trainer);
void printTrainer(Trainer trainer, FILE* output_channel);

// NULL is pokemon doesn't exist or null arg
Pokemon getTrainerPokemon(Trainer trainer, int pokemon_id);

// INVALID_ARGUMENT, POKEON_DOESNT_EXIST
TrainerErrorCode trainerRemovePokemon(Trainer trainer, int pokemon_id);
// INVALID_ARGUMENT, POKEMON_DOESNT_EXIST, POKEMON_HP_IS_AT_MAX, NO_AVILABLE_ITEM_FOUND
TrainerErrorCode trainerHealPokemon(Trainer trainer, int pokemon_id);
// INVALID_ARGUMENT, POKEMON_DOESNT_EXIST, NO_AVILABLE_ITEM_FOUND
TrainerErrorCode trainerTrainPokemon(Trainer trainer, int pokemon_id);
// -1 for invalid arg
double trainerCalculateBattleDelta(Trainer trainer, Pokemon pokemon);
// INVALID_ARGUMENT, POKEMON_DOESNT_EIST
TrainerErrorCode trainersBattle(Trainer trainer, int pokemon_id,
                                Trainer other_trainer, int other_pokemon_id);
// INVALID_ARGUMENT, ALREADY_IN_LOCATION,LOCATION_IS_NOT_REACHABLE
TrainerErrorCode tarinerGoToLocation(Trainer trainer, Location location);
// INVALID_ARGUMENT, OUT_OF_MEMORY
TrainerErrorCode trainerHunt(Trainer trainer, FILE* output_channel);
// INVALID_ARGUMENT, STORE_ITEM_OUT_OF_STOCK, BUDGET_IS_INSUFFICIENT, OUT_OF_MEMORY
TrainerErrorCode trainerBuyItem(Trainer trainer, Store store, char* type,
                                int value);

// null if null arg
char* getTrainerName(Trainer trainer);
// -1 is null arg
double getTrainerXP(Trainer trainer);

#endif
