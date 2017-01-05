#include "../utils.h"
#include "test_utilities.h"

bool testStringCopy() {
  char* ret;
  char* string = "string!";

  // NULL input
  ret = stringCopy(NULL);
  ASSERT_TEST(NULL == ret);

  // Valid input
  ret = stringCopy(string);
  ASSERT_TEST(ret != NULL);

  free(ret);
  return true;
}

bool testStringCompare() {
  char *string1 = "string!";
  char *string2 = "strong!!";
  int ret;

  // string1 < string2
  ret = stringCompare(string1, string2);
  ASSERT_TEST(-1 == ret);

  // string1 > string2
  ret = stringCompare(string2, string1);
  ASSERT_TEST(1 == ret);

  // string1 == string2
  ret = stringCompare(string1, string1);
  ASSERT_TEST(0 == ret);

  return true;
}

bool testIntCompare() {
  int positive1 = 12, positive2 = 34, negative = -12, zero = 0;
  int ret;

  // two positives
  ret = intCompare(&positive1, &positive2);
  ASSERT_TEST(-1 == ret);
  ret = intCompare(&positive2, &positive1);
  ASSERT_TEST(1 == ret);

 // positive and negative
  ret = intCompare(&negative, &positive2);
  ASSERT_TEST(-1 == ret);
  ret = intCompare(&positive2, &negative);
  ASSERT_TEST(1 == ret);

  // positive and zero
  ret = intCompare(&zero, &positive2);
  ASSERT_TEST(-1 == ret);
  ret = intCompare(&positive2, &zero);
  ASSERT_TEST(1 == ret);

  // negative and zero
  ret = intCompare(&zero, &negative);
  ASSERT_TEST(1 == ret);
  ret = intCompare(&negative, &zero);
  ASSERT_TEST(-1 == ret);
  
  // equals
  ret = intCompare(&positive2, &positive2);
  ASSERT_TEST(0 == ret);
  ret = intCompare(&zero, &zero);
  ASSERT_TEST(0 == ret);
  ret = intCompare(&negative, &negative);
  ASSERT_TEST(0 == ret);
  
  return true;
}

bool testIntCopy() {
  int number = 2;
  int *copy;

  // NULL input
  copy = intCopy(NULL);
  ASSERT_TEST(NULL == copy);

  // Valid input
  copy = intCopy(&number);
  ASSERT_TEST(copy != NULL);

  free(copy);
  return true;
}

bool testIntDestroy() {
  int *number = malloc(sizeof(int));
  *number = 12;

  // NULL input
  intDestroy(NULL);

  // Valid input
  intDestroy(number);

  return true;
}

int main() {
  RUN_TEST(testStringCopy);
  RUN_TEST(testStringCompare);
  RUN_TEST(testIntCompare);
  RUN_TEST(testIntCopy);
  RUN_TEST(testIntDestroy);

 return 0;
}
