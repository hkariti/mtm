#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "store.h"

struct store_t {
  Inventory potions;
  Inventory candies;
};

/**
 * storeGetInventoryByType - gets the relevant Inventory holding items of a type
 *
 * @param store - existing store
 * @param type  - item type
 *
 * @return
 *  NULL - invalid type
 *  An Inventory object otherwise
 */
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
  store->potions = inventoryCreate();
  store->candies = inventoryCreate();
  if (NULL == store->potions || NULL == store->candies) {
    storeDestroy(store);
    return NULL;
  }

  return store;
}

void storeDestroy(Store store) {
  if (NULL == store) return;
  inventoryDestroy(store->candies);
  inventoryDestroy(store->potions);
  free(store);
}

StoreErrorCode storeAddItem(Store store, char* type, int value) {
  if (NULL == store || NULL == type || value <= 0) {
      return STORE_INVALID_ARGUMENT;
  }

  Inventory chosen_inventory;
  InventoryErrorCode add_item_result;
  chosen_inventory = storeGetInventoryByType(store, type);
  if (NULL == chosen_inventory) return STORE_INVALID_ARGUMENT;
  add_item_result = inventoryAddItem(chosen_inventory, value);
  assert(add_item_result != INVENTORY_INVALID_ARGUMENT);
  if (add_item_result == INVENTORY_OUT_OF_MEMORY) return STORE_OUT_OF_MEMORY;
  return STORE_SUCCESS;
}

StoreErrorCode storeBuyItem(Store store, char* type, int value) {
  if (NULL == store || NULL == type || value <= 0) {
      return STORE_INVALID_ARGUMENT;
  }

  Inventory chosen_inventory;
  InventoryErrorCode remove_result;
  chosen_inventory = storeGetInventoryByType(store, type);
  if (NULL == chosen_inventory) return STORE_INVALID_ARGUMENT;
  remove_result = inventoryRemoveItem(chosen_inventory, value);
  if (INVENTORY_OUT_OF_MEMORY == remove_result) return STORE_OUT_OF_MEMORY;
  if (INVENTORY_OUT_OF_STOCK == remove_result) return STORE_ITEM_OUT_OF_STOCK;
  return STORE_SUCCESS;
}

StoreErrorCode storeGetItemPrice(Store store, char* type, int value,
                                 int* price) {
  if (NULL == store || NULL == type || NULL == price || value <= 0) {
    return STORE_INVALID_ARGUMENT;
  }
  Inventory chosen_inventory;
  int price_per_value;
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
  inventoryPrint(store->candies, "candy", output_channel);
  inventoryPrint(store->potions, "potion", output_channel);
}
