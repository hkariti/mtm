#ifndef ITEM_H_
#define ITEM_H_

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
void increaseQuantity(Item item);
void decreaseQuantity(Item item); // Should return error if already at 0 or just keep it at 0?
void resetQuantity(Item item);
bool isItemOver(Item item);

#endif 