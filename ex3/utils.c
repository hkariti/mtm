#include <utils.h>

char* stringCopy(char* string) {
  if (NULL == string) return NULL;
  int length = strlen(string);
  char* string_copy = malloc(length + 1);
  if (NULL == string_copy) return NULL;
  strcpy(string_copy, string);
  return string_copy;
}
