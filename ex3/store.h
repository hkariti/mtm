#ifndef _STORE_H
#define _STORE_H
#include "map.h"
#include "inventory.h"

typedef struct store_t *Store;

typedef enum {
  STORE_SUCCESS,
  STORE_INVALID_ARGUMENT,
  STORE_ITEM_OUT_OF_STOCK,
  STORE_OUT_OF_MEMORY
} StoreErrorCode;

Store storeCreate();
void storeDestroy(Store store);
StoreErrorCode storeAddItem(Store store, char* type, int value);
StoreErrorCode storeBuyItem(Store store, char* type, int value);
StoreErrorCode storeGetItemPrice(Store store, char* type, int value, int* price); //returns -1 if item doesnt exist
void storePrintStock(Store store, FILE* output_channel);

#endif
