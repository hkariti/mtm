#ifndef POKEMON_GO_H_
#define POKEMON_GO_H_

#include "location.h"
#include "trainer.h"
#include "print_utils.h"
#include "pokedex.h"
#include "evolutions.h"

/**
* PokemonGo Game Container
*
* Implements a Container for a PokemonGo WorldWide Best Seller Game
*
* The info is stored in a PokemonGo structure, which stores the following info:
*  - Evolutions	- Map of all game's Evolutions
*  - Pokedex	- Pokedex with all information about in-game pokemons
*  - Locations	- Map of all the in-game locations
*  - Trainers   - Map of all the in-game trainers
*  - Store		- The one and only Store monopol 
*					with all great & expensive items
*  - output_channel - The output channel to print everything except errors to
*
* The following functions are available:
*  pokemongoCreate		 - Creates a game
*  pokemongoDestroy		 - Destroy the game
*  pokemongoTrainerAdd   - Add a trainer to the game
*  pokemongoTrainerGo    - Move the trainer to a new unexplorerd location 
*  pokemongoStoreAdd     - Add a new items to the store
*  pokemongoTrainerPurchase - Order the trainer to buy a new item
*  pokemongoBattleFight  - Perform a battle between 2 crazy pokemons
*  pokemongoPokemonHeal  - Order the trainer to heal his pokemon
*  pokemongoPokemonTrain - Order the trainer to train his pokemon
*  pokemongoReportTrainer- Report the specified trainer info
*  pokemongoReportLocations- Report all the game's trainers
*  pokemongoReportStock  - Report all the game's store stock
*/



/** Type used for returning error codes from PokemonGo functions */
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

/** Type for defining the PokemonGo */
typedef struct PokemonGo_t *PokemonGo;

/**
* pokemonGoCreate: Allocates and initialize new PokemonGo world
					containing locations, one store, and no trainers.
* @param pokedex - Pokedex containing info for all the pokemons in the game
* @param evolutions - evolutions map containing info for all evolutions in game
* @param locations - map containing all locations to add to game
* @return
* 	NULL - if one of the parameters is NULL or allocations failed.
* 	A new PokemonGo in case of success.
*/
PokemonGo pokemongoCreate(Pokedex pokedex, Evolutions evolutions,
		Map locations, FILE* output_channel);

/**
* pokemongoDestroy: Deallocates an existing pokemonGo. Clears all elements
*				  by using the stored free & destroy functions.
* @param pokemon_go - Target PokemonGo to be deallocated.
*					If pokemon_go is NULL nothing will be done
*/
void pokemongoDestroy(PokemonGo pokemon_go);

/**
* pokemongoTrainerAdd: Add a new trainer to the specified location in world
*		if pokemons exist in that location the trainer will try to catch it
* @param pokemon_go - The target PokemonGo game
* @param trainee_name - The name of the new trainer to add
* @param budget - The budget of pokecoins the new trainer will have
* @param start_location - The name of the location the trainer will start in
* @return
* 	POKEMONGO_INVALID_ARGUMENT - if one of the parameters is NULL 
*			or budget is negative.
*	POKEMONGO_LOCATION_DOES_NOT_EXIST - the specified location doesn't exist
* 	POKEMONGO_OUT_OF_MEMORY if allocations fails
*	POKEMONGO_SUCCESS trainer was added successfully
*/
PokemonGoErrorCode pokemongoTrainerAdd(PokemonGo pokemon_go,
	char* trainer_name, int budget, char* start_location);


/**
* pokemongoTrainerGo: order the trainer to move to the specified location
*		and if pokemons exist there the trainer will try to catch it
* @param pokemon_go - The target PokemonGo game
* @param trainee_name - The name of the new trainer to move
* @param new_location - The name of the location the trainer will move to
* @return
* 	POKEMONGO_INVALID_ARGUMENT - if one of the parameters is NULL
*	POKEMONGO_TRAINER_DOES_NOT_EXIST - the specified trainer doesn't exist
*	POKEMONGO_LOCATION_DOES_NOT_EXIST - the specified location doesn't exist
*	POKEMONGO_TRAINER_ALREADY_IN_LOCATION - trainer is already in that location
*	POKEMONGO_LOCATION_NOT_REACHABLE - the specified location is not nearby
* 	POKEMONGO_OUT_OF_MEMORY if allocations fails
*	POKEMONGO_SUCCESS trainer moved successfully
*/
PokemonGoErrorCode pokemongoTrainerGo(PokemonGo pokemon_go,
	char* trainer_name, char* new_location);

/**
* pokemonStoreAdd: Add new items to the store
* @param pokemon_go - The target PokemonGo game
* @param item_type - The type of the items to add (potion or candy)
* @param value - The value of the items to add
* @param quantity - The quantity of the items to add
* @return
* 	POKEMONGO_INVALID_ARGUMENT - if one of the parameters is NULL,
*			item_type is not a valid type, value or quantity are not positive
* 	POKEMONGO_OUT_OF_MEMORY if allocations fails
*	POKEMONGO_SUCCESS item added successfully
*/
PokemonGoErrorCode pokemongoStoreAdd(PokemonGo pokemon_go,
	char* item_type, int value, int quantity);

