#ifndef POKESTOP_H
#define POKESTOP_H
#include "location.h"
#include "item.h"
#include "trainer.h"

namespace mtm {
namespace pokemongo{
class Pokestop: public Location {
public:
	// Constructs an empty Pokestop
	Pokestop() : Location() {};
	// Destructor. Destroys all items in the Pokestop as well.
	~Pokestop();
	// Adds a trainer to the Pokestop, and gives them the first item
	// they can carry from the items list
	// @throws LocationTrainerAlreadyInLocationException if trainer is already
	//         in the Pokestop.
	void Arrive(Trainer& trainer) override;
	// Adds an item to the pokestop.
	// @throws PokestopInvalidItemException if null arg is passed
	void AddItem(Item* item);
private:
	std::vector<Item*> items;
};
} // pokemongo
} // mtm

#endif
