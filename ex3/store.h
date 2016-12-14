#ifndef _STORE_H
#define _STORE_H
#include <map.h>
#include <item.h>

typedef Map Store;

Store storeCreate();
void storeDestroy(Store store);
MtmErrorCode storeAddItem(Store store, Item item);
MtmErrorCode storeRemoveItem(Store store, Item item);
Item storeGetItem(Store store, Item item);
void storePrintStock(Store store);

#endif
