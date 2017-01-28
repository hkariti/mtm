#include "test_utils.h"
#include "../trainer.h"
#include "../gym.h"
#include "../exceptions.h"
#include <string>
#include <set>

using namespace mtm::pokemongo;

bool testGymArrive() {

	Trainer blue_1("ash", BLUE);
	Trainer red("gary", RED);
	Trainer blue_2("ashh", BLUE);
	Trainer yellow("yel", YELLOW);


	Gym gym;

	// arriving to gym and being leader
	ASSERT_NO_THROW(gym.Arrive(blue_1));
	ASSERT_EQUAL(blue_1.TotalScore(), 11);

	// already in location
	ASSERT_THROW(LocationTrainerAlreadyInLocationException, gym.Arrive(blue_1));

	// already in location
	ASSERT_THROW(LocationTrainerAlreadyInLocationException, gym.Arrive(blue_1));

	// battle between trainers
	ASSERT_NO_THROW(gym.Arrive(red));
	ASSERT_EQUAL(blue_1.TotalScore(), 13);
	ASSERT_EQUAL(blue_1.TotalScore(), 13);

	// enter new same-team as leader trainer
	ASSERT_NO_THROW(gym.Arrive(blue_2));
	ASSERT_EQUAL(blue_1.TotalScore(), 13);
	ASSERT_EQUAL(red.TotalScore(), 0);
	ASSERT_EQUAL(blue_2.TotalScore(), 1);

	// get prefered trainer in team
	ASSERT_NO_THROW(gym.Arrive(yellow));
	ASSERT_EQUAL(blue_1.TotalScore(), 2);
	ASSERT_EQUAL(red.TotalScore(), 0);
	ASSERT_EQUAL(blue_2.TotalScore(), 1);
	ASSERT_EQUAL(yellow.TotalScore(), 13);
	ASSERT_NO_THROW(gym.Leave(red));
	ASSERT_NO_THROW(gym.Leave(yellow));
	ASSERT_EQUAL(blue_1.TotalScore(), 12);
	ASSERT_EQUAL(blue_2.TotalScore(), 1);

	return true;
}


bool testGymLeave() {

	Trainer blue_1("ash", BLUE);
	Trainer red("gary", RED);
	Trainer blue_2("ashh", BLUE);
	Trainer yellow("yel", YELLOW);
	Gym gym;

	// trainer not found
	ASSERT_THROW(LocationTrainerNotFoundException, gym.Leave(red));

	// arriving to gym and being leader
	ASSERT_NO_THROW(gym.Arrive(blue_1));
	ASSERT_NO_THROW(gym.Arrive(blue_2));
	ASSERT_NO_THROW(gym.Arrive(red));
	ASSERT_NO_THROW(gym.Arrive(yellow));
	ASSERT_EQUAL(blue_1.TotalScore(), 2);
	ASSERT_EQUAL(red.TotalScore(), 0);
	ASSERT_EQUAL(blue_2.TotalScore(), 1);
	ASSERT_EQUAL(yellow.TotalScore(), 13);

	// battle between prefered trainers
	ASSERT_NO_THROW(gym.Leave(yellow));
	ASSERT_EQUAL(blue_1.TotalScore(), 14);
	ASSERT_EQUAL(blue_2.TotalScore(), 1);
	ASSERT_EQUAL(red.TotalScore(), -1);
	ASSERT_EQUAL(yellow.TotalScore(), 3);

	// and again: same team auto leadre
	ASSERT_NO_THROW(gym.Leave(blue_1));
	ASSERT_EQUAL(blue_1.TotalScore(), 4);
	ASSERT_EQUAL(blue_2.TotalScore(), 11);
	ASSERT_EQUAL(red.TotalScore(), -1);
	ASSERT_EQUAL(yellow.TotalScore(), 3);

	// emty gym no leader
	ASSERT_NO_THROW(gym.Leave(blue_2));
	ASSERT_NO_THROW(gym.Leave(red));
	ASSERT_EQUAL(blue_1.TotalScore(), 4);
	ASSERT_EQUAL(blue_2.TotalScore(), 1);
	ASSERT_EQUAL(red.TotalScore(), -1);
	ASSERT_EQUAL(yellow.TotalScore(), 3);


	return true;
}
