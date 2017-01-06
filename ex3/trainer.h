#ifndef TRAINER_H_
#define TRAINER_H_

#include "map.h"
#include "pokemon.h"
#include "location.h"
#include "store.h"
#include "print_utils.h"

/**
 * PokemonGo Trainer
 *
 * Implements the Trainer object in the PokemonGo game. A trainer is the
 * playable entity in the game. They have the following attributes:
 *   - A current location, one of the locations in the game's location map.
 *     A trainer can also move to other locations.
 *   - Money. Used to buy items from the store.
 *   - Items. Bought from the store, can be used on pokemon. Currently supported
 *     items are: potions, candies
 *   - A list of captured pokemon. Each pokemon has a unique id. Pokemon
 *     are captured when the trainer moves to a location, if the location
 *     is not empty. Capturing a pokemon gives money to the trainer.
 *   - Experience points. Can be increased by battling other trainers.
 *
 * The following functions are availble:
 * trainerCreate        - Create a new trainer with no pokemon.
 * trainerDestroy       - Destroy a trainer, freeing all of its resources.
 * trainerCopy          - Create a copy of an existing trainer object.
 * trainerPrint         - Print a report on the trainer's attributes, pokemon
 *                        and items.
 * trainerGetPokemon    - Get a trainer's pokemon by its unique id.
 * trainerRemovePokemon - Remove a pokemon from the trainer's party.
 * trainerHealPokemon   - Use a potion on a pokemon to increase its HP.
 * trainerTrainPokemon  - Use a candy on a pokemon to increase its CP.
 * trainersBattle       - Battle two trainers against one another.
 * trainerGoToLocation  - Change a trainer's location, if it's reachable from
 *                        its current location.
 * trainerHunt          - Capture the pokemon in the current location
 * trainerBuyItem       - Buy an item from the store.
 * trainerGetName       - Get the trainer's name
 * trainerGetXP         - Get the trainer's experience points.
 */

// Type definition
typedef struct Trainer_t *Trainer;

// Constants
#define TRAINER_INITIAL_XP 1

// Possible errors
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

/**
 * trainerCreate - Create a new trainer, without any pokemon
 *
 * @param name          - The trainer's name
 * @param budget        - The trainer's initial amount of money
 * @param start_location- The trainer's starting location
 *
 * @return
 *  NULL - NULL arg or allocation error
 *  A new Trainer object in case of success
 */
Trainer trainerCreate(char* name, int budget, Location start_location);

/**
 * trainerDestroy - Deallocate a trainer and all its resources
 *
 * @param trainer   - an existing Trainer.
 *
 * If trainer is NULL nothing will be done.
 */
void trainerDestroy(Trainer trainer);

/**
 * trainerCopy - Copy an existing trainer and all its resources
 *
 * @param trainer   - an existing Trainer
 *
 * @return
 *  NULL    - NULL arg or memory error
 *  A Trainer object which is a copy of the given trainer in case of success
 */
Trainer trainerCopy(Trainer trainer);

/**
 * trainerPrint - Print a report about a trainer's attribute, item and pokemon
 *
 * @param trainer - an existing Trainer
 */
void trainerPrint(Trainer trainer, FILE* output_channel);

/**
 * trainerGetPokemon - Get a pokemon from the trainer's party
 *
 * @param trainer   - an existing Trainer
 * @param pokemon_id- The unique id of the pokemon in the trainer's party
 *
 * @return
 *  NULL - NULL arg or invalid id
 *  A pokemon object of the requested pokemon on success. Changes to this object
 *  will affect the actual pokemon in the trainer's party.
 */
Pokemon trainerGetPokemon(Trainer trainer, int pokemon_id);

/**
 * trainerRemovePokemon - Remove a pokemon from the trainer's party
 *
 * @param trainer       - an existing Trainer
 * @param pokemon_id    - The unique id of the pokemon in the trainer's party.
 *
 * @return
 *  TRAINER_INVALID_ARGUMENT        - NULL arg
 *  TRAINER_POKEMON_DOESNT_EXIST    - Invalid id.
 *  TRAINER_SUCCESS                 - Pokemon was removed
 */
TrainerErrorCode trainerRemovePokemon(Trainer trainer, int pokemon_id);

/**
 * trainerHealPokemon - Use a potion on a pokemon to increase its HP
 *
 * @param trainer       - an existing Trainer
 * @param pokemon_id    - The unique id of the pokemon in the trainer's party.
 *
 * The potion that will be selected is the one that will increase the
 * pokemon's HP to the highest value, AND will go the minimum amount past
 * the pokemon's max HP (i.e. waste as little as possible).
 *
 * @return
 *  TRAINER_INVALID_ARGUMENT        - NULL arg
 *  TRAINER_POKEMON_DOESNT_EXIST    - Invalid id
 *  TRAINER_POKEMON_HP_IS_AT_MAX    - Requested pokemon is already at max hp
 *  TRAINER_NO_AVAILABLE_ITEM_FOUND - The trainer doesn't have any potions
 *  TRIANER_SUCCESS                 - Potion was given successfully
 */
