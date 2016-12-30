#include "../evolutions.h"
#include "mocks/evolutions_mocks.h"
#include "test_utilities.h"


bool testCreateEvolutions() {
  Evolutions evolutions = createEvolutions();

  ASSERT_TEST(evolutions != NULL);
  destroyEvolutions(evolutions);
  return true;
}

bool testDestroyEvolutions() {
  // Test with NULL
  destroyEvolutions(NULL);

  Evolutions evolutions;
  // Test with empty
  evolutions = createEvolutions();
  destroyEvolutions(evolutions);

  // Test with entries
  evolutions = demoEvolutions();
  destroyEvolutions(evolutions);

  return true;
}

bool testEvolutionsAddEntry() {
  Evolutions evolutions = createEvolutions();
  EvolutionsErrorCode add_result;

  // Test NULL params
  add_result = evolutionsAddEntry(NULL, "pikachu", 1, (PokedexEntry)0xdeadbeef);
  ASSERT_TEST(EVOLUTIONS_INVALID_ARGUMENT == add_result);
  add_result = evolutionsAddEntry(evolutions, NULL, 1, (PokedexEntry)0xdeadbeef);
  ASSERT_TEST(EVOLUTIONS_INVALID_ARGUMENT == add_result);
  add_result = evolutionsAddEntry(evolutions, "pikachu", 1, NULL);
  ASSERT_TEST(EVOLUTIONS_INVALID_ARGUMENT == add_result);

  // Test new entry
  add_result = evolutionsAddEntry(evolutions, "pikachu", 1, (PokedexEntry)0xdeadbeef);
  ASSERT_TEST(EVOLUTIONS_SUCCESS == add_result);

  // Test existing entry
  add_result = evolutionsAddEntry(evolutions, "pikachu", 1, (PokedexEntry)0xdeadbeef);
  ASSERT_TEST(EVOLUTIONS_SUCCESS == add_result);
  add_result = evolutionsAddEntry(evolutions, "pikachu", 2, (PokedexEntry)0x1337);
  ASSERT_TEST(EVOLUTIONS_SUCCESS == add_result);

  destroyEvolutions(evolutions);
  return true;
}

bool testGetEvolution() {
  EVOLUTIONS_SET_UP();
  PokedexEntry ret;

  // Test NULL params
  ret = getEvolution(NULL, "pikachu", 1);
  ASSERT_TEST(NULL == ret);
  ret = getEvolution(evolutions, NULL, 1);
  ASSERT_TEST(NULL == ret);

  // Test level is too low (pikachu evolves at level 2 in our example)
  ret = getEvolution(evolutions, "pikachu", 0);
  ASSERT_TEST(NULL == ret);
  ret = getEvolution(evolutions, "pikachu", 1);
  ASSERT_TEST(NULL == ret);

  // Test level is equals or higher
  ret = getEvolution(evolutions, "pikachu", 2);
  ASSERT_TEST(NULL != ret);
  ret = getEvolution(evolutions, "pikachu", 3);
  ASSERT_TEST(NULL != ret);

  // Test after update
  evolutionsAddEntry(evolutions, "pikachu", 1, (PokedexEntry)0x1337);
  ret = getEvolution(evolutions, "pikachu", 1);
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
