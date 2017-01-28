#include "../pokemon_go.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "test_utils.h"
#include "../trainer.h"

using namespace mtm::pokemongo;
using namespace std;

void SetUpWorld(World* world) {
	std::istringstream world_in("	GYM haifa\n\
								POKESTOP tel_aviv POTION 5 CANDY 3 POTION 10\n\
								STARBUCKS eilat pikachu 3.5 1 squirtle 4 2\n\
								GYM ashdod\n\
								POKESTOP ashkelon POTION 4\n\
								STARBUCKS kfar_saba charmander 4 1 charizard 100 100");
	string line;

	while (std::getline(world_in, line)) {
		istringstream line_stream(line);
		line_stream >> *world;
	}
}


bool testTrainerAdd() {
	World* world = new World();
	SetUpWorld(world);
	PokemonGo pokemon_go(world);

	// invalid args
	ASSERT_THROW(PokemonGoInvalidArgsException,
		pokemon_go.AddTrainer("", YELLOW, "aroma"));

	// success
	ASSERT_NO_THROW(pokemon_go.AddTrainer("ash", YELLOW, "eilat"));

	// trainer already exist
	ASSERT_THROW(PokemonGoTrainerNameAlreadyUsedExcpetion,
		pokemon_go.AddTrainer("ash", YELLOW, "aroma"));

	// location doesn't exist
	ASSERT_THROW(PokemonGoLocationNotFoundException,
		pokemon_go.AddTrainer("gary", YELLOW, "aroma"));
	
	return true;
}

bool testTrainerMove() {
	World* world = new World();
	SetUpWorld(world);
	PokemonGo pokemon_go(world);

	world->Connect("tel_aviv", "haifa", NORTH, SOUTH);
	world->Connect("tel_aviv", "kfar_saba", EAST, WEST);
	world->Connect("tel_aviv", "ashdod", SOUTH, NORTH);
	world->Connect("ashdod", "ashkelon", SOUTH, NORTH);
	world->Connect("ashkelon", "eilat", SOUTH, NORTH);
	world->Connect("eilat", SOUTH);

	pokemon_go.AddTrainer("ash", YELLOW, "eilat");

	// trainer not found
	ASSERT_THROW(PokemonGoTrainerNotFoundExcpetion,
		pokemon_go.MoveTrainer("gary", WEST));

	// location reached dead end
	ASSERT_THROW(PokemonGoReachedDeadEndException,
		pokemon_go.MoveTrainer("ash", WEST));

	// success
	ASSERT_NO_THROW(pokemon_go.MoveTrainer("ash", SOUTH));
	ASSERT_EQUAL(pokemon_go.WhereIs("ash"), "eilat");
	ASSERT_NO_THROW(pokemon_go.MoveTrainer("ash", NORTH));
	ASSERT_EQUAL(pokemon_go.WhereIs("ash"), "ashkelon");

	return true;
}

bool testWhereIs() {
	World* world = new World();
	SetUpWorld(world);
	PokemonGo pokemon_go(world);

	world->Connect("tel_aviv", "haifa", NORTH, SOUTH);
	world->Connect("tel_aviv", "kfar_saba", EAST, WEST);
	world->Connect("tel_aviv", "ashdod", SOUTH, NORTH);
	world->Connect("ashdod", "ashkelon", SOUTH, NORTH);
	world->Connect("ashkelon", "eilat", SOUTH, NORTH);
	world->Connect("eilat", SOUTH);

	pokemon_go.AddTrainer("ash", YELLOW, "eilat");

	// invalid name
	ASSERT_THROW(PokemonGoTrainerNotFoundExcpetion, 
		pokemon_go.WhereIs("gary"));
	ASSERT_THROW(PokemonGoTrainerNotFoundExcpetion, pokemon_go.WhereIs(""));

	// success
	ASSERT_EQUAL(pokemon_go.WhereIs("ash"), "eilat");

	return true;
}

bool testTrainersIn() {
	World* world = new World();
	SetUpWorld(world);
	PokemonGo pokemon_go(world);

	world->Connect("tel_aviv", "haifa", NORTH, SOUTH);
	world->Connect("tel_aviv", "kfar_saba", EAST, WEST);
	world->Connect("tel_aviv", "ashdod", SOUTH, NORTH);
	world->Connect("ashdod", "ashkelon", SOUTH, NORTH);
	world->Connect("ashkelon", "eilat", SOUTH, NORTH);
	world->Connect("eilat", SOUTH);

	pokemon_go.AddTrainer("ash", YELLOW, "eilat");

	// location doesnt exist
	ASSERT_THROW(PokemonGoLocationNotFoundException,
		pokemon_go.GetTrainersIn("aroma"));

	ASSERT_TRUE(pokemon_go.GetTrainersIn("ashdod").empty());
	ASSERT_EQUAL(pokemon_go.GetTrainersIn("eilat").size(), 1);

	return true;
}

bool testGetScore() {
	World* world = new World();
	SetUpWorld(world);

	world->Connect("tel_aviv", "haifa", NORTH, SOUTH);
	world->Connect("tel_aviv", "kfar_saba", EAST, WEST);
	world->Connect("tel_aviv", "ashdod", SOUTH, NORTH);
	world->Connect("ashdod", "ashkelon", SOUTH, NORTH);
	world->Connect("ashkelon", "eilat", SOUTH, NORTH);
	world->Connect("eilat", SOUTH);

	PokemonGo pokemon_go(world);
	pokemon_go.AddTrainer("Ash", RED, "tel_aviv");

	ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);
	ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 0);
	ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);

	pokemon_go.MoveTrainer("Ash", SOUTH);

	ASSERT_EQUAL(pokemon_go.GetScore(RED), 11);

	pokemon_go.MoveTrainer("Ash", SOUTH);
	pokemon_go.MoveTrainer("Ash", SOUTH);
	pokemon_go.MoveTrainer("Ash", SOUTH);
	pokemon_go.MoveTrainer("Ash", SOUTH);
	pokemon_go.MoveTrainer("Ash", SOUTH);
	pokemon_go.MoveTrainer("Ash", SOUTH);


	pokemon_go.AddTrainer("Clemont", BLUE, "ashdod");
	pokemon_go.MoveTrainer("Ash", NORTH);
	pokemon_go.MoveTrainer("Ash", NORTH);

	ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 0);
	ASSERT_EQUAL(pokemon_go.GetScore(RED), 14);

	return true;
}
