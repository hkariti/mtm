#include <inventory.h>
#include "test_utilities.h"

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

bool testCreateInventory() {
  Inventory inventory = createInventory();

  ASSERT_TEST(NULL != inventory);

  destroyInventory(inventory);
  return true;
}

bool testDestroyInventory() {
  Inventory inventory = demoInventory();

  // Test NULL input
  destroyInventory(NULL);

  // Valid input
  destroyInventory(inventory);

  return true;
}

bool testCopyInventory() {
  Inventory inventory = demoInventory();
  Inventory inventory_copy;

  // NULL input
  inventory_copy = copyInventory(NULL);
  ASSERT_TEST(NULL == inventory_copy);

  // Valid input
  inventory_copy = copyInventory(inventory);
  ASSERT_TEST(NULL != inventory_copy);

  destroyInventory(inventory_copy);
  destroyInventory(inventory);
  return true;
}

bool testInventoryAddItem() {
  Inventory inventory = createInventory();
  MapResult add_result;

  // NULL input
  add_result = inventoryAddItem(NULL, 1);
  ASSERT_TEST(MAP_NULL_ARGUMENT == add_result);

  // New item
  add_result = inventoryAddItem(inventory, 1);
  ASSERT_TEST(MAP_SUCCESS == add_result);

  // Existing item
  add_result = inventoryAddItem(inventory, 1);
  ASSERT_TEST(MAP_SUCCESS == add_result);

  return true;
}

bool testInventoryContains() {
  Inventory inventory = demoInventory();

  ASSERT_TEST(inventoryContains(inventory, 1) == true);
  ASSERT_TEST(inventoryContains(inventory, 1337) == false);

  destroyInventory(inventory);
  return true;
}
bool testInventoryPopItem() {
  Inventory inventory = demoInventory();
  bool pop_result;

  // NULL input
  pop_result = inventoryPopItem(NULL, 1);
  ASSERT_TEST(false == pop_result);

  // Item not in stock
  pop_result = inventoryPopItem(inventory, 1337);
  ASSERT_TEST(false == pop_result);

  // Successful input; "2" exists twice in our demo inventory
  pop_result = inventoryPopItem(inventory, 2);
  ASSERT_TEST(true == pop_result);
  pop_result = inventoryPopItem(inventory, 2);
  ASSERT_TEST(true == pop_result);
  // Item has run out now
  pop_result = inventoryPopItem(inventory, 2);
  ASSERT_TEST(false == pop_result);

  destroyInventory(inventory);
  return true;
}

bool testPrintInventory() {
  FILE* dummy_file = (FILE*)0xdeadbeef;
  Inventory dummy_inventory = (Inventory)0x1337;

  // NULL inputs
  printInventory(NULL, "potion", dummy_file);
  printInventory(dummy_inventory, NULL, dummy_file);
  printInventory(dummy_inventory, "potion", NULL);

  return true;
}

bool testInventoryForeach() {
  Inventory inventory = demoInventory();

  INVENTORY_FOREACH(i, inventory) {
    ASSERT_TEST(*i > 0);
  }
  return true;
}

int main() {
  RUN_TEST(testCreateInventory);
  RUN_TEST(testDestroyInventory);
  RUN_TEST(testCopyInventory);
  RUN_TEST(testInventoryAddItem);
  RUN_TEST(testInventoryContains);
  RUN_TEST(testInventoryPopItem);
  RUN_TEST(testPrintInventory);
  RUN_TEST(testInventoryForeach);

  return 0;
}
