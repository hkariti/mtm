#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "print_utils.h"
#include "map.h"

/**
 * Inventory - A generic container of items
 *
 * Implements a collection of items of the same type. Each item has a value
 * and a quantity. Note the inventory isn't aware of the types of item it
 * stores, that's up to you.
 *
 * The following functions are available:
 *  createInventory     - Creates a new empty inventory.
 *  destroyInventory    - Deletes an inventory and frees all resources.
 *  copyInventory       - Copies an existing inventory.
 *  inventoryAddItem    - Adds a new item to the inventory, given its value.
 *  inventoryRemoveItem - Removes an existing item, given its value, if it's
 *                        in the inventory.
 *  inventoryContains   - returns whether the inventory contains an item,
 *                        given by its value.
 *  printInventory      - Prints an inventory's stock
 *  INVENTORY_FOREACH   - Iterates over item values in the inventory, from
 *                        smallest value to highest value.
 */

// Type definition
typedef Map Inventory;

// Possible error codes
typedef enum {
	INVENTORY_SUCCESS,
	INVENTORY_INVALID_ARGUMENT,
	INVENTORY_OUT_OF_MEMORY,
	INVENTORY_OUT_OF_STOCK,
} InventoryErrorCode;

/**
 * createInventory - creates a new empty inventory.
 *
 * @return
 *  NULL - allocation error
 *  A new Inventory in case of success
 */
Inventory createInventory();

/**
 * destroyInventory - Deallocates an existing inventory and all its items.
 *
 * @param inventory - existing Inventory. If inventory is NULL nothing is done.
 */
void destroyInventory(Inventory inventory);

/**
 * copyInventory - Copies an existing Inventory and all its items.
 *
 * @param inventory - Inventory type to copy
 *
 * @return
 *  NULL - allocation error or NULL argument
 *  A new Inventory containing the same items in case of success
 */
Inventory copyInventory(Inventory inventory);

/**
 * inventoryAddItem - Adds an item to the inventory
 *
 * @param inventory - existing Inventory
 * @param value     - value of the item to add
 *
 * @return
 *  INVENTORY_INVALID_ARGUMENT  - NULL argument
 *  INVENTORY_OUT_OF_MEMORY     - allocation error
 *  INVENTORY_SUCCESS           - The item was inserted to the inventory
 */
InventoryErrorCode inventoryAddItem(Inventory inventory, int value);

/**
 * inventoryRemoveItem - Removes an item from the inventory
 *
 * @param inventory - existing Inventory
 * @param value     - Value of item to remove
 *
 * @return
 * INVENTORY_INVALID_ARGUMENT   - NULL argument was passed
 * INVENTORY_OUT_OF_STOCK       - Requested item is not in the inventory
 * INVENTORY_OUT_OF_MEMORY      - Allocation error
 * INVENTORY_SUCCESS            - The item was removed from the inventory.
 */
InventoryErrorCode inventoryRemoveItem(Inventory inventory, int value);

/**
 * inventoryContains - Checks if the inventory contains an item
 *
 * @param inventory - existing Inventory
 * @param value     - Value of item to check for
 *
 * @return
 *  true    - Item is in the inventory
 *  false   - Item is not in inventory or NULL arg
 */
bool inventoryContains(Inventory inventory, int value);

/**
 * printInventory - Print inventory stock, sorted by value
 *
 * @param inventory         - Existing inventory
 * @param type              - Type of items in the inventory
 * @param output_channel    - File to write the output to
 */
void printInventory(Inventory inventory, char* type,
                    FILE* output_channel);

/**
 * Macro for iterating over the items in the inventory.
 * Declares a new iterator for the loop.
 */
#define INVENTORY_FOREACH(iterator, inventory) MAP_FOREACH(int*, iterator, \
                                                           inventory)
#endif
