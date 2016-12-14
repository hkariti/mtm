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
MtmErrorCode inventoryAddItem(int value);
bool inventoryPopItem(int value); // return true if exist or false if out of stock
void printInventory(char* inventory_type);

#endif 