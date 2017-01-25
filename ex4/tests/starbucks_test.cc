#include "test_utils.h"
#include "../trainer.h"
#include "../pokemon.h"
#include "../starbucks.h"
#include "../exceptions.h"
#include <string>
#include <vector>

using namespace mtm::pokemongo;

Starbucks* createStarbucks() {
	std::vector<Pokemon> pokemons;
	pokemons.push_back(Pokemon("pikachu", 3.5, 1));
	pokemons.push_back(Pokemon("charmander", 2.1, 1));
	pokemons.push_back(Pokemon("charizard", 100.3, 2));
	pokemons.push_back(Pokemon("dragonite", 4, 3));
	return new Starbucks(pokemons);
}

bool testArrive() {

	Starbucks* starbucks = createStarbucks();
	Trainer ash("ash", BLUE);
	Trainer gary("gary", RED);
	Trainer oak("oak", RED);

	
	// catch first pokemon in site
	ASSERT_NO_THROW(starbucks->Arrive(ash));
	ASSERT_NO_THROW(ash.GetStrongestPokemon());

	// trainer already in site
	ASSERT_THROW(LocationTrainerAlreadyInLocationException,
		starbucks->Arrive(ash));

	ASSERT_NO_THROW(starbucks->Arrive(oak));
	// can't catch higher level pokemon
	ASSERT_NO_THROW(starbucks->Arrive(gary));
	ASSERT_THROW(TrainerNoPokemonsFoundException, gary.GetStrongestPokemon());

	delete starbucks;
	return true;
}

bool testLeave() {

	Starbucks* starbucks = createStarbucks();
	Trainer ash("ash", BLUE);
	Trainer gary("gary", RED);

	// trainer not found
	ASSERT_THROW(LocationTrainerNotFoundException, starbucks->Leave(ash));

	// leave success
	ASSERT_NO_THROW(starbucks->Arrive(ash));
	ASSERT_NO_THROW(starbucks->Leave(ash));
	ASSERT_NO_THROW(starbucks->Arrive(ash));

	delete starbucks;
	return true;
}


int main() {
	RUN_TEST(testArrive);
	RUN_TEST(testLeave);
	return 0;
}

