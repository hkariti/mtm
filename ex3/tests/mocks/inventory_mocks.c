#include "inventory_mocks.h"

Inventory demoInventory() {
  Inventory inventory = createInventory();

  inventoryAddItem(inventory, 1);
  inventoryAddItem(inventory, 1);
  inventoryAddItem(inventory, 1);
  inventoryAddItem(inventory, 1);

  inventoryAddItem(inventory, 2);
  inventoryAddItem(inventory, 2);

  return inventory;
}
