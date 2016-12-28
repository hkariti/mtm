#include <store.h>
#include "test_utilities.h"

bool testStoreCreate() {
  Store store;
  store = storeCreate();
  ASSERT_TEST(NULL != store);
  storeDestroy(store);

  return true;
}

bool testStoreDestroy() {
  Store store;

  // NULL input
  storeDestroy(NULL);

  // Empty store
  store = storeCreate();
  storeDestroy(store);

  // Full store
  store = demoStore();
  storeDestroy(store);

  return true;
}

bool testStoreAddItem() {
  Store store = storeCreate();
  StoreErrorCode add_result;

  // Invalid input
  add_result = storeAddItem(store, "candy", -1);
  ASSERT_TEST(STORE_INVALID_ARGUMENT == add_result);
  add_result = storeAddItem(store, "drugs", 1);
  ASSERT_TEST(STORE_INVALID_ARGUMENT == add_result);

  // New item
  add_result = storeAddItem(store, "candy", 1);
  ASSERT_TEST(STORE_SUCCESS == add_result);
  add_result = storeAddItem(store, "potion", 1);
  ASSERT_TEST(STORE_SUCCESS == add_result);
  // Existing item
  add_result = storeAddItem(store, "candy", 1);
  ASSERT_TEST(STORE_SUCCESS == add_result);
  add_result = storeAddItem(store, "potion", 1);
  ASSERT_TEST(STORE_SUCCESS == add_result);

  storeDestroy(store);
  return true;
}

bool testStoreBuyItem() {
  Store store = demoStore();
  StoreErrorCode buy_result;

  // Invalid input
  buy_result = storeBuyItem(store, "candy", -1);
  ASSERT_TEST(STORE_INVALID_ARGUMENT == buy_result);
  buy_result = storeBuyItem(store, "drugs", 1);
  ASSERT_TEST(STORE_INVALID_ARGUMENT == buy_result);

  // In stock item; candy/potion with value 2 exists twice in demo store.
  buy_result = storeBuyItem(store, "candy", 2);
  ASSERT_TEST(STORE_SUCCESS == buy_result);
  buy_result = storeBuyItem(store, "candy", 2);
  ASSERT_TEST(STORE_SUCCESS == buy_result);
  buy_result = storeBuyItem(store, "potion", 2);
  ASSERT_TEST(STORE_SUCCESS == buy_result);
  buy_result = storeBuyItem(store, "potion", 2);
  ASSERT_TEST(STORE_SUCCESS == buy_result);
  // Out of stock item
  buy_result = storeBuyItem(store, "candy", 2);
  ASSERT_TEST(STORE_ITEM_OUT_OF_STOCK == buy_result);
  buy_result = storeBuyItem(store, "potion", 2);
  ASSERT_TEST(STORE_ITEM_OUT_OF_STOCK == buy_result);

  storeDestroy(store);
  return true;
}

bool testStoreGetItemPrice() {
  Store store = demoStore();
  StoreErrorCode price_result;
  int price;

  // Invalid input
  price_result = storeGetItemPrice(store, "drugs", 1, &price);
  ASSERT_TEST(STORE_INVALID_ARGUMENT == price_result);
  price_result = storeGetItemPrice(store, "candy", -1, &price);
  ASSERT_TEST(STORE_INVALID_ARGUMENT == price_result);

  // Out of stock item
  price_result = storeGetItemPrice(store, "candy", 1337, &price);
  ASSERT_TEST(STORE_ITEM_OUT_OF_STOCK == price_result);

  // In stock item
  price_result = storeGetItemPrice(store, "candy", 2, &price);
  ASSERT_TEST(STORE_SUCCESS == price_result);
  price_result = storeGetItemPrice(store, "potion", 1, &price);
  ASSERT_TEST(STORE_SUCCESS == price_result);

  storeDestroy(store);
  return true;
}

int main() {
  RUN_TEST(testStoreCreate);
  RUN_TEST(testStoreDestroy);
  RUN_TEST(testStoreAddItem);
  RUN_TEST(testStoreBuyItem);
  RUN_TEST(testStoreGetItemPrice);

  return 0;
}
