#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "trainer.h"
#include "utils.h"


struct Trainer_t {
	char* name;
    Location current_location;
	Map pokemons;
	Inventory potions;
	Inventory candies;
	int money;
	double xp;
	int total_pokemon_caught;
};

/**
 * inventoryGetByType - get a trainer's Inventory by its type
 *
 * @param trainer   - an existing Trainer
 * @param type      - the item's type
 *
 * @return
 * NULL - the trainer doesn't have an inventory for the type requested 
 * An Inventory object will be returned on success
 */
static Inventory inventoryGetByType(Trainer trainer, char* type) {
  if (strcmp(type, "candy") == 0) return trainer->candies;
  if (strcmp(type, "potion") == 0) return trainer->potions;
  return NULL;
}

/**
 * veryifyMemoryAllocations - verify trainerCreate's memory allocs
 *
 * @param trainer - a Trainer object
 *
 * @return
 *  false - one of trainer's internal fields wasn't successfully allocated
 *  true  - everything was allocated successfully
 */
static bool verifyMemoryAllocations(Trainer trainer) {
  return trainer->name != NULL &&
    trainer->pokemons != NULL &&
    trainer->potions != NULL &&
    trainer->candies != NULL;
}

Trainer trainerCreate(char* name, int budget, Location start_location) {
  if (NULL == name || budget < 0 || NULL == start_location) return NULL;

  Trainer trainer = malloc(sizeof(*trainer));
  if (NULL == trainer) return NULL;
  trainer->name = stringCopy(name);
  trainer->current_location = start_location;
  trainer->pokemons = mapCreate((copyMapKeyElements)intCopy,
                                (copyMapDataElements)pokemonCopy,
                                (freeMapKeyElements)intDestroy,
                                (freeMapDataElements)pokemonDestroy,
                                (compareMapKeyElements)intCompare);
  trainer->potions = inventoryCreate();
  trainer->candies = inventoryCreate();
  trainer->money = budget;
  trainer->xp = TRAINER_INITIAL_XP;
  trainer->total_pokemon_caught = 0;
  if (!verifyMemoryAllocations(trainer)) {
    trainerDestroy(trainer);
    return NULL;
  }
  return trainer;
}

void trainerDestroy(Trainer trainer) {
  if (NULL == trainer) return;
  free(trainer->name);
  mapDestroy(trainer->pokemons);
  inventoryDestroy(trainer->potions);
  inventoryDestroy(trainer->candies);
  free(trainer);
}

Trainer trainerCopy(Trainer trainer) {
  if (NULL == trainer) return NULL;

  Trainer trainer_copy;
  trainer_copy = trainerCreate(trainer->name, trainer->money,
                               trainer->current_location);
  if (NULL == trainer_copy) return NULL;
  // Destroy some empty fields, to be replaced with copies
  mapDestroy(trainer_copy->pokemons);
  inventoryDestroy(trainer_copy->candies);
  inventoryDestroy(trainer_copy->potions);
  // Copy internal fields
  trainer_copy->potions = inventoryCopy(trainer->potions);
  trainer_copy->candies = inventoryCopy(trainer->candies);
  trainer_copy->pokemons = mapCopy(trainer->pokemons);
  trainer_copy->xp = trainer->xp;
  trainer_copy->total_pokemon_caught = trainer->total_pokemon_caught;
  if (!verifyMemoryAllocations(trainer_copy)) {
    trainerDestroy(trainer_copy);
    return NULL;
  }
  return trainer_copy;
}

void trainerPrint(Trainer trainer, FILE* output_channel) {
  assert(trainer != NULL);
  assert(output_channel != NULL);
  char* location_name = locationGetName(trainer->current_location);
  mtmPrintTrainerHeader(output_channel, trainer->name, location_name,
                        trainer->money, trainer->xp);
  mtmPrintItemsHeaderForTrainer(output_channel);
  inventoryPrint(trainer->candies, "candy", output_channel);
  inventoryPrint(trainer->potions, "potion", output_channel);
  mtmPrintPokemonsHeaderForTrainer(output_channel);
  MAP_FOREACH(int*, id, trainer->pokemons) {
    Pokemon pokemon = mapGet(trainer->pokemons, id);
    pokemonPrint(pokemon, output_channel);
  }
}

