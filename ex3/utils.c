#include "utils.h"

char* stringCopy(char* string) {
  if (NULL == string) return NULL;
  int length = strlen(string);
  char* string_copy = malloc(length + 1);
  if (NULL == string_copy) return NULL;
  strcpy(string_copy, string);
  return string_copy;
}

int intCompare(int* a, int* b) {
  return *a - *b;
}

int* intCopy(int *key) {
  int *key_copy;
  key_copy = malloc(sizeof(int));
  if (NULL == key_copy) return NULL;
  *key_copy = *key;
  return key_copy;
}

void intDestroy(int *key) {
  free(key);
}
