#ifndef ITEM_H_
#define ITEM_H_

#include "print_utils.h"

typedef struct Item_t *Item;

enum ItemType {
	Potion,
	Candy
};

struct Item_t { //TODO: move to .c file
	int value;
	int quantity;
	ItemType type;
};

Item createItem(ItemType type, int value, int quantity);
void destroyITem(Item item);
Item copyItem(Item item);
int itemCompareByValue(Item item_1, Item item_2);

void printItem(Item item);
int getItemPrice(Item item);
void itemIncreaseQuantity(Item item);
void itemDecreaseQuantity(Item item); // Should return error if already at 0 or just keep it at 0?
void itemResetQuantity(Item item);
bool isItemOver(Item item);

#endif 