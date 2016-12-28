#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>

char* stringCopy(char* string);
int intCompare(int* a, int* b);
int* intCopy(int *key);
void intDestroy(int *key);
#endif
