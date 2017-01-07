#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>

/**
 * Common functions in use by several modules
 *
 * Available functions:
 * stringCopy   - Return a copy of a string
 * stringCompare    - Compare strings (like strcmp)
 * intCompare       - Compare ints, taking int* as arguments
 * intCopy          - Copy an int, taking int*
 * intDestroy       - Free memory taken by an int
 */

/**
 * stringCopy - Copy a string
 *
 * @param string - String to be copied
 *
 * @return
 *  NULL - NULL arg
 *  A new allocated string which is a copy of the original on success
 */
char* stringCopy(char* string);

/**
 * stringCompare - Compare strings
 *
 * @param str1 - First string
 * @param str2 - Second string
 *
 * @return
 *  -1  - str1 < str2
 *  0   - str1 == str2
 *  1   - str1 > str2
 */
int stringCompare(char* str1, char* str2);

/**
 * intCompare   - Compare ints
 *
 * @param int1 - First int
 * @param int2 - Second int
 *
 * @return
 *  -1  - int1 < int2
 *  0   - int1 == int2
 *  1   - int1 > int2
 */
int intCompare(int* a, int* b);

/**
 * intCopy - Copy an int
 *
 * @param key - pointer to int to copy
 *
 * @return
 *  NULL - NULL arg
 *  An allocated int* which points to a copy of the original on success
 */
int* intCopy(int *key);

/**
 * intDestroy - Deallocate an int
 *
 * @param key - pointer to int to deallocate
 *
 * If key is NULL nothing will be done
 */
void intDestroy(int *key);
#endif
