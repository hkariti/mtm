#include "map_mtm.h"
#include <stdlib.h>

typedef struct Element_t {
  MapKeyElement key;
  MapDataElement data;
  struct Element_t* next;
} *Element;

struct Map_t {
  Element first_element;
  Element iterator;
  bool iterator_is_undefined;
  copyMapKeyElements copyKey;
  copyMapDataElements copyData;
  freeMapKeyElements freeKey;
  freeMapDataElements freeData;
  compareMapKeyElements compareKeys;
};

/**
*  mapFind: Returns the internal Element structure associated with a 
*       specific key in the map. I
*       iterator points to the previous element after this operation
*
* @iterator - after this operation, iterator points to the before 
*        the element need to be found. 
*        if the element doesn't exist, iterator will point to the element,
*        right before where it would have been if it will exist.
* @param map - The map for which to get the data element from.
* @param keyElement - The key element which need to be found.
* @return
*  NULL - if a NULL pointer was sent or if the map does not contain 
      the requested key.
*  Pointer to the internal element structure associated with the key otherwise.
*/
static Element mapFind(Map map, MapKeyElement key) {
  Element previous_element = NULL;
  Element iterator = map->first_element;
  while (iterator != NULL && map->compareKeys(iterator->key, key) == -1) {
    previous_element = iterator;
    iterator = iterator->next;
  }
  map->iterator = previous_element;
  if (iterator != NULL && map->compareKeys(iterator->key, key) == 0) {
    return iterator;
  }
  return NULL;
}



Map mapCreate(copyMapKeyElements copyKeyElement,
  copyMapDataElements copyDataElement,
  freeMapKeyElements freeKeyElement,
  freeMapDataElements freeDataElement,
  compareMapKeyElements compareKeyElements)
{
  if (NULL == copyKeyElement || NULL == copyDataElement ||
    NULL == freeKeyElement || NULL == freeDataElement ||
    NULL == compareKeyElements) {
    return NULL;
  }

  Map map = malloc(sizeof(*map));
  if (NULL == map) return NULL;

  map->copyKey = copyKeyElement;
  map->copyData = copyDataElement;
  map->freeKey = freeKeyElement;
  map->freeData = freeDataElement;
  map->compareKeys = compareKeyElements;
  map->first_element = NULL;
  map->iterator = NULL;
  map->iterator_is_undefined = false;

  return map;
}

void mapDestroy(Map map) {
  if (MAP_SUCCESS == mapClear(map)) {
    free(map);
  }
}

Map mapCopy(Map map) {
  if (NULL == map) return NULL;
  Map new_map = mapCreate(map->copyKey, map->copyData, 
    map->freeKey, map->freeData, map->compareKeys);
  if (NULL == new_map) return NULL;
  MapKeyElement element_to_add_key = mapGetFirst(map);
  while (element_to_add_key != NULL) {
    MapDataElement element_to_add_data = mapGet(map, element_to_add_key);
    if (mapPut(new_map, element_to_add_key, element_to_add_data) ==
      MAP_OUT_OF_MEMORY) {
      mapDestroy(new_map);
      return NULL;
    }
    element_to_add_key = mapGetNext(map);
  }
  return new_map;
}

int mapGetSize(Map map) {
  if (NULL == map) return -1;
  int size = 0;
  for (Element iterator = map->first_element; iterator != NULL; 
    iterator = iterator->next) {
    size++;
  }
  map->iterator_is_undefined = true;
  return size;
}

bool mapContains(Map map, MapKeyElement key) {
  if (NULL == map || NULL == key) return false;
  Element iterator = map->iterator; // save current iterator value
  Element element_found = mapFind(map, key);
  map->iterator = iterator; // restore iterator value
  map->iterator_is_undefined = true;
  if (NULL == element_found) return false;
  return true;
}

MapResult mapPut(Map map, MapKeyElement key, MapDataElement data) {
  if (NULL == map || NULL == key || NULL == data) return MAP_NULL_ARGUMENT;
  Element new_element = malloc(sizeof(*new_element));
  if (NULL == new_element) return MAP_OUT_OF_MEMORY;
  new_element->data = map->copyData(data);
  if (NULL == new_element->data) {
    free(new_element);
    return MAP_OUT_OF_MEMORY;
  }
  new_element->key = map->copyKey(key);
  if (NULL == new_element->key) {
    map->freeData(new_element->data);
    free(new_element);
    return MAP_OUT_OF_MEMORY;
  }
  if (mapContains(map, key)) { // if key already exist, override it
    mapRemove(map, key);
  }
  mapFind(map, key); //now iterator points to previous element (See mapFind)
  Element previous_element = map->iterator;
  if (previous_element != NULL) {
    new_element->next = previous_element->next;
    previous_element->next = new_element;
  }
  else {
    new_element->next = map->first_element;
    map->first_element = new_element;
  }
  map->iterator_is_undefined = true;
  return MAP_SUCCESS;
}

MapDataElement mapGet(Map map, MapKeyElement key) {
  if (NULL == map || NULL == key) return NULL;
  Element iterator = map->iterator;  // save current iterator value
  Element element_found = mapFind(map, key);
  map->iterator = iterator; // restore iterator value
  if (NULL == element_found) return NULL;
  return element_found->data;
}

MapResult mapRemove(Map map, MapKeyElement key) {
  if (NULL == map || NULL == key) return MAP_NULL_ARGUMENT;
  Element element_to_remove = mapFind(map, key);
  if (NULL == element_to_remove) return MAP_ITEM_DOES_NOT_EXIST;
  Element previous_element = map->iterator;
  if (previous_element != NULL) {
    previous_element->next = element_to_remove->next;
  }
  else {  // if removed element is the first one
    map->first_element = element_to_remove->next;
  }
  map->freeKey(element_to_remove->key);
  map->freeData(element_to_remove->data);
  free(element_to_remove);
  map->iterator_is_undefined = true;
  return MAP_SUCCESS;
}

MapKeyElement mapGetFirst(Map map) {
  if (NULL == map) return NULL;
  map->iterator = map->first_element;
  map->iterator_is_undefined = false;
  if (NULL == map->iterator) return NULL;
  return map->iterator->key;
}

MapKeyElement mapGetNext(Map map) {
  if (NULL == map) return NULL;
  if (map->iterator_is_undefined) return NULL;
  if (NULL == map->iterator) return NULL;
  map->iterator = map->iterator->next;
  if (NULL == map->iterator) return NULL;
  return map->iterator->key;
}

MapResult mapClear(Map map) {
  if (NULL == map) return MAP_NULL_ARGUMENT;
  MapKeyElement first_key = mapGetFirst(map);
  while (first_key != NULL) {
    mapRemove(map,first_key);
    first_key = mapGetFirst(map);
  }
  return MAP_SUCCESS;
}
