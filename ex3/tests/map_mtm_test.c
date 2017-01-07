#include "test_utilities.h"
#include <string.h>
#include "../map_mtm/map_mtm.h"
#include "../utils.h"

bool testMapCreateDestroy() {
    Map map = mapCreate((copyMapKeyElements)stringCopy,
            (copyMapDataElements)intCopy,
            (freeMapKeyElements)free,
            (freeMapDataElements)free,
            (compareMapKeyElements)stringCompare);
    ASSERT_TEST(map != NULL);
    ASSERT_TEST(mapGetSize(map) == 0);
    ASSERT_TEST(mapGetFirst(map) == NULL);
    mapDestroy(map);
    return true;

}

bool testMapPutAndIterate() {
    Map map = mapCreate((copyMapKeyElements)intCopy,
            (copyMapDataElements)intCopy,
            (freeMapKeyElements)free,
            (freeMapDataElements)free,
            (compareMapKeyElements)intCompare);
    for (int i = 1; i<1001; ++i) {
        ASSERT_TEST(mapPut(map, &i, &i) == MAP_SUCCESS);
        ASSERT_TEST(mapGetSize(map) == i);
    }
    int* current_key = mapGetFirst(map);
    for (int i = 1; i<1001; ++i) {
        int* current_data = mapGet(map, current_key);
        ASSERT_TEST(*current_data == i);
        current_key = mapGetNext(map);
    }
    int* current_data = mapGet(map, current_key);
    ASSERT_TEST(current_data == NULL);
    mapDestroy(map);
    return true;
}

bool testMapGet() {
    Map map = mapCreate((copyMapKeyElements)stringCopy,
            (copyMapDataElements)intCopy,
            (freeMapKeyElements)free,
            (freeMapDataElements)free,
            (compareMapKeyElements)stringCompare);
    char buffer[1000];
    for (int i = 1; i < 1001; ++i) {
        sprintf(buffer, "%d", i);
        ASSERT_TEST(mapPut(map, buffer, &i) == MAP_SUCCESS);
        ASSERT_TEST(mapGetSize(map) == i);
    }
    for (int i = 1; i < 1001; ++i) {
        sprintf(buffer, "%d", i);
        ASSERT_TEST(*(int*)mapGet(map, buffer) == i);
    }
    for (int i = 1000; i > 0; --i) {
        sprintf(buffer, "%d", i);
        ASSERT_TEST(*(int*)mapGet(map, buffer) == i);
    }
    mapDestroy(map);
    return true;
}

bool testMapCopyAndClear() {
    Map map = mapCreate((copyMapKeyElements)stringCopy,
            (copyMapDataElements)intCopy,
            (freeMapKeyElements)free,
            (freeMapDataElements)free,
            (compareMapKeyElements)stringCompare);
    Map map_copy;
  char buffer[1000];
  for (int i = 0; i < 5; ++i) {
    sprintf(buffer, "%d", i);
    map_copy = mapCopy(map);
    ASSERT_TEST(map_copy != NULL);
    ASSERT_TEST(mapGetSize(map_copy) == i);
    ASSERT_TEST(mapGetSize(map) == i);

    int* current_key = mapGetFirst(map);
    for (int j = 1; j<= i; ++j) {
      int* current_data = mapGet(map_copy, current_key);
      ASSERT_TEST(*current_data == j-1);
      current_key = mapGetNext(map);
    }
    int* current_data = mapGet(map, current_key);
    ASSERT_TEST(current_data == NULL);

    ASSERT_TEST(mapPut(map, buffer, &i) == MAP_SUCCESS);
    ASSERT_TEST(mapClear(map_copy) == MAP_SUCCESS);
    mapDestroy(map_copy);
  }

  mapDestroy(map);
  return true;
}

int main() {
  RUN_TEST(testMapCreateDestroy);
  RUN_TEST(testMapPutAndIterate);
  RUN_TEST(testMapGet);
  RUN_TEST(testMapCopyAndClear);
  return 0;
}
