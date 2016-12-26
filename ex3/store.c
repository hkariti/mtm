#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "store.h"

struct store_t {
  Inventory potions;
  Inventory candies;
};

static Inventory storeGetInventoryByType(Store store, char* type) {
  assert(store != NULL);
  assert(type != NULL);
  if (strcmp(type, "candy") == 0) {
    return store->candies;
  } else if (strcmp(type, "potion") == 0) {
    return store->potions;
  } else {
    return NULL;
  }
}

Store storeCreate() {
  Store store;
  store = malloc(sizeof(struct store_t));
  if (NULL == store) return NULL;
  store->potions = createInventory();
  store->candies = createInventory();
  if (NULL == store->potions || NULL == store->candies) {
    storeDestroy(store);
    return NULL;
  }

  return store;
}

void storeDestroy(Store store) {
  if (NULL == store) return;
  destroyInventory(store->candies);
  destroyInventory(store->potions);
}

StoreErrorCode storeAddItem(Store store, char* type, int value) {
  assert(store != NULL);
  assert(type != NULL);
  if (value <= 0) return STORE_INVALID_ARGUMENT;

  Inventory chosen_inventory;
  MapResult add_item_result;
  chosen_inventory = storeGetInventoryByType(store, type);
  if (NULL == chosen_inventory) return STORE_INVALID_ARGUMENT;
  add_item_result = inventoryAddItem(chosen_inventory, value);
  assert(add_item_result != MAP_NULL_ARGUMENT);
  if (add_item_result == MAP_OUT_OF_MEMORY) return STORE_OUT_OF_MEMORY;
  return STORE_SUCCESS;
}

StoreErrorCode storeBuyItem(Store store, char* type, int value) {
  assert(store != NULL);
  assert(type != NULL);
  if (value <= 0) return STORE_INVALID_ARGUMENT;

  Inventory chosen_inventory;
  bool pop_result;
  chosen_inventory = storeGetInventoryByType(store, type);
  if (NULL == chosen_inventory) return STORE_INVALID_ARGUMENT;
  pop_result = inventoryPopItem(chosen_inventory, value);
  if (!pop_result) return STORE_ITEM_OUT_OF_STOCK;
  return STORE_SUCCESS;
}

StoreErrorCode storeGetItemPrice(Store store, char* type, int value, int* price)
                              {
  assert(store != NULL);
  assert(type != NULL);
  assert(price != NULL);
  Inventory chosen_inventory;
  int price_per_value;
  if (value < 0) return STORE_INVALID_ARGUMENT;
  if (strcmp(type, "candy") == 0) {
    chosen_inventory = store->candies;
    price_per_value = 2;

  } else if (strcmp(type, "potion") == 0) {
    chosen_inventory = store->potions;
    price_per_value = 1;
  } else {
    return STORE_INVALID_ARGUMENT;
  }

  if (!inventoryContains(chosen_inventory, value)) {
    return STORE_ITEM_OUT_OF_STOCK;
  }

  *price = price_per_value * value;
  return STORE_SUCCESS;
}

void storePrintStock(Store store, FILE* output_channel) {
  assert(store != NULL);
  assert(output_channel != NULL);
  mtmPrintStockHeader(output_channel);
  printInventory(store->candies, "candy", output_channel);
  printInventory(store->potions, "potion", output_channel);
}