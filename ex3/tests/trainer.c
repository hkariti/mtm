#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <trainer.h>
#include <utils.h>


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

static Inventory getInventoryByType(Trainer trainer, char* type) {
  if (strcmp(type, "candy") == 0) return trainer->candies;
  if (strcmp(type, "potion") == 0) return trainer->potions;
  return NULL;
}

static bool verifyMemoryAllocations(Trainer trainer) {
  return trainer->name != NULL &&
    trainer->pokemons != NULL &&
    trainer->potions != NULL &&
    trainer->candies != NULL;
}

Trainer createTrainer(char* name, int budget, Location start_location) {
  if (NULL == name || budget < 0 || NULL == start_location) return NULL;

  Trainer trainer = malloc(sizeof(*trainer));
  if (NULL == trainer) return NULL;
  trainer->name = stringCopy(name);
  trainer->current_location = start_location;
  trainer->pokemons = mapCreate((copyMapKeyElements)intCopy,
                                (copyMapDataElements)copyPokemon,
                                (freeMapKeyElements)intDestroy,
                                (freeMapDataElements)destroyPokemon,
                                (compareMapKeyElements)intCompare);
  trainer->potions = createInventory();
  trainer->candies = createInventory();
  trainer->money = budget;
  trainer->xp = TRAINER_INITIAL_XP;
  trainer->total_pokemon_caught = 0;
  if (!verifyMemoryAllocations(trainer)) {
    destroyTrainer(trainer);
    return NULL;
  }
  return trainer;
}

void destroyTrainer(Trainer trainer) {
  if (NULL == trainer) return;
  free(trainer->name);
  mapDestroy(trainer->pokemons);
  destroyInventory(trainer->potions);
  destroyInventory(trainer->candies);
  free(trainer);
}

Trainer trainerCopy(Trainer trainer) {
  if (NULL == trainer) return NULL;

  Trainer trainer_copy;
  trainer_copy = createTrainer(trainer->name, trainer->money,
                               trainer->current_location);
  if (NULL == trainer_copy) return NULL;
  mapDestroy(trainer_copy->pokemons);
  destroyInventory(trainer_copy->candies);
  destroyInventory(trainer_copy->potions);
  trainer_copy->potions = copyInventory(trainer->potions);
  trainer_copy->candies = copyInventory(trainer->candies);
  trainer_copy->pokemons = mapCopy(trainer->pokemons);
  trainer_copy->xp = trainer->xp;
  trainer_copy->total_pokemon_caught = trainer->total_pokemon_caught;
  if (!verifyMemoryAllocations(trainer_copy)) {
    destroyTrainer(trainer_copy);
    return NULL;
  }
  return trainer_copy;
}

void printTrainer(Trainer trainer, FILE* output_channel) {
  assert(trainer != NULL);
  assert(output_channel != NULL);
  char* location_name = locationGetName(trainer->current_location);
  mtmPrintTrainerHeader(output_channel, trainer->name, location_name,
                        trainer->money, trainer->xp);
  mtmPrintItemsHeaderForTrainer(output_channel);
  printInventory(trainer->candies, "candy", output_channel); //TODO: Insert type into inventory
  printInventory(trainer->potions, "potion", output_channel);
  mtmPrintPokemonsHeaderForTrainer(output_channel);
  MAP_FOREACH(int*, id, trainer->pokemons) {
    Pokemon pokemon = mapGet(trainer->pokemons, id);
    printPokemon(pokemon, output_channel);
  }
}