TrainerErrorCode trainerHealPokemon(Trainer trainer, int pokemon_id);

/**
 * trainerTrainPokemon - Use a candy on a pokemon to increase its CP
 *
 * @param trainer       - an existing Trainer
 * @param pokemon_id    - The unique id of the pokemon in the trainer's party.
 *
 * The potion that will be selected is the one with the higest value.
 *
 * @return
 *  TRAINER_INVALID_ARGUMENT        - NULL arg
 *  TRAINER_POKEMON_DOESNT_EXIST    - Invalid id
 *  TRAINER_NO_AVAILABLE_ITEM_FOUND - The trainer doesn't have any candies
 *  TRIANER_SUCCESS                 - Candy was given successfully
 */
TrainerErrorCode trainerTrainPokemon(Trainer trainer, int pokemon_id);

/**
 * trainersBattle - Battle between two trainers
 *
 * @param trainer_1     - an existing Trainer, one side of the battle
 * @param pokemon1_id   - The unique id of a pokemon from trainer_1's party
 * @param trainer_2     - an existing Trainer, other side of the battle
 * @param pokemon2_id   - The unique id of a pokemon frmo trainer_2's party
 *
 * The battle will affect each trainer's XP. See implementation for the formula.
 *
 * For affects on the pokemon, see pokemonBattle in pokemon.c.
 *
 * @return
 *  TRAINER_INVALID_ARGUMENT        - NULL arg
 *  TRAINER_POKEMON_DOESNT_EXIST    - Invalid id for one of the pokemon
 *  TRAINER_SUCCESS                 - The battle is done.
 */
TrainerErrorCode trainersBattle(Trainer trainer_1, int pokemon1_id,
                                Trainer trainer_2, int pokemon2_id);

// INVALID_ARGUMENT, ALREADY_IN_LOCATION,LOCATION_IS_NOT_REACHABLE
/**
 * trainerGoToLocation  - Change a trainer's location
 *
 * @param trainer   - an existing Trainer
 * @param location  - The new location to move the trainer to
 *
 * The move will only succeed of the new location is a neighbor of the current
 * location.
 *
 * @return
 *  TRAINER_INVALID_ARGUMENT             - NULL arg
 *  TRAINER_ALREADY_IN_LOCATION         - The current location was passed as
 *                                        argument
 *  TRAINER_LOCATION_IS_NOT_REACHABLE   - The given location isn't a neighbor
 *                                        of the current location
 *  TRAINER_SUCCESS                     - The trainer has moved successfully.
 */
TrainerErrorCode trainerGoToLocation(Trainer trainer, Location location);

// INVALID_ARGUMENT, OUT_OF_MEMORY
/**
 * trainerHunt - Capture the pokemon in the current location
 *
 * @param trainer           - an existing Trainer
 * @param output_channel    - An open file to write the capture result to
 *
 * @return
 *  TRAINER_INVALID_ARGUMENT    - NULL arg
 *  TRAINER_OUT_OF_MEMORY       - memory error
 *  TRAINER_SUCCESS             - A pokemon was captured, or location is empty.
 */
TrainerErrorCode trainerHunt(Trainer trainer, FILE* output_channel);

// INVALID_ARGUMENT, STORE_ITEM_OUT_OF_STOCK, BUDGET_IS_INSUFFICIENT, OUT_OF_MEMORY
/**
 * trainerBuyItem - Buy an item from the store
 *
 * @param trainer   - an existing Trainer
 * @param store     - The store to buy from.
 * @param type      - The item type to buy. Must be supported by both trainer
 *                    and the store.
 * @param value     - The item value to buy.
 *
 * @return
 *  TRAINER_INVALID_ARGUMENT        - NULL arg or type isn't supported by
 *                                    trainer or store.
 *  TRAINER_STORE_ITEM_OUT_OF_STOCK - The store doesn't have the item requested.
 *  TRAINER_BUDGET_IS_INSUFFICIENT  - The trainer doesn't have enough money.
 *  TRAINER_OUT_OF_MEMORY           - memory error
 *  TRAINER_SUCCESS                 - Item was bought.
 */
TrainerErrorCode trainerBuyItem(Trainer trainer, Store store, char* type,
                                int value);

/**
 * trainerGetName - Get the trainer's name
 *
 * @param trainer   - an existing Trainer
 *
 * @return
 *  NULL - NULL arg
 *  The trainer's name on success.
 */
char* trainerGetName(Trainer trainer);

/**
 * trainerGetXP - Get the trainer's experience points
 *
 * @param trainer   - an existing Trainer
 *
 * @return
 *  NULL - NULL arg
 *  The trainer's experience points on success.
 */
double trainerGetXP(Trainer trainer);

#endif
