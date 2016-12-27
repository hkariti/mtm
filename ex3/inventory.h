#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "print_utils.h"
#include "map.h"

typedef enum {
	INVENTORY_SUCCESS,
	INVENTORY_INVALID_ARGUMENT,
	INVENTORY_OUT_OF_MEMORY,
	INVENTORY_OUT_OF_STOCK,
} InventoryErrorCode;


//TODO: implement invntory.c using new struct
typedef struct Inventory_t { //TODO: move to .c file
	Map Inventory;
	char* type;
} *Inventory;

typedef Map Inventory;

Inventory createInventory(char* type);
void destroyInventory(Inventory inventory);
// NULL if alloc error
Inventory copyInventory(Inventory inventory);
// errors: INVENTORY_INVALID_ARGUMENT, OUT_OF_MEMORY
InventoryErrorCode inventoryAddItem(Inventory inventory, int value);
// errors:  INVALID_ARGUMENT, OUT_OF_STOCK
InventoryErrorCode inventoryRemoveItem(Inventory inventory, int value);

// return false if null arg
bool inventoryContains(Inventory inventory, int value);

void printInventory(Inventory inventory,
                    FILE* output_channel);

#define INVENTORY_FOREACH(iterator, inventory) MAP_FOREACH(int*, iterator, \
                                                           inventory)
#endif
