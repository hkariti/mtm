#include "test_utils.h"
#include "../trainer.h"
#include "../exceptions.h"
#include <string>
#include <set>

using namespace mtm::pokemongo;

bool testTrainerCtors() {

	// invalid name
	ASSERT_THROW(TrainerInvalidArgsException, Trainer("", RED));
	// success
	ASSERT_NO_THROW(Trainer("Tomer", BLUE));

	// Check copy ctor
	Trainer trainee = Trainer("tomer", RED);
	ASSERT_NO_THROW(Trainer(trainee));

	return true;
}


bool testAssignment() {

	// test assignment operator
	Trainer trainer = Trainer("Ash", BLUE);
	Trainer trainer_2 = Trainer("Gary", BLUE);
	ASSERT_NO_THROW(trainer_2 = trainer);

	return true;
}

bool testGetStrongestPokemon() {
	Trainer ash = Trainer("ash", RED);
	Pokemon pikachu = Pokemon("pikachu", 10, 1);
	Pokemon mew = Pokemon("mew", 5, 1);
	Pokemon mewtwo = Pokemon("mewtwo", 30, 1);

	ash.TryToCatch(pikachu);
	ash.TryToCatch(mew);
	ash.TryToCatch(mewtwo);
	
	ASSERT_TRUE(ash.GetStrongestPokemon() == mewtwo);

	return true;
}

bool testKillStrongestPokemon() {
	Trainer ash = Trainer("ash", RED);
	Pokemon pikachu = Pokemon("pikachu", 10, 1);
	Pokemon mew = Pokemon("mew", 5, 1);
	Pokemon mewtwo = Pokemon("mewtwo", 30, 1);

	ash.TryToCatch(pikachu);
	ash.TryToCatch(mew);
	ash.TryToCatch(mewtwo);

	// kill all pokemons and check kill order
	ASSERT_TRUE(ash.GetStrongestPokemon() == mewtwo);
	ASSERT_NO_THROW(ash.KillStrongestPokemon());
	ASSERT_TRUE(ash.GetStrongestPokemon() == pikachu);
	ASSERT_NO_THROW(ash.KillStrongestPokemon());
	ASSERT_TRUE(ash.GetStrongestPokemon() == mew);
	ASSERT_NO_THROW(ash.KillStrongestPokemon());
	ASSERT_THROW(TrainerNoPokemonsFoundException, ash.KillStrongestPokemon());


	return true;
}

bool testCompareisonOperators() {

	// trainers with no pokemons
	Trainer ash = Trainer("ash", RED);
	Trainer gary = Trainer("gary", RED);
	ASSERT_FALSE(ash > gary);
	ASSERT_FALSE(ash < gary);
	ASSERT_FALSE(ash != gary);
	ASSERT_TRUE(ash == gary);
	ASSERT_TRUE(ash >= gary);
	ASSERT_TRUE(ash <= gary);

	// one trainer with pokemons
	Pokemon pikachu = Pokemon("pikachu", 10, 1);
	Pokemon mew = Pokemon("mew", 5, 1);
	ash.TryToCatch(pikachu);
	ASSERT_TRUE(ash > gary);
	ASSERT_FALSE(ash < gary);
	ASSERT_TRUE(ash != gary);
	ASSERT_FALSE(ash == gary);
	ASSERT_TRUE(ash >= gary);
	ASSERT_FALSE(ash <= gary);

	// trainers with not equal pokemons
	gary.TryToCatch(mew);
	ASSERT_TRUE(ash > gary);
	ASSERT_FALSE(ash < gary);
	ASSERT_TRUE(ash != gary);
	ASSERT_FALSE(ash == gary);
	ASSERT_TRUE(ash >= gary);
	ASSERT_FALSE(ash <= gary);

	// trainers with equal pokemons
	gary.TryToCatch(pikachu);
	ASSERT_FALSE(ash > gary);
	ASSERT_FALSE(ash < gary);
	ASSERT_FALSE(ash != gary);
	ASSERT_TRUE(ash == gary);
	ASSERT_TRUE(ash >= gary);
	ASSERT_TRUE(ash <= gary);

	return true;
}