Pokemon trainerGetPokemon(Trainer trainer, int pokemon_id) {
  if (NULL == trainer) return NULL;
  return mapGet(trainer->pokemons, &pokemon_id);
}

TrainerErrorCode trainerRemovePokemon(Trainer trainer, int pokemon_id) {
  if (NULL == trainer) return TRAINER_INVALID_ARGUMENT;
  
  MapResult remove_result = mapRemove(trainer->pokemons, &pokemon_id);
  if (MAP_ITEM_DOES_NOT_EXIST == remove_result) {
    return TRAINER_POKEMON_DOESNT_EXIST; 
  }
  return TRAINER_SUCCESS;
}

TrainerErrorCode trainerHealPokemon(Trainer trainer, int pokemon_id) {
  if (NULL == trainer) return TRAINER_INVALID_ARGUMENT;
  Pokemon pokemon = trainerGetPokemon(trainer, pokemon_id);
  if (NULL == pokemon) return TRAINER_POKEMON_DOESNT_EXIST;

  double hp = pokemonGetHP(pokemon);
  if (hp == MAX_POKEMON_HP) return TRAINER_POKEMON_HP_IS_AT_MAX;
  // Find the most efficient potion
  int chosen_potion = -1;
  INVENTORY_FOREACH(potion, trainer->potions) {
    double hp_after_potion = hp + *potion;
    // Potions are sorted, so every potion is better than the last
    chosen_potion = *potion;
    // We reached the max, no use wasting better potions
    if (hp_after_potion >= MAX_POKEMON_HP) {
      break;
    }
  }

  if (chosen_potion < 0) return TRAINER_NO_AVAILABLE_ITEM_FOUND;
  inventoryRemoveItem(trainer->potions, chosen_potion);
  pokemonGivePotion(pokemon, chosen_potion);

  return TRAINER_SUCCESS;
}

TrainerErrorCode trainerTrainPokemon(Trainer trainer, int pokemon_id) {
  if (NULL == trainer) return TRAINER_INVALID_ARGUMENT;
  Pokemon pokemon = trainerGetPokemon(trainer, pokemon_id);
  if (NULL == pokemon) return TRAINER_POKEMON_DOESNT_EXIST;

  int chosen_candy = -1;
  INVENTORY_FOREACH(candy, trainer->candies) {
    // Get to the last (highest) candy
    chosen_candy = *candy;
  }

  if (chosen_candy < 0) return TRAINER_NO_AVAILABLE_ITEM_FOUND;
  inventoryRemoveItem(trainer->candies, chosen_candy);
  pokemonGiveCandy(pokemon, chosen_candy);

  return TRAINER_SUCCESS;
}

/**
 * trainerCalculateBattleDelta - calculate one side's delta in a battle
 *
 * @param trainer - An existing Trainer that is a side in the battle
 * @param pokemon   - The trainer's pokemon that is participating in the battle
 *
 * @return
 *  The side's delta - trainer->xp * trainer->cp + (1 + pokemon->hp/200)
 */
static double trainerCalculateBattleDelta(Trainer trainer, Pokemon pokemon) {
  if (NULL == trainer || NULL == pokemon) return -1;

  double cp = pokemonGetCP(pokemon);
  double hp = pokemonGetHP(pokemon);
  assert(cp != -1);
  assert(hp != -1);

  double delta = trainer->xp * cp * (1 + hp/200);
  return delta;
}

