#include "../evolutions.h"
#include "mocks/evolutions_mocks.h"
#include "test_utilities.h"


bool testCreateEvolutions() {
  Evolutions evolutions = evolutionsCreate();

  ASSERT_TEST(evolutions != NULL);
  evolutionsDestroy(evolutions);
  return true;
}

bool testDestroyEvolutions() {
  // Test with NULL
  evolutionsDestroy(NULL);

  Evolutions empty_evolutions;
  // Test with empty
  empty_evolutions = evolutionsCreate();
  evolutionsDestroy(empty_evolutions);

  // Test with entries
  EVOLUTIONS_SET_UP();
  EVOLUTIONS_TEAR_DOWN();

  return true;
}

bool testEvolutionsAddEntry() {
  Evolutions evolutions = evolutionsCreate();
  EvolutionsErrorCode add_result;

  // Test NULL params
  add_result = evolutionsAddEntry(NULL, "pikachu", 1, (PokedexEntry)0xdeadbeef);
  ASSERT_TEST(EVOLUTIONS_INVALID_ARGUMENT == add_result);
  add_result = evolutionsAddEntry(evolutions, NULL, 1,
                                  (PokedexEntry)0xdeadbeef);
  ASSERT_TEST(EVOLUTIONS_INVALID_ARGUMENT == add_result);
  add_result = evolutionsAddEntry(evolutions, "pikachu", 1, NULL);
  ASSERT_TEST(EVOLUTIONS_INVALID_ARGUMENT == add_result);

  // Test new entry
  add_result = evolutionsAddEntry(evolutions, "pikachu", 1,
                                  (PokedexEntry)0xdeadbeef);
  ASSERT_TEST(EVOLUTIONS_SUCCESS == add_result);

  // Test existing entry
  add_result = evolutionsAddEntry(evolutions, "pikachu", 1,
                                  (PokedexEntry)0xdeadbeef);
  ASSERT_TEST(EVOLUTIONS_SUCCESS == add_result);
  add_result = evolutionsAddEntry(evolutions, "pikachu", 2,
                                  (PokedexEntry)0x1337);
  ASSERT_TEST(EVOLUTIONS_SUCCESS == add_result);

  evolutionsDestroy(evolutions);
  return true;
}

bool testGetEvolution() {
  EVOLUTIONS_SET_UP();
  PokedexEntry ret;

  // Test NULL params
  ret = evolutionsGet(NULL, "pikachu", 1);
  ASSERT_TEST(NULL == ret);
  ret = evolutionsGet(evolutions, NULL, 1);
  ASSERT_TEST(NULL == ret);

  // Test level is too low (pikachu evolves at level 2 in our example)
  ret = evolutionsGet(evolutions, "pikachu", 0);
  ASSERT_TEST(NULL == ret);
  ret = evolutionsGet(evolutions, "pikachu", 1);
  ASSERT_TEST(NULL == ret);

  // Test level is equals or higher
  ret = evolutionsGet(evolutions, "pikachu", 2);
  ASSERT_TEST(NULL != ret);
  ret = evolutionsGet(evolutions, "pikachu", 3);
  ASSERT_TEST(NULL != ret);

  // Test after update
  evolutionsAddEntry(evolutions, "pikachu", 1, (PokedexEntry)0x1337);
  ret = evolutionsGet(evolutions, "pikachu", 1);
  ASSERT_TEST(NULL != ret);

  EVOLUTIONS_TEAR_DOWN();
  return true;
}

int main() {
  RUN_TEST(testCreateEvolutions);
  RUN_TEST(testDestroyEvolutions);
  RUN_TEST(testEvolutionsAddEntry);
  RUN_TEST(testGetEvolution);

  return 0;
}
