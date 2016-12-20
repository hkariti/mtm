#ifndef _STORE_H
#define _STORE_H
#include "map.h"
#include "inventory.h"

typedef Map Store;

Store storeCreate();
void storeDestroy(Store store);
MtmErrorCode storeAddItem(Store store, char* type, int value);
MtmErrorCode storeBuytem(Store store, char* type, int value);
int storeGetItemPrice(Store store, char* type, int value); //returns -1 if item doesnt exist
void storePrintStock(Store store);

#endif