Pokemon getTrainerPokemon(Trainer trainer, int pokemon_id) {
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
  Pokemon pokemon = getTrainerPokemon(trainer, pokemon_id);
  if (NULL == pokemon) return TRAINER_POKEMON_DOESNT_EXIST;

  int hp = pokemonGetHP(pokemon);
  if (hp == MAX_POKEMON_HP) return TRAINER_POKEMON_HP_IS_AT_MAX;
  // Find the most efficient potion
  int chosen_potion = -1;
  INVENTORY_FOREACH(potion, trainer->potions) {
    int hp_after_potion = hp + *potion;
    // Not enough to fill HP, no use looking further
    if (hp_after_potion < MAX_POKEMON_HP) {
      // If no better potion was seen before, it's the best we'll get
      if (chosen_potion < 0) {
        chosen_potion = *potion;
      };
      break;
    }
    // We're saturating the pokemon's HP. Remember this one and hope for a more
    // efficient potion next
    chosen_potion = *potion;
  }

  if (chosen_potion < 0) return TRAINER_NO_AVAILABLE_ITEM_FOUND;
  inventoryRemoveItem(trainer->potions, chosen_potion);
  pokemonGivePotion(pokemon, chosen_potion);

  return TRAINER_SUCCESS;
}

TrainerErrorCode trainerTrainPokemon(Trainer trainer, int pokemon_id) {
  if (NULL == trainer) return TRAINER_INVALID_ARGUMENT;
  Pokemon pokemon = getTrainerPokemon(trainer, pokemon_id);
  if (NULL == pokemon) return TRAINER_POKEMON_DOESNT_EXIST;

  int chosen_candy = -1;
  INVENTORY_FOREACH(candy, trainer->candies) { // TODO: yuck
    chosen_candy = *candy;
    break;
  }

  if (chosen_candy < 0) return TRAINER_NO_AVAILABLE_ITEM_FOUND;
  inventoryRemoveItem(trainer->candies, chosen_candy);
  pokemonGiveCandy(pokemon, chosen_candy);

  return TRAINER_SUCCESS;
}

double trainerCalculateBattleDelta(Trainer trainer, Pokemon pokemon) {
  if (NULL == trainer || NULL == pokemon) return -1;

  double cp = pokemonGetCP(pokemon);
  double hp = pokemonGetHP(pokemon);
  assert(cp != -1);
  assert(hp != -1);

  double delta = trainer->xp * cp * (1 + hp/200);
  return delta;
}

// TODO: Verify this implementation
TrainerErrorCode trainersBattle(Trainer trainer, int pokemon_id,
                                Trainer other_trainer, int other_pokemon_id) {
  if (NULL == trainer || NULL == other_trainer) return TRAINER_INVALID_ARGUMENT;
  Pokemon pokemon, other_pokemon;
  pokemon = getTrainerPokemon(trainer, pokemon_id);
  other_pokemon = getTrainerPokemon(other_trainer, other_pokemon_id);
  if (NULL == pokemon || NULL == other_pokemon) {
    return TRAINER_POKEMON_DOESNT_EXIST;
  }
  double my_delta = trainerCalculateBattleDelta(trainer, pokemon);
  double other_delta = trainerCalculateBattleDelta(other_trainer, other_pokemon);

  pokemonBattle(pokemon, other_pokemon, other_delta);
  trainer->xp += my_delta/10;

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
  if (NULL == trainer || NULL == output_channel) return TRAINER_INVALID_ARGUMENT;
  char* location_name = locationGetName(trainer->current_location);
  char* species;
  Pokemon captured_pokemon;

  if (locationIsEmpty(trainer->current_location)) {
    mtmPrintCatchResult(output_channel, trainer->name, NULL, location_name);
    return TRAINER_SUCCESS;
  }

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
  mtmPrintCatchResult(output_channel, trainer->name, species, location_name);

  return TRAINER_SUCCESS;
}

TrainerErrorCode trainerBuyItem(Trainer trainer, Store store, char* type,
                                int value) {
  if (NULL == trainer || NULL == store || NULL == type) {
    return TRAINER_INVALID_ARGUMENT; 
  }
  int price;
  Inventory chosen_inventory = getInventoryByType(trainer, type);
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

char* getTrainerName(Trainer trainer) {
  if (NULL == trainer) return NULL;

  return trainer->name;
}

double getTrainerXP(Trainer trainer) {
  if (NULL == trainer) return -1;

  return trainer->xp;
}