TrainerErrorCode trainersBattle(Trainer trainer_1, int pokemon1_id,
                                Trainer trainer_2, int pokemon2_id) {
  if (NULL == trainer_1 || NULL == trainer_2) return TRAINER_INVALID_ARGUMENT;

  Pokemon pokemon_1, pokemon_2;
  pokemon_1 = trainerGetPokemon(trainer_1, pokemon1_id);
  pokemon_2 = trainerGetPokemon(trainer_2, pokemon2_id);
  if (NULL == pokemon_1 || NULL == pokemon_2) {
    return TRAINER_POKEMON_DOESNT_EXIST;
  }

  double delta_2 = trainerCalculateBattleDelta(trainer_1, pokemon_1);
  double delta_1 = trainerCalculateBattleDelta(trainer_2, pokemon_2);

  pokemonBattle(pokemon_1, pokemon_2, delta_1, delta_2);

  trainer_1->xp += delta_2 / 10;
  trainer_2->xp += delta_1 / 10;

  return TRAINER_SUCCESS;
}

TrainerErrorCode trainerGoToLocation(Trainer trainer, Location location) {
  if (NULL == trainer || NULL == location) return TRAINER_INVALID_ARGUMENT;

  if (locationCompare(trainer->current_location, location) == 0) {
    return TRAINER_ALREADY_IN_LOCATION; 
  }

  if (!locationIsNeighour(trainer->current_location, location)) {
    return TRAINER_LOCATION_IS_NOT_REACHABLE;
  }
  trainer->current_location = location;

  return TRAINER_SUCCESS;
}

TrainerErrorCode trainerHunt(Trainer trainer, FILE* output_channel) {
  if (NULL == trainer || NULL == output_channel) {
      return TRAINER_INVALID_ARGUMENT;
  }
  char* location_name = locationGetName(trainer->current_location);
  char* species;
  Pokemon captured_pokemon;

  // Handle empty location
  if (locationIsEmpty(trainer->current_location)) {
    mtmPrintCatchResult(output_channel, trainer->name, NULL, location_name);
    return TRAINER_SUCCESS;
  }
  // Fetch the pokemon currently in the location
  captured_pokemon = locationPopPokemon(trainer->current_location);
  if (NULL == captured_pokemon) return TRAINER_OUT_OF_MEMORY;
  species = pokemonGetSpecies(captured_pokemon);
  // Set pokemon id, catch pokemon and collect coins
  int pokemon_id = ++trainer->total_pokemon_caught;
  trainer->money += pokemonCaught(captured_pokemon, pokemon_id);
  MapResult put_result = mapPut(trainer->pokemons, &pokemon_id,
                                captured_pokemon);
  free(captured_pokemon);
  assert(put_result != MAP_NULL_ARGUMENT);
  if (MAP_OUT_OF_MEMORY == put_result) return TRAINER_OUT_OF_MEMORY;
  // Print catch result
  mtmPrintCatchResult(output_channel, trainer->name, species, location_name);

  return TRAINER_SUCCESS;
}

TrainerErrorCode trainerBuyItem(Trainer trainer, Store store, char* type,
                                int value) {
  if (NULL == trainer || NULL == store || NULL == type) {
    return TRAINER_INVALID_ARGUMENT; 
  }
  int price;
  Inventory chosen_inventory = inventoryGetByType(trainer, type);
  StoreErrorCode store_result = storeGetItemPrice(store, type, value, &price);
  if (STORE_INVALID_ARGUMENT == store_result || NULL == chosen_inventory) {
    return TRAINER_INVALID_ARGUMENT; 
  }
  if (STORE_ITEM_OUT_OF_STOCK == store_result) {
    return TRAINER_STORE_ITEM_OUT_OF_STOCK; 
  }
  if (price > trainer->money) return TRAINER_BUDGET_IS_INSUFFICIENT;
  trainer->money -= price;
  store_result = storeBuyItem(store, type, value);
  if (STORE_OUT_OF_MEMORY == store_result) return TRAINER_OUT_OF_MEMORY;
  assert(STORE_SUCCESS == store_result);
  InventoryErrorCode add_result = inventoryAddItem(chosen_inventory, value);
  if (INVENTORY_OUT_OF_MEMORY == add_result) return TRAINER_OUT_OF_MEMORY;

  return TRAINER_SUCCESS;
}

char* trainerGetName(Trainer trainer) {
  if (NULL == trainer) return NULL;

  return trainer->name;
}

double trainerGetXP(Trainer trainer) {
  if (NULL == trainer) return -1;

  return trainer->xp;
}