/**
* pokemongoTrainerPurchase: order the trainer to purchase a specified item
* @param pokemon_go - The target PokemonGo game
* @param trainee_name - The name of the trainer that will purchase
* @param item_type - The type of the item to be purchased
* @param value - The value of the item to be purchased
* @return
* 	POKEMONGO_INVALID_ARGUMENT - if one of the parameters is NULL,
*			item_type is not a valid type, value or quantity are not positive
*	POKEMONGO_TRAINER_DOES_NOT_EXIST - the specified trainer doesn't exist
*	POKEMONGO_BUDGET_IS_NOT_SUFFICIENT - the trainer's budget is not enough
*	POKEMONGO_ITEM_OUT_OF_STOCK - the requested item is not available
* 	POKEMONGO_OUT_OF_MEMORY if allocations fails
*	POKEMONGO_SUCCESS item purchased successfully
*/
PokemonGoErrorCode pokemongoTrainerPurchase(PokemonGo pokemon_go,
	char* trainer_name, char* item_type, int value);


/**
* pokemongoBattleFight: Perform a battle between two trainer's pokemons
* @param pokemon_go - The target PokemonGo game
* @param trainer1_name - The name of the first trainer
* @param trainer2_name - The name of the second trainer
* @param pokemon1_id - The first pokemon trainer's pokemon id 
* @param pokemon2_id - The second pokemon trainer's pokemon id
* @return
* 	POKEMONGO_INVALID_ARGUMENT - if one of the parameters is NULL
		or if the same trainer is mentioned
*	POKEMONGO_TRAINER_DOES_NOT_EXIST - the one of the trainers doesn't exist
*	POKEMONGO_POKEMON_DOES_NOT_EXIST - the one of the pokemons doesn't exist
*	POKEMONGO_SUCCESS battle performed successfully
*/
PokemonGoErrorCode pokemongoBattleFight(PokemonGo pokemon_go,
   char* trainer1_name, char* trainer2_name, int pokemon1_id, int pokemon2_id);


/**
* pokemongoPokemonHeal: Order the trainer to heal the mentioned pokemomn
* @param pokemon_go - The target PokemonGo game
* @param trainer_name - The name of the trainer which will heal
* @param pokemon_id - The pokemon's trainer id to heal
* @return
* 	POKEMONGO_INVALID_ARGUMENT - if one of the parameters is NULL
*	POKEMONGO_TRAINER_DOES_NOT_EXIST - The trainer doesn't exist
*	POKEMONGO_POKEMON_DOES_NOT_EXIST - The trainer doesn't exist
*	POKEMONGO_NO_AVAILABLE_ITEM_FOUND - Trainer doesn't have potions
*	POKEMONGO_SUCCESS heal performed successfully
*/
PokemonGoErrorCode pokemongoPokemonHeal(PokemonGo pokemon_go,
	char* trainer_name, int pokemon_id);


/**
* pokemongoPokemonTrain: Order the trainer to train the mentioned pokemon
						 with a candy
* @param pokemon_go - The target PokemonGo game
* @param trainer_name - The name of the trainer which will train
* @param pokemon_id - The pokemon's trainer id to train
* @return
* 	POKEMONGO_INVALID_ARGUMENT - if one of the parameters is NULL
*	POKEMONGO_TRAINER_DOES_NOT_EXIST - The trainer doesn't exist
*	POKEMONGO_POKEMON_DOES_NOT_EXIST - The trainer doesn't exist
*	POKEMONGO_NO_AVAILABLE_ITEM_FOUND - Trainer doesn't have candies
*	POKEMONGO_SUCCESS training performed successfully
*/
PokemonGoErrorCode pokemongoPokemonTrain(PokemonGo pokemon_go,
	char* trainer_name, int pokemon_id);


/**
* pokemongoReportTrainer: Print the specified trainer's information
	The information includes trainer's items and pokemons
* @param pokemon_go - The target PokemonGo game
* @param trainer_name - The name of the trainer which will be reported
* @return
*	POKEMONGO_TRAINER_DOES_NOT_EXIST - The trainer doesn't exist
*	POKEMONGO_SUCCESS printing performed successfully
*/
PokemonGoErrorCode pokemongoReportTrainer(PokemonGo pokemon_go, char* trainer_name);


/**
* pokemongoReportTrainer: Print all world's wildest locations
	The printing will be at leicographical order
* @param pokemon_go - The target PokemonGo game
* @return
*	POKEMONGO_SUCCESS printing performed successfully
*/
PokemonGoErrorCode pokemongoReportLocations(PokemonGo pokemon_go);


/**
* pokemongoReportStock: Print all store's precious items
		The printing will be at raising up value order
* @param pokemon_go - The target PokemonGo game
* @return
*	POKEMONGO_SUCCESS printing performed successfully
*/
PokemonGoErrorCode pokemongoReportStock(PokemonGo pokemon_go);

#endif
