#include "../location.h"
#include "../utils.h"
#include "../pokemon.h"
#include "mocks/evolutions_mocks.h"
#include "mocks/pokedex_mocks.h"
#include "mocks/location_mocks.h"
#include "mocks/pokemon_mocks.h"
#include "test_utilities.h"

bool testCreateLocation() {
  Location location;
  // NULL input
  location = locationCreate(NULL);
  ASSERT_TEST(NULL == location);

  // valid input
  location = locationCreate("Hong Kong");
  ASSERT_TEST(NULL != location);

  locationDestroy(location);
  return true;
}

bool testDestroyLocation() {
  LOCATION_SET_UP();

  // NULL Input
  locationDestroy(NULL);

  // Valid input
  Location loc = demoLocationWithPokemon("Eilat", 1, pokedex, evolutions);
  locationDestroy(loc);

  LOCATION_TEAR_DOWN();
  return true;
}

bool testCopyLocation() {
  LOCATION_SET_UP();

  Location empty_location = demoLocationWithPokemon("Eilat", 0, pokedex,
                                                      evolutions);
  Location non_empty_location = demoLocationWithPokemon("Eilat", 5, pokedex,
                                                          evolutions);

  // NULL input
  Location location_copy = locationCopy(NULL);
  ASSERT_TEST(NULL == location_copy);

  // Valid input
  location_copy = locationCopy(non_empty_location);
  ASSERT_TEST(location_copy != NULL);
  locationDestroy(location_copy);

  location_copy = locationCopy(empty_location);
  ASSERT_TEST(location_copy != NULL);
  locationDestroy(location_copy);

  locationDestroy(empty_location);
  locationDestroy(non_empty_location);
  LOCATION_TEAR_DOWN();
  return true;
}

bool testPrintLocation() {
  LOCATION_SET_UP();

  locationPrint(location, stdout);

  LOCATION_TEAR_DOWN();
  return true;
}

bool testLocationCompare() {
  LOCATION_SET_UP();
  int result;

  Location eilat_1 = demoLocationWithPokemon("Eilat", 0, pokedex, evolutions);
  Location tel_aviv = demoLocationWithPokemon("Tel Aviv", 0, pokedex,
                                                evolutions);
  Location eilat_2 = demoLocationWithPokemon("Eilat", 0, pokedex, evolutions);

  result = locationCompare(eilat_1, eilat_2);
  ASSERT_TEST(0 == result);
  result = locationCompare(tel_aviv, eilat_2);
  ASSERT_TEST(0 < result);
  result = locationCompare(eilat_1, tel_aviv);
  ASSERT_TEST(0 > result);

  locationDestroy(eilat_1);
  locationDestroy(tel_aviv);
  locationDestroy(eilat_2);

  LOCATION_TEAR_DOWN();
  return true;
}


bool testLocationGetName() {
  LOCATION_SET_UP();

  Location palm = demoLocationWithPokemon("Palem", 2, pokedex, evolutions);

  // Invalid input
  char* name = locationGetName(NULL);
  ASSERT_TEST(NULL == name);

  // Valid input
  name = locationGetName(palm);
  int cmp = strcmp(name, "Palem");
  ASSERT_TEST(cmp == 0);

  locationDestroy(palm);
  LOCATION_TEAR_DOWN();
  return true;
}

bool testLocationAppendPokemon() {
  LOCATION_SET_UP();

  Location empty_location = demoLocationWithPokemon("Eilat", 0, pokedex,
                                                      evolutions);
  Pokemon pokemon = demoPokemon(pokedex, evolutions);
  LocationErrorCode result;
  bool is_empty;

  // Invalid input
  result = locationAppendPokemon(NULL, NULL);
  ASSERT_TEST(LOCATION_INVALID_ARGUMENT == result);
  result = locationAppendPokemon(location, NULL);
  ASSERT_TEST(LOCATION_INVALID_ARGUMENT == result);
  result = locationAppendPokemon(NULL, pokemon);
  ASSERT_TEST(LOCATION_INVALID_ARGUMENT == result);

  // Valid input - not empty location
  result = locationAppendPokemon(location, pokemon);
  ASSERT_TEST(LOCATION_SUCCESS == result);

  // valid input - empty location
  is_empty = locationIsEmpty(empty_location);
  ASSERT_TEST(true == is_empty);
  result = locationAppendPokemon(empty_location, pokemon);
  ASSERT_TEST(LOCATION_SUCCESS == result);
  is_empty = locationIsEmpty(empty_location);
  ASSERT_TEST(false == is_empty);

  pokemonDestroy(pokemon);
  locationDestroy(empty_location);

  LOCATION_TEAR_DOWN();
  return true;
}


