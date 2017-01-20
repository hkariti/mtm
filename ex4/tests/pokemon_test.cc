#include "test_utils.h"
#include "../pokemon.h"
#include "../exceptions.h"
#include <string>
#include <set>

using namespace mtm::pokemongo;

bool testPokemonCtors() {

	// Check Default types ctor
	// invalid species
	ASSERT_THROW(PokemonInvalidArgsException, Pokemon("", 5, 5));
	// invalid cp
	ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", 0, 5));
	// invalid level
	ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", 5, 0));
	// success
	ASSERT_NO_THROW(Pokemon("pikachu", 5, 5));


	// Check customized types ctor
	std::set<PokemonType> types = std::set<PokemonType>();
	types.insert(POISON);

	// invalid species
	ASSERT_THROW(PokemonInvalidArgsException, Pokemon("", types, 5, 5));
	// invalid cp
	ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", types, 0, 5));
	// invalid level
	ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", types, 5, 0));
	// success
	ASSERT_NO_THROW(Pokemon("pikachu", types, 5, 5));

	// Check copy ctor
	Pokemon pokemon = Pokemon("pikachu", types, 5, 5);
	ASSERT_NO_THROW(Pokemon(pokemon));

	return true;
}


bool testAssignment() {

	// test assignment operator
	Pokemon pokemon("pikachu", 5, 5);
	Pokemon pokemon_2("pikachu", 5, 5);
	ASSERT_NO_THROW(pokemon_2 = pokemon);

	return true;
}

bool testCompareisonOperators() {

	// hit power no equal
	Pokemon pikachu("pikachu", 10, 5);
	Pokemon mew("mew", 5, 5);
	ASSERT_TRUE(pikachu > mew);
	ASSERT_TRUE(pikachu >= mew);
	ASSERT_TRUE(pikachu != mew);
	ASSERT_FALSE(pikachu == mew);
	ASSERT_FALSE(pikachu <= mew);
	ASSERT_FALSE(pikachu < mew);

	// hit power equal, types not same
	std::set<PokemonType> high_value_types;
	high_value_types.insert(POISON);
	high_value_types.insert(GHOST);
	std::set<PokemonType> low_value_types;
	low_value_types.insert(NORMAL);

	pikachu = Pokemon("pikachu", high_value_types, 5, 5);
	mew = Pokemon("mew", low_value_types, 5, 5);
	ASSERT_TRUE(pikachu > mew);
	ASSERT_TRUE(pikachu >= mew);
	ASSERT_TRUE(pikachu != mew);
	ASSERT_FALSE(pikachu == mew);
	ASSERT_FALSE(pikachu <= mew);
	ASSERT_FALSE(pikachu < mew);

	// equal pokemons
	pikachu = Pokemon("pikachu", low_value_types, 5, 5);
	mew = Pokemon("mew", low_value_types, 5, 5);
	ASSERT_FALSE(pikachu > mew);
	ASSERT_TRUE(pikachu >= mew);
	ASSERT_FALSE(pikachu != mew);
	ASSERT_TRUE(pikachu == mew);
	ASSERT_TRUE(pikachu <= mew);
	ASSERT_FALSE(pikachu < mew);

	return true;
}

bool testLevel() {

	// test success
	Pokemon pokemon("pikachu", 5, 5);
	ASSERT_TRUE(5 == pokemon.Level());

	return true;
}
bool testTrain() {

	// test assignment operator
	Pokemon pokemon("pikachu", 5, 5);
	ASSERT_THROW(PokemonInvalidArgsException, pokemon.Train(-1));
	ASSERT_THROW(PokemonInvalidArgsException, pokemon.Train(0.5));
	ASSERT_NO_THROW(pokemon.Train(4));

	return true;
}


int main() {
  RUN_TEST(testPokemonCtors);
  RUN_TEST(testAssignment);
  RUN_TEST(testCompareisonOperators);
  RUN_TEST(testLevel);
  RUN_TEST(testTrain);

  return 0;
}

