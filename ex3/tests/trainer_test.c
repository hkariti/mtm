#include "../trainer.h"
#include "../utils.h"
#include "mocks/trainer_mocks.h"
#include "test_utilities.h"

void pokemonInjure(Pokemon pokemon) {
  pokemonBattle(pokemon, pokemon, 50, 50);
}

bool testCreateTrainer() {
  TRAINER_SET_UP();
  Trainer trainer;
  Location location = mapGet(locations, "empty_location");

  // Invalid inputs
  trainer = trainerCreate(NULL, 1, location);
  ASSERT_TEST(NULL == trainer);
  trainer = trainerCreate("tembel", -1, location);
  ASSERT_TEST(NULL == trainer);
  trainer = trainerCreate("tembel", 1, NULL);
  ASSERT_TEST(NULL == trainer);

  // Valid inputs
  trainer = trainerCreate("tembel", 12, location);
  ASSERT_TEST(trainer != NULL);

  trainerDestroy(trainer);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testDestroyTrainer() {
  TRAINER_SET_UP();

  // NULL Input
  trainerDestroy(NULL);

  // Valid input
  Trainer trainer = demoTrainerEmpty("trainer", locations);
  trainerDestroy(trainer);

  TRAINER_TEAR_DOWN();
  return true;
}

bool testCopyTrainer() {
  TRAINER_SET_UP();

  Trainer trainer = demoTrainerEmpty("trainer", locations);
  Trainer trainer_with_pokemon = demoTrainerWithPokemon("trainer", locations);
  Trainer trainer_copy;

  // NULL input
  trainer_copy = trainerCopy(NULL);
  ASSERT_TEST(NULL == trainer_copy);

  // Valid input
  trainer_copy = trainerCopy(trainer);
  ASSERT_TEST(trainer_copy != NULL);
  trainerDestroy(trainer_copy);

  trainer_copy = trainerCopy(trainer_with_pokemon);
  ASSERT_TEST(trainer_copy != NULL);
  trainerDestroy(trainer_copy);

  trainerDestroy(trainer);
  trainerDestroy(trainer_with_pokemon);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testPrintTrainer() {
  TRAINER_SET_UP();

  Trainer trainer = demoTrainerEmpty("trainer", locations);
  FILE* dummy_file = (FILE*)0x1234;
  // NULL input
  trainerPrint(NULL, dummy_file);
  trainerPrint(trainer, NULL);

  trainerDestroy(trainer);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testGetTrainerPokemon() {
  TRAINER_SET_UP();

  Trainer trainer = demoTrainerEmpty("trainer", locations);
  Trainer trainer_with_pokemon = demoTrainerWithPokemon("trainer", locations);
  Pokemon pokemon;

  // Invalid input
  pokemon = trainerGetPokemon(NULL, 1);
  ASSERT_TEST(NULL == pokemon);
  pokemon = trainerGetPokemon(trainer, -1);
  ASSERT_TEST(NULL == pokemon);
  pokemon = trainerGetPokemon(trainer, 1);
  ASSERT_TEST(NULL == pokemon);
  pokemon = trainerGetPokemon(trainer, 0);
  ASSERT_TEST(NULL == pokemon);
  pokemon = trainerGetPokemon(trainer_with_pokemon, 135);
  ASSERT_TEST(NULL == pokemon);

  // Valid input
  pokemon = trainerGetPokemon(trainer_with_pokemon, 1);
  ASSERT_TEST(pokemon != NULL);

  trainerDestroy(trainer);
  trainerDestroy(trainer_with_pokemon);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testTrainerRemovePokemon() {
  TRAINER_SET_UP();

  Trainer trainer = demoTrainerEmpty("trainer", locations);
  Trainer trainer_with_pokemon = demoTrainerWithPokemon("trainer", locations);
  TrainerErrorCode remove_result;

  // Invalid input
  remove_result = trainerRemovePokemon(NULL, 1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == remove_result);

  // Pokemon doesn't exist
  remove_result = trainerRemovePokemon(trainer, 1);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == remove_result);
  remove_result = trainerRemovePokemon(trainer_with_pokemon, 13);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == remove_result);

  // Valid input
  remove_result = trainerRemovePokemon(trainer_with_pokemon, 1);
  ASSERT_TEST(TRAINER_SUCCESS == remove_result);
  remove_result = trainerRemovePokemon(trainer_with_pokemon, 2);
  ASSERT_TEST(TRAINER_SUCCESS == remove_result);

  // Can't remove again
  remove_result = trainerRemovePokemon(trainer_with_pokemon, 1);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == remove_result);

  trainerDestroy(trainer);
  trainerDestroy(trainer_with_pokemon);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testTrainerHealPokemon() {
  TRAINER_SET_UP();

  TrainerErrorCode heal_result;
  Trainer empty_trainer = demoTrainerEmpty("trainer", locations);
  Trainer trainer_with_items = demoTrainerWithItems("trainer", locations);
  Trainer trainer_with_pokemon = demoTrainerWithPokemon("trainer", locations);
  Trainer full_trainer = demoTrainerFull("trainer", locations);

  // Invalid input
  heal_result = trainerHealPokemon(NULL, 1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == heal_result);

  // Pokemon doesn't exist
  heal_result = trainerHealPokemon(empty_trainer, 1);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == heal_result);
  heal_result = trainerHealPokemon(trainer_with_items, 1);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == heal_result);
  heal_result = trainerHealPokemon(trainer_with_pokemon, 134);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == heal_result);
  heal_result = trainerHealPokemon(full_trainer, 134);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == heal_result);

  // Pokemon HP is at max
  // trainer_with_pokemon doesn't have any items
  heal_result = trainerHealPokemon(trainer_with_pokemon, 1);
  ASSERT_TEST(TRAINER_POKEMON_HP_IS_AT_MAX == heal_result);
  heal_result = trainerHealPokemon(full_trainer, 1);
  ASSERT_TEST(TRAINER_POKEMON_HP_IS_AT_MAX == heal_result);

  // Injure some pokemon to make potions work
  pokemonInjure(trainerGetPokemon(trainer_with_pokemon, 2));
  pokemonInjure(trainerGetPokemon(full_trainer, 2));
  pokemonInjure(trainerGetPokemon(full_trainer, 3));
  pokemonInjure(trainerGetPokemon(full_trainer, 4));

  // No potions
  heal_result = trainerHealPokemon(trainer_with_pokemon, 2);
  ASSERT_TEST(TRAINER_NO_AVAILABLE_ITEM_FOUND == heal_result);

  // Valid input
  // full_trainer has 2 potions
  heal_result = trainerHealPokemon(full_trainer, 2);
  ASSERT_TEST(TRAINER_SUCCESS == heal_result);
  heal_result = trainerHealPokemon(full_trainer, 3);
  ASSERT_TEST(TRAINER_SUCCESS == heal_result);

  // No potions left now
  heal_result = trainerHealPokemon(full_trainer, 4);
  ASSERT_TEST(TRAINER_NO_AVAILABLE_ITEM_FOUND == heal_result);

  trainerDestroy(empty_trainer);
  trainerDestroy(trainer_with_items);
  trainerDestroy(trainer_with_pokemon);
  trainerDestroy(full_trainer);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testTrainerTrainPokemon() {
  TRAINER_SET_UP();

  TrainerErrorCode train_result;
  Trainer empty_trainer = demoTrainerEmpty("trainer", locations);
  Trainer trainer_with_items = demoTrainerWithItems("trainer", locations);
  Trainer trainer_with_pokemon = demoTrainerWithPokemon("trainer", locations);
  Trainer full_trainer = demoTrainerFull("trainer", locations);
  
  // NULL input
  train_result = trainerTrainPokemon(NULL, 1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == train_result);

  // Pokemon doesn't exist
  train_result = trainerTrainPokemon(empty_trainer, 1);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == train_result);
  train_result = trainerTrainPokemon(trainer_with_items, 1);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == train_result);
  train_result = trainerTrainPokemon(trainer_with_pokemon, 134);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == train_result);
  train_result = trainerTrainPokemon(full_trainer, 145);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == train_result);

  // No candies
  train_result = trainerTrainPokemon(trainer_with_pokemon, 1);
  ASSERT_TEST(TRAINER_NO_AVAILABLE_ITEM_FOUND == train_result);

  // Valid input
  // full_trainer has 2 candies
  train_result = trainerTrainPokemon(full_trainer, 1);
  ASSERT_TEST(TRAINER_SUCCESS == train_result);
  train_result = trainerTrainPokemon(full_trainer, 1);
  ASSERT_TEST(TRAINER_SUCCESS == train_result);

  // No candies left
  train_result = trainerTrainPokemon(full_trainer, 1);
  ASSERT_TEST(TRAINER_NO_AVAILABLE_ITEM_FOUND == train_result);

  trainerDestroy(empty_trainer);
  trainerDestroy(trainer_with_items);
  trainerDestroy(trainer_with_pokemon);
  trainerDestroy(full_trainer);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testTrainersBattle() {
  TRAINER_SET_UP();

  Trainer trainer1 = demoTrainerWithPokemon("trainer", locations);
  Trainer trainer2 = demoTrainerWithPokemon("trainer", locations);
  TrainerErrorCode battle_result;

  // NULL input
  battle_result = trainersBattle(NULL, 1, trainer2, 1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == battle_result);
  battle_result = trainersBattle(trainer1, 1, NULL, 1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == battle_result);

  // Pokemon doesn't exist
  battle_result = trainersBattle(trainer1, 214, trainer2, 1);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == battle_result);
  battle_result = trainersBattle(trainer1, 1, trainer2, 34);
  ASSERT_TEST(TRAINER_POKEMON_DOESNT_EXIST == battle_result);

  // Valid input
  battle_result = trainersBattle(trainer1, 1, trainer2, 1);
  ASSERT_TEST(TRAINER_SUCCESS == battle_result);

  trainerDestroy(trainer1);
  trainerDestroy(trainer2);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testTrainerGoToLocation() {
  TRAINER_SET_UP();

  Location neighbor = mapGet(locations, "neighbor");
  Location full_location = mapGet(locations, "full_location");
  Location location = mapGet(locations, "empty_location");
  Trainer trainer = demoTrainerEmpty("trainer", locations);
  TrainerErrorCode go_result;

  // Invalid argument
  go_result = trainerGoToLocation(NULL, location);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == go_result);
  go_result = trainerGoToLocation(trainer, NULL);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == go_result);

  // Already in location
  go_result = trainerGoToLocation(trainer, location);
  ASSERT_TEST(TRAINER_ALREADY_IN_LOCATION == go_result);

  // Location not reachable
  go_result = trainerGoToLocation(trainer, full_location);
  ASSERT_TEST(TRAINER_LOCATION_IS_NOT_REACHABLE == go_result);

  // Success
  go_result = trainerGoToLocation(trainer, neighbor);
  ASSERT_TEST(TRAINER_SUCCESS == go_result);

  trainerDestroy(trainer);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testTrainerHunt() {
  TRAINER_SET_UP();

  Trainer trainer = demoTrainerEmpty("trainer", locations);
  FILE* output_channel = stdout;
  TrainerErrorCode hunt_result;

  // Invalid argument
  hunt_result = trainerHunt(NULL, output_channel);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == hunt_result);
  hunt_result = trainerHunt(trainer, NULL);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == hunt_result);

  // Successful hunt; location has 1 pokemon in it
  hunt_result = trainerHunt(trainer, output_channel);
  ASSERT_TEST(TRAINER_SUCCESS == hunt_result);

  // Successful hunt; location is now empty
  hunt_result = trainerHunt(trainer, output_channel);
  ASSERT_TEST(TRAINER_SUCCESS == hunt_result);

  trainerDestroy(trainer);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testTrainerBuyItem() {
  TRAINER_SET_UP();

  Trainer poor_trainer = demoTrainerWithMoney("trainer", 0, locations);
  Trainer normal_trainer = demoTrainerWithMoney("trainer", 5, locations);
  Trainer rich_trainer = demoTrainerWithMoney("trainer", 500, locations);
  Store store = demoStore();
  TrainerErrorCode buy_result;

  // Invalid argument
  buy_result = trainerBuyItem(NULL, store, "candy", 1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == buy_result);
  buy_result = trainerBuyItem(poor_trainer, NULL, "candy", 1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == buy_result);
  buy_result = trainerBuyItem(poor_trainer, store, NULL, 1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == buy_result);
  buy_result = trainerBuyItem(poor_trainer, store, "drugs", 1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == buy_result);
  buy_result = trainerBuyItem(poor_trainer, store, "candy", -1);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == buy_result);
  buy_result = trainerBuyItem(poor_trainer, store, "candy", 0);
  ASSERT_TEST(TRAINER_INVALID_ARGUMENT == buy_result);

  // Out of stock
  buy_result = trainerBuyItem(poor_trainer, store, "candy", 1234);
  ASSERT_TEST(TRAINER_STORE_ITEM_OUT_OF_STOCK == buy_result);
  buy_result = trainerBuyItem(poor_trainer, store, "potion", 1234);
  ASSERT_TEST(TRAINER_STORE_ITEM_OUT_OF_STOCK == buy_result);
  buy_result = trainerBuyItem(rich_trainer, store, "candy", 1234);
  ASSERT_TEST(TRAINER_STORE_ITEM_OUT_OF_STOCK == buy_result);
  buy_result = trainerBuyItem(rich_trainer, store, "potion", 1234);
  ASSERT_TEST(TRAINER_STORE_ITEM_OUT_OF_STOCK == buy_result);

  // Budget is insufficient
  buy_result = trainerBuyItem(poor_trainer, store, "candy", 2);
  ASSERT_TEST(TRAINER_BUDGET_IS_INSUFFICIENT == buy_result);
  buy_result = trainerBuyItem(normal_trainer, store, "candy", 100);
  ASSERT_TEST(TRAINER_BUDGET_IS_INSUFFICIENT == buy_result);
  buy_result = trainerBuyItem(poor_trainer, store, "potion", 2);
  ASSERT_TEST(TRAINER_BUDGET_IS_INSUFFICIENT == buy_result);
  buy_result = trainerBuyItem(normal_trainer, store, "potion", 100);
  ASSERT_TEST(TRAINER_BUDGET_IS_INSUFFICIENT == buy_result);

  // Success; normal trainer has money for 2 candies and a potion.
  buy_result = trainerBuyItem(normal_trainer, store, "candy", 2);
  ASSERT_TEST(TRAINER_SUCCESS == buy_result);
  buy_result = trainerBuyItem(normal_trainer, store, "potion", 1);
  ASSERT_TEST(TRAINER_SUCCESS == buy_result);

  // Budget is insufficient again
  buy_result = trainerBuyItem(normal_trainer, store, "candy", 2);
  ASSERT_TEST(TRAINER_BUDGET_IS_INSUFFICIENT == buy_result);
  buy_result = trainerBuyItem(normal_trainer, store, "potion", 1);
  ASSERT_TEST(TRAINER_BUDGET_IS_INSUFFICIENT == buy_result);

  // Rich trainer cleans the shop
  buy_result = trainerBuyItem(rich_trainer, store, "candy", 2);
  ASSERT_TEST(TRAINER_SUCCESS == buy_result);
  buy_result = trainerBuyItem(rich_trainer, store, "potion", 1);
  ASSERT_TEST(TRAINER_SUCCESS == buy_result);
  buy_result = trainerBuyItem(rich_trainer, store, "potion", 1);
  ASSERT_TEST(TRAINER_SUCCESS == buy_result);

  // Store has run out of stock
  buy_result = trainerBuyItem(rich_trainer, store, "candy", 2);
  ASSERT_TEST(TRAINER_STORE_ITEM_OUT_OF_STOCK == buy_result);
  buy_result = trainerBuyItem(rich_trainer, store, "potion", 1);
  ASSERT_TEST(TRAINER_STORE_ITEM_OUT_OF_STOCK == buy_result);

  storeDestroy(store);
  trainerDestroy(poor_trainer);
  trainerDestroy(normal_trainer);
  trainerDestroy(rich_trainer);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testGetTrainerName() {
  TRAINER_SET_UP();

  Trainer trainer = demoTrainerEmpty("trainer", locations);
  char* name;

  // Invalid argument
  name = trainerGetName(NULL);
  ASSERT_TEST(NULL == name);

  // Success
  name = trainerGetName(trainer);
  ASSERT_TEST(name != NULL);

  trainerDestroy(trainer);
  TRAINER_TEAR_DOWN();
  return true;
}

bool testGetTrainerXP() {
  TRAINER_SET_UP();

  Trainer trainer = demoTrainerEmpty("trainer", locations);
  double xp;

  // Invalid argument
  xp = trainerGetXP(NULL);
  ASSERT_TEST(-1 == xp);

  // Success
  xp = trainerGetXP(trainer);
  ASSERT_TEST(xp >= TRAINER_INITIAL_XP);

  trainerDestroy(trainer);
  TRAINER_TEAR_DOWN();
  return true;
}

int main() {
  RUN_TEST(testCreateTrainer);
  RUN_TEST(testDestroyTrainer);
  RUN_TEST(testCopyTrainer);
  RUN_TEST(testGetTrainerPokemon);
  RUN_TEST(testTrainerRemovePokemon);
  RUN_TEST(testTrainerHealPokemon);
  RUN_TEST(testTrainerTrainPokemon);
  RUN_TEST(testTrainersBattle);
  RUN_TEST(testTrainerGoToLocation);
  RUN_TEST(testTrainerHunt);
  RUN_TEST(testTrainerBuyItem);
  RUN_TEST(testGetTrainerName);
  RUN_TEST(testGetTrainerXP);

  return 0;
}