bool testLocationPopPokemon() {
  LOCATION_SET_UP();

  Location one_pokemon_location;
  Pokemon poppedPokemon;
    one_pokemon_location = demoLocationWithPokemon("Eilat", 1, pokedex,
                                                   evolutions);
  // Invalid input
  poppedPokemon = locationPopPokemon(NULL);
  ASSERT_TEST(NULL == poppedPokemon);

  // non empty location
  poppedPokemon = locationPopPokemon(one_pokemon_location);
  ASSERT_TEST(NULL != poppedPokemon);
  pokemonDestroy(poppedPokemon);

  // empty location
  poppedPokemon = locationPopPokemon(one_pokemon_location);
  ASSERT_TEST(NULL == poppedPokemon);

  locationDestroy(one_pokemon_location);
  LOCATION_TEAR_DOWN();
  return true;
}

bool testLocationIsEmpty() {
  LOCATION_SET_UP();

  bool result;
  Location empty_location = demoLocationWithPokemon("Eilat", 0, pokedex,
                                                      evolutions);

  // empty location
  result = locationIsEmpty(empty_location);
  ASSERT_TEST(true == result);

  // non empty location
  result = locationIsEmpty(location);
  ASSERT_TEST(false == result);

  locationDestroy(empty_location);
  LOCATION_TEAR_DOWN();
  return true;
}

bool testLocationIsNeighbour() {
  LOCATION_SET_UP();

  bool result;
  Location eilat = demoLocationWithPokemon("Eilat", 0, pokedex, evolutions);
  Location beersheva = demoLocationWithPokemon("Beer Sheva", 0, pokedex,
                                                 evolutions);
  Location medellin = demoLocationWithPokemon("Medellin", 0, pokedex,
                                                evolutions);

  locationAddNeighbor(eilat, "Beer Sheva");
  locationAddNeighbor(beersheva, "Eilat");

  // not neighbours
  result = locationIsNeighour(eilat, medellin);
  ASSERT_TEST(false == result);

  // neighbours
  result = locationIsNeighour(eilat, beersheva);
  ASSERT_TEST(true == result);

  locationDestroy(eilat);
  locationDestroy(beersheva);
  locationDestroy(medellin);
  LOCATION_TEAR_DOWN();
  return true;
}

// errors: LOCATION_INVALID_ARGUMENT, LOCATION_OUT_OF_MEMORY
LocationErrorCode locationAddNeighbor(Location location, char* neighbour_name);

bool testLocationAddNeighbor() {
  LOCATION_SET_UP();

  Location eilat = demoLocationWithPokemon("Eilat", 0, pokedex, evolutions);
  Location beersheva = demoLocationWithPokemon("Beer Sheva", 0, pokedex,
                                                 evolutions);
  LocationErrorCode result;
  bool is_neighbour;

  // Invalid argument
  result = locationAddNeighbor(NULL, NULL);
  ASSERT_TEST(LOCATION_INVALID_ARGUMENT == result);
  result = locationAddNeighbor(NULL, "eilat");
  ASSERT_TEST(LOCATION_INVALID_ARGUMENT == result);
  result = locationAddNeighbor(eilat, NULL);
  ASSERT_TEST(LOCATION_INVALID_ARGUMENT == result);

  // add neighbours
  is_neighbour = locationIsNeighour(eilat, beersheva);
  ASSERT_TEST(false == is_neighbour);
  result = locationAddNeighbor(eilat, "Beer Sheva");
  ASSERT_TEST(LOCATION_SUCCESS == result);
  is_neighbour = locationIsNeighour(eilat, beersheva);
  ASSERT_TEST(true == is_neighbour);

  locationDestroy(eilat);
  locationDestroy(beersheva);
  LOCATION_TEAR_DOWN();
  return true;
}

int main() {
  RUN_TEST(testCreateLocation);
  RUN_TEST(testDestroyLocation);
  RUN_TEST(testCopyLocation);
  RUN_TEST(testPrintLocation);
  RUN_TEST(testLocationCompare);
  RUN_TEST(testLocationGetName);
  RUN_TEST(testLocationAppendPokemon);
  RUN_TEST(testLocationPopPokemon);
  RUN_TEST(testLocationIsEmpty);
  RUN_TEST(testLocationIsNeighbour);
  RUN_TEST(testLocationAddNeighbor);
  return 0;
}
