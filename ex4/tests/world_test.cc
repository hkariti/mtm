#include "test_utils.h"
#include "../world.h"
#include "../exceptions.h"
#include <string>
#include <set>
#include <fstream>
#include <sstream>

using namespace mtm::pokemongo;

bool WorldInput() {
	World world;

	// Empty Line
	std::istringstream input("");
	ASSERT_THROW(WorldInvalidInputLineException, input >> world);

	// Invalid location type
	std::istringstream input_2("HOME sweet PIKACHU");
	ASSERT_THROW(WorldInvalidInputLineException, input_2 >> world);
	
	// Gym Create
	// too many args
	std::istringstream input_3("GYM water_gym MISTY");
	ASSERT_THROW(WorldInvalidInputLineException, input_3 >> world);
	// no name
	std::istringstream input_4("GYM  ");
	ASSERT_THROW(WorldInvalidInputLineException, input_4 >> world);
	// success
	std::istringstream input_5("GYM water_gym");
	ASSERT_NO_THROW(input_5 >> world);
	
	// Pokestop Create
	std::istringstream input_6("POKESTOP 101_miles POTION 5 CHOCOLATE 3");
	ASSERT_THROW(WorldInvalidInputLineException, input_6 >> world);
	std::istringstream input_7("POKESTOP 101_miles POTION -5 CANDY 3");
	ASSERT_THROW(WorldInvalidInputLineException, input_7 >> world);
	std::istringstream input_8("POKESTOP 101_miles POTION 5 CANDY");
	ASSERT_THROW(WorldInvalidInputLineException, input_8 >> world);
	std::istringstream input_9("POKESTOP 101_miles POTION 5 CANDY 3 POTION 5 CANDY 3 POTION 5 CANDY 3 POTION 5 CANDY 3");
	ASSERT_NO_THROW(input_9 >> world);

	// Starbucks Create
	// not enough args
	std::istringstream input_10("STARBUCKS coffee_shop pikachu 2.5 3 pikachu 1 2 charmander 3.45");
	ASSERT_THROW(WorldInvalidInputLineException, input_10 >> world);
	// string as int
	std::istringstream input_11("STARBUCKS coffee_shop pikachu 2.5 3 pikachu 1 2 123 3.45 4 3");
	ASSERT_THROW(WorldInvalidInputLineException, input_11 >> world);
	// int as string
	std::istringstream input_12("STARBUCKS coffee_shop pikachu 2.5 3 pikachu 1 2 charmander squirtle 4 3");
	ASSERT_THROW(WorldInvalidInputLineException, input_12 >> world);
	// success
	std::istringstream input_13("STARBUCKS coffee_shop pikachu 2.5 3 pikachu 1 2 charmander 3.45 4");
	ASSERT_NO_THROW(input_13 >> world);

	// location already exist
	std::istringstream input_14("GYM coffee_shop");
	ASSERT_THROW(WorldLocationNameAlreadyUsed, input_14 >> world);

    return true;
}

bool WorldRemove() {
	World world;
    // Remove location
	std::istringstream input_15("GYM coffee_shop1");
	ASSERT_NO_THROW(input_15 >> world);
    ASSERT_NO_THROW(world.Remove("coffee_shop1"));

	return true;
}
