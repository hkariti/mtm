#include "../inventory.h"
#include "mocks/inventory_mocks.h"
#include "test_utilities.h"

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
  InventoryErrorCode add_result;

  // NULL input
  add_result = inventoryAddItem(NULL, 1);
  ASSERT_TEST(INVENTORY_INVALID_ARGUMENT == add_result);

  // New item
  add_result = inventoryAddItem(inventory, 1);
  ASSERT_TEST(INVENTORY_SUCCESS == add_result);

  // Existing item
  add_result = inventoryAddItem(inventory, 1);
  ASSERT_TEST(INVENTORY_SUCCESS == add_result);

  destroyInventory(inventory);
  return true;
}

bool testInventoryContains() {
  Inventory inventory = demoInventory();

  // NULL input
  ASSERT_TEST(inventoryContains(NULL, 1) == false);

  // Valid input
  ASSERT_TEST(inventoryContains(inventory, 1) == true);
  ASSERT_TEST(inventoryContains(inventory, 1337) == false);

  destroyInventory(inventory);
  return true;
}

bool testInventoryRemoveItem() {
  Inventory inventory = demoInventory();
  InventoryErrorCode remove_result;

  // NULL input
  remove_result = inventoryRemoveItem(NULL, 1);
  ASSERT_TEST(INVENTORY_INVALID_ARGUMENT == remove_result);

  // Item not in stock
  remove_result = inventoryRemoveItem(inventory, 1337);
  ASSERT_TEST(INVENTORY_OUT_OF_STOCK == remove_result);

  // Successful input; "2" exists twice in our demo inventory
  remove_result = inventoryRemoveItem(inventory, 2);
  ASSERT_TEST(INVENTORY_SUCCESS == remove_result);
  remove_result = inventoryRemoveItem(inventory, 2);
  ASSERT_TEST(INVENTORY_SUCCESS == remove_result);
  // Item has run out now
  remove_result = inventoryRemoveItem(inventory, 2);
  ASSERT_TEST(INVENTORY_OUT_OF_STOCK == remove_result);

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

  destroyInventory(inventory);
  return true;
}

int main() {
  RUN_TEST(testCreateInventory);
  RUN_TEST(testDestroyInventory);
  RUN_TEST(testCopyInventory);
  RUN_TEST(testInventoryAddItem);
  RUN_TEST(testInventoryContains);
  RUN_TEST(testInventoryRemoveItem);
  RUN_TEST(testPrintInventory);
  RUN_TEST(testInventoryForeach);

  return 0;
}
