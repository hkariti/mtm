#include <stdlib.h>
#include <assert.h>
#include "inventory.h"
#include "utils.h"

Inventory createInventory() {
  Map inventory;
  inventory = mapCreate((copyMapKeyElements)intCopy,
                        (copyMapDataElements)intCopy,
                        (freeMapKeyElements)intDestroy,
                        (freeMapDataElements)intDestroy,
                        (compareMapKeyElements)intCompare);

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
  int *amount, *new_amount;

  amount = mapGet(inventory, &value);
  // Check if item is out of stock
  if (NULL == amount) return INVENTORY_OUT_OF_STOCK;
  // Remove item from inventory if it's run out now
  if (*amount == 1) {
    mapRemove(inventory, &value);
  } else {
    // Decrease item's quantity.
    new_amount = malloc(sizeof(int));
    if (NULL == new_amount) return INVENTORY_OUT_OF_MEMORY;
    *new_amount = *amount - 1;
    MapResult put_result = mapPut(inventory, &value, new_amount);
    assert(put_result != MAP_NULL_ARGUMENT);
    free(new_amount);
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
