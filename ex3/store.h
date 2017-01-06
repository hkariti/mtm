#ifndef _STORE_H
#define _STORE_H
#include "map.h"
#include "inventory.h"

/**
 * The friendly inter-location store, just down the road.
 *
 * This implements the game's store. It contains potions and candies which
 * trainer can buy.
 *
 * The following functions are available:
 * storeCreate          - Create an empty store.
 * storeDestroy         - Destroy the store, freeing all resources and items.
 * storeAddItem         - Add an item to the store.
 * storeBuyItem         - Buy (i.e. remove) an item from the store.
 * storeGetItemPrice    - Get a given item's price, based on type and value.
 * storePrintStock      - Print a report of the store's stock.
 */

// Type definition
typedef struct store_t *Store;

// Possible errors
typedef enum {
  STORE_SUCCESS,
  STORE_INVALID_ARGUMENT,
  STORE_ITEM_OUT_OF_STOCK,
  STORE_OUT_OF_MEMORY
} StoreErrorCode;

/**
 * storeCreate - create an empty store.
 *
 * @return
 *  NULL - alloc error
 *  A new empty store on success.
 */
Store storeCreate();

/**
 * storeDestroy - Deallocate a store, freeing all memory and items.
 *
 * @param store - An existing store.
 *
 * If store is NULL, nothing will be done.
 */
void storeDestroy(Store store);

/**
 * storeAddItem - Add an item to the store
 *
 * @param store - an existing store.
 * @param type  - an item type. currently valid types are: candy, potion
 * @param value - the items' value.
 *
 * @return
 *  STORE_INVALID_ARGUMENT  - A NULL arg, invalid item type or value isn't
 *                            positive
 *  STORE_OUT_OF_MEMORY     - memory error
 *  STORE_SUCCESS           - item was added successfuly
 */
StoreErrorCode storeAddItem(Store store, char* type, int value);

/**
 * storeBuyItem - remove an item from the store
 *
 * @param store     - an existing store.
 * @param type      - an item type. currently valid tpes are: candy, potion
 * @param value     - an item's value.
 *
 * @return
 *  STORE_INVALID_ARGUMENT  - a NULL arg, invalid item type or value isn't
 *                            positive
 *  STORE_OUT_OF_MEMORY     - memory error
 *  STORE_ITEM_OUT_OF_STOCK - No item of the requested value in the store
 *  STORE_SUCCESS           - Item bought. Do come again.
 */
StoreErrorCode storeBuyItem(Store store, char* type, int value);

/**
 * storeGetItemPrice - get a price on an item
 *
 * @param store     - an existing store.
 * @param type      - an item type. Currently valid types are: candy, potion
 * @param value     - an item's value
 * @param price     - Will be filled with the item's price. Should be
 *                    preallocated.
 *
 * @return
 *  STORE_INVALID_ARGUMENT  - a NULL arg was passed or value isn't positive
 *  STORE_ITEM_OUT_OF_STOCK - Item of the requested value isn't in the store
 *  STORE_SUCCES            - successful query.
 */
StoreErrorCode storeGetItemPrice(Store store, char* type, int value,
                                 int* price);

/**
 * storePrintStock - print a report of the store's stock
 *
 * @param store             - An existing store.
 * @param output_channel    - An open file to write the report to.
 */
void storePrintStock(Store store, FILE* output_channel);

#endif
