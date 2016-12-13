#ifndef ITEM_H_
#define ITEM_H_

typedef struct Item_t *Item;

Item CreateItem(char* type, int value, int quantity);
void DestroyItem(Item item);
Item ItemCopy(Item item);
int GetItemPrice(Item item);
void IncreaseQuantity(Item item);
void DecreaseQuantity(Item item); // Should return error if already at 0 or just keep it at 0?
void ResetQuantity(Item item);
bool isItemOver(Item item);
void PrintItem(Item item);

#endif 