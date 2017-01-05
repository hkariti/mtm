#include "utils.h"

char* stringCopy(char* string) {
  if (NULL == string) return NULL;
  int length = (int)strlen(string);
  char* string_copy = malloc(length + 1);
  if (NULL == string_copy) return NULL;
  strcpy(string_copy, string);
  return string_copy;
}

int stringCompare(char* str1, char* str2) {
  int cmp = strcmp((char*)str1, (char*)str2);
  if (cmp > 0) return 1;
  if (cmp < 0) return -1;
  return 0;
}

int intCompare(int* a, int* b) {
  int cmp = *a - *b;
  if (cmp > 0) return 1;
  if (cmp < 0) return -1;
  return 0;
}

int* intCopy(int *key) {
  if (NULL == key) return NULL;
  int *key_copy;
  key_copy = malloc(sizeof(int));
  if (NULL == key_copy) return NULL;
  *key_copy = *key;
  return key_copy;
}

void intDestroy(int *key) {
  if (NULL == key) return;
  free(key);
}
