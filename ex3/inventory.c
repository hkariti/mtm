#include <stdlib.h>
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

MapResult inventoryAddItem(Inventory inventory, int value) {
  if (NULL == inventory) return MAP_NULL_ARGUMENT;
  int *amount;
  amount = mapGet(inventory, &value);
  if (NULL == amount) {
    // Item not in inventory, add a new item
    amount = malloc(sizeof(int));
    if (NULL == amount) return MAP_OUT_OF_MEMORY;
    *amount = 1;
  } else {
    // Item in inventory, increase its quantity by one
    *amount = *amount + 1;
  }

  // TODO: Free amount, possible memory leak here
  return mapPut(inventory, &value, amount);

}

bool inventoryContains(Inventory inventory, int value) {
  return mapContains(inventory, &value);
}

bool inventoryPopItem(Inventory inventory, int value) {
  if (NULL == inventory) return false;
  int *amount;

  amount = mapGet(inventory, &value);
  // Return false if item isn't in stock
  if (NULL == amount) return false;
  // Decrease item's quantity
  *amount = *amount - 1;
  // Remove item from inventory if it's run out
  if (*amount == 0) {
    mapRemove(inventory, &value);
  } else {
    mapPut(inventory, &value, amount);
  }
  return true;
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
