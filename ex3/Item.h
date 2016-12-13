#ifndef ITEM_H_
#define ITEM_H_

#include "print_utils.h"

typedef struct Item_t *Item;

enum ItemType {
	Potion,
	Candy
};

struct Item_t { //TODO: move to .c file
	ItemType type;
	int value;
	int quantity;
};

Item createItem(char* type, int value, int quantity);
void destroyITem(Item item);
Item copyItem(Item item);
void printItem(Item item);
int getItemPrice(Item item);
void itemIncreaseQuantity(Item item);
void itemDecreaseQuantity(Item item); // Should return error if already at 0 or just keep it at 0?
void itemResetQuantity(Item item);
bool isItemOver(Item item);

#endif 