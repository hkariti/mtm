#ifndef TRAINER_H_
#define TRAINER_H_

#include "map.h"
#include "pokemon.h"
#include "location.h"
#include "store.h"
#include "print_utils.h"

#define TRAINER_INITIAL_XP 1

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

Trainer trainerCreate(char* name, int budget, Location start_location);
void trainerDestroy(Trainer trainer);
Trainer trainerCopy(Trainer trainer);
void trainerPrint(Trainer trainer, FILE* output_channel);

// NULL is pokemon doesn't exist or null arg
Pokemon trainerGetPokemon(Trainer trainer, int pokemon_id);

// INVALID_ARGUMENT, POKEON_DOESNT_EXIST
TrainerErrorCode trainerRemovePokemon(Trainer trainer, int pokemon_id);
// INVALID_ARGUMENT, POKEMON_DOESNT_EXIST, POKEMON_HP_IS_AT_MAX, NO_AVILABLE_ITEM_FOUND
TrainerErrorCode trainerHealPokemon(Trainer trainer, int pokemon_id);
// INVALID_ARGUMENT, POKEMON_DOESNT_EXIST, NO_AVILABLE_ITEM_FOUND
TrainerErrorCode trainerTrainPokemon(Trainer trainer, int pokemon_id);
// INVALID_ARGUMENT, POKEMON_DOESNT_EIST
TrainerErrorCode trainersBattle(Trainer trainer_1, int pokemon1_id,
                                Trainer trainer_2, int pokemon2_id);
// INVALID_ARGUMENT, ALREADY_IN_LOCATION,LOCATION_IS_NOT_REACHABLE
TrainerErrorCode trainerGoToLocation(Trainer trainer, Location location);
// INVALID_ARGUMENT, OUT_OF_MEMORY
TrainerErrorCode trainerHunt(Trainer trainer, FILE* output_channel);
// INVALID_ARGUMENT, STORE_ITEM_OUT_OF_STOCK, BUDGET_IS_INSUFFICIENT, OUT_OF_MEMORY
TrainerErrorCode trainerBuyItem(Trainer trainer, Store store, char* type,
                                int value);

// null if null arg
char* trainerGetName(Trainer trainer);
// -1 is null arg
double trainerGetXP(Trainer trainer);

#endif