bool testIsAlly() {
	Trainer ash = Trainer("ash", RED);
	Trainer gary = Trainer("gary", BLUE);
	Trainer misty = Trainer("gary", RED);

	// test failure
	ASSERT_FALSE(ash.IsAlly(gary));

	// test success
	ASSERT_TRUE(ash.IsAlly(misty));

	return true;
}
bool testTrain() {

	// test assignment operator
	Pokemon pokemon = Pokemon(std::string("pikachu"), 5, 5);
	ASSERT_THROW(PokemonInvalidArgsException, pokemon.Train(-1));
	ASSERT_THROW(PokemonInvalidArgsException, pokemon.Train(0.5));
	ASSERT_NO_THROW(pokemon.Train(4));

	return true;
}

bool testTryToCatch() {
	Trainer ash = Trainer("ash", RED);
	Pokemon pikachu = Pokemon("pikachu", 10, 1);
	Pokemon mewtwo = Pokemon("mewtwo", 30, 3);

	ash.TryToCatch(pikachu);
	ash.TryToCatch(mewtwo);

	// try to catch higher level pokemon
	ASSERT_FALSE(ash.TryToCatch(mewtwo));

	// catch success
	ASSERT_TRUE(ash.TryToCatch(pikachu));
	ASSERT_NO_THROW(ash.KillStrongestPokemon());
	ASSERT_NO_THROW(ash.KillStrongestPokemon());
	ASSERT_THROW(TrainerNoPokemonsFoundException, ash.KillStrongestPokemon());

	return true;
}

bool testTrainerBattle() {
	std::set<PokemonType> types;
	types.insert(NORMAL);
	Trainer ash = Trainer("ash", RED);
	Trainer gary = Trainer("gary", YELLOW);
	Trainer misty = Trainer("misty", BLUE);
	Pokemon pikachu = Pokemon("pikachu", types, 3, 1);
	Pokemon mewtwo = Pokemon("mewtwo", types, 1000, 1);

	// test trainers have no pokemons, level doesn't change
	ASSERT_EQUAL(&ash, TrainersBattle(ash, gary));
	ASSERT_EQUAL(3, ash.TotalScore());
	ASSERT_EQUAL(0, gary.TotalScore());

	// test one trainer has pokemons, test trainer level & battle score
	ash.TryToCatch(pikachu);
	ASSERT_EQUAL(&ash, TrainersBattle(ash, gary));
	ASSERT_EQUAL(6, ash.TotalScore());
	ASSERT_EQUAL(-1, gary.TotalScore());
	ASSERT_EQUAL(&ash, TrainersBattle(gary, ash));
	ASSERT_EQUAL(9, ash.TotalScore());
	ASSERT_EQUAL(-2, gary.TotalScore());

	// test pokemons equal, only team color matter 
	gary.TryToCatch(pikachu);
	misty.TryToCatch(pikachu);
	ASSERT_EQUAL(&gary, TrainersBattle(gary, misty));
	ASSERT_EQUAL(&gary, TrainersBattle(misty, gary));
	ASSERT_EQUAL(&ash, TrainersBattle(ash, gary));
	ASSERT_EQUAL(&ash, TrainersBattle(gary, ash));
	ASSERT_EQUAL(&misty, TrainersBattle(ash, misty));
	ASSERT_EQUAL(&misty, TrainersBattle(misty, ash));

	// test pokemons not equal 
	gary.TryToCatch(mewtwo);
	ASSERT_EQUAL(&gary, TrainersBattle(gary, ash));
	ASSERT_EQUAL(&gary, TrainersBattle(ash, gary));

	// test pokemon is dead
	ASSERT_THROW(TrainerNoPokemonsFoundException, ash.GetStrongestPokemon());

	return true;
}
int main() {
  RUN_TEST(testTrainerCtors);
  RUN_TEST(testAssignment);
  RUN_TEST(testCompareisonOperators);
  RUN_TEST(testIsAlly);
  RUN_TEST(testGetStrongestPokemon);
  RUN_TEST(testKillStrongestPokemon);
  RUN_TEST(testTryToCatch);
  RUN_TEST(testTrainerBattle);
  return 0;
}

