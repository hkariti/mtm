#include "store_mocks.h"

Store demoStore() {
  Store store = storeCreate();
  storeAddItem(store, "potion", 1);
  storeAddItem(store, "potion", 1);
  storeAddItem(store, "potion", 1);
  storeAddItem(store, "potion", 2);
  storeAddItem(store, "potion", 2);
  storeAddItem(store, "potion", 100);
  storeAddItem(store, "candy", 1);
  storeAddItem(store, "candy", 1);
  storeAddItem(store, "candy", 2);
  storeAddItem(store, "candy", 2);
  storeAddItem(store, "candy", 100);

  return store;
}
