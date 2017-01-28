#include "../pokestop.h"
#include "test_utils.h"

using namespace mtm::pokemongo;

bool testConstructor() {
	ASSERT_NO_THROW(Pokestop());

	return true;
}

bool testDestructor() {
	Pokestop *pokestop = new Pokestop;
	Item* item;
	// Add some items
	item = new Potion(4);
	pokestop->AddItem(item);
	item = new Candy(2);
	pokestop->AddItem(item);

	// Destroy pokestop, items should be destroyed
	ASSERT_NO_THROW(delete pokestop);

	return true;
}

bool testAddItem() {
	Pokestop pokestop;
	Item *item;

	// test null arg
	ASSERT_THROW(PokestopInvalidItemException, pokestop.AddItem(NULL));

	// Add items
	item = new Potion(3);
	ASSERT_NO_THROW(pokestop.AddItem(item));
	item = new Candy(3);
	ASSERT_NO_THROW(pokestop.AddItem(item));

	return true;
}

bool testArrive() {
	Pokestop pokestop;
	Trainer trainer1("ash", BLUE);
	Trainer trainer2("gary", RED);
	Item *item;

	// Test arrive to empty pokestop
	ASSERT_NO_THROW(pokestop.Arrive(trainer1));

	// Test trainer already in location
	ASSERT_THROW(LocationTrainerAlreadyInLocationException,
				 pokestop.Arrive(trainer1));

	// add some items
	item = new Potion(3);
	pokestop.AddItem(item);
	item = new Candy(1);
	pokestop.AddItem(item);

	// Add a new trainer
	ASSERT_NO_THROW(pokestop.Arrive(trainer2));

	// TODO: Add a getItems function?
	return true;
}

bool testLeave() {
	Pokestop pokestop;
	Trainer trainer1("ash", BLUE);
	Trainer trainer2("gary", RED);

	// Test trainer not in location
	ASSERT_THROW(LocationTrainerNotFoundException,
				 pokestop.Leave(trainer1));

	// Add trainers
	pokestop.Arrive(trainer1);
	pokestop.Arrive(trainer2);

	// Make them leave now
	ASSERT_NO_THROW(pokestop.Leave(trainer1));
	ASSERT_NO_THROW(pokestop.Leave(trainer2));

	// Not in location again
	ASSERT_THROW(LocationTrainerNotFoundException,
				 pokestop.Leave(trainer1));

	return true;
}

bool testGetTrainers() {
	Pokestop pokestop;
	Trainer trainer1("ash", BLUE);
	Trainer trainer2("gary", RED);
	std::vector<Trainer*> trainers;

	// Check empty
	ASSERT_NO_THROW(trainers = pokestop.GetTrainers());
	ASSERT_TRUE(trainers.size() == 0);

	// Add trainers and list
	pokestop.Arrive(trainer1);
	pokestop.Arrive(trainer2);
	ASSERT_NO_THROW(trainers = pokestop.GetTrainers());
	ASSERT_TRUE(trainers.size() == 2);
	// TODO: add trainer.GetName and compare items in vector

	// Remove one and list
	pokestop.Leave(trainer1);
	ASSERT_NO_THROW(trainers = pokestop.GetTrainers());
	ASSERT_TRUE(trainers.size() == 1);

	// Remove last and list
	pokestop.Leave(trainer2);
	ASSERT_NO_THROW(trainers = pokestop.GetTrainers());
	ASSERT_TRUE(trainers.size() == 0);
	
	return true;
}
