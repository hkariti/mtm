#include <stdlib.h>
#include <assert.h>
#include <inventory.h>

static int uintCompare(int* a, int* b) {
  return *a - *b;
}

static int* uintCopy(int *key) {
  int *key_copy;
  key_copy = malloc(sizeof(int));
  if (NULL == key_copy) return NULL;
  *key_copy = *key;
  return key_copy;
}

static void uintDestroy(int *key) {
  free(key);
}

Inventory createInventory() {
  Map inventory;
  inventory = mapCreate((copyMapKeyElements)uintCopy,
                        (copyMapDataElements)uintCopy,
                        (freeMapKeyElements)uintDestroy,
                        (freeMapDataElements)uintDestroy,
                        (compareMapKeyElements)uintCompare);

  return inventory;
}

void destroyInventory(Inventory inventory) {
  mapDestroy(inventory);
}

Inventory copyInventory(Inventory inventory) {
  if (NULL == inventory) return NULL;
  return mapCopy(inventory);
}

InventoryErrorCode inventoryAddItem(Inventory inventory, int value) {
  if (NULL == inventory) return INVENTORY_INVALID_ARGUMENT;
  int *amount, *new_amount;
  new_amount = malloc(sizeof(int));
  if (NULL == new_amount) return INVENTORY_OUT_OF_MEMORY;
  amount = mapGet(inventory, &value);
  if (NULL == amount) {
    // Item not in inventory, add a new item
    *new_amount = 1;
  } else {
    // Item in inventory, increase its quantity by one
    *new_amount = *amount + 1;
  }

  MapResult put_result = mapPut(inventory, &value, new_amount);
  free(new_amount);
  assert(put_result != MAP_NULL_ARGUMENT);
  if (MAP_OUT_OF_MEMORY == put_result) return INVENTORY_OUT_OF_MEMORY;
  return INVENTORY_SUCCESS;
}

bool inventoryContains(Inventory inventory, int value) {
  return mapContains(inventory, &value);
}

InventoryErrorCode inventoryRemoveItem(Inventory inventory, int value) {
  if (NULL == inventory) return INVENTORY_INVALID_ARGUMENT;
  int *amount;

  amount = mapGet(inventory, &value);
  // Check if item is out of stock
  if (NULL == amount) return INVENTORY_OUT_OF_STOCK;
  // Decrease item's quantity
  *amount = *amount - 1;
  // Remove item from inventory if it's run out
  if (*amount == 0) {
    mapRemove(inventory, &value);
  } else {
    MapResult put_result = mapPut(inventory, &value, amount);
    assert(put_result != MAP_NULL_ARGUMENT);
    if (MAP_OUT_OF_MEMORY == put_result) return INVENTORY_OUT_OF_MEMORY;
  }
  return INVENTORY_SUCCESS;
}

void printInventory(Inventory inventory, char* inventory_type,
                    FILE* output_channel) {
  if (NULL == inventory || NULL == inventory_type ||  NULL == output_channel) {
    return;
  }
  MAP_FOREACH(int*, value, inventory) {
    int *quantity = mapGet(inventory, value);
    mtmPrintItem(output_channel, inventory_type, *value, *quantity);
  }
}
