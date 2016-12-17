#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "print_utils.h"
#include "map.h"

typedef Map Inventory;
typedef Inventory Potions;
typedef Inventory Candies;

Inventory createInventory();
void destroyInventory(Inventory inventory);
Inventory copyInventory(Inventory inventory);
MapResult inventoryAddItem(Inventory inventory, int value);
 // return true if exist or false if out of stock
bool inventoryPopItem(Inventory inventory, int value);

void printInventory(Inventory inventory, char* inventory_type,
                    FILE* output_channel);

#define INVENTORY_FOREACH(iterator, inventory) MAP_FOREACH(int*, iterator, \
                                                           inventory)
#endif
