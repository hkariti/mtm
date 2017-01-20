#ifndef POKESTOP_H
#define POKESTOP_H
#include "location.h"
#include "item.h"
#include "trainer.h"

namespace mtm {
namespace pokemongo{
class Pokestop: public Location {
public:
	~Pokestop();
	Pokestop() : Location() {};
	void Arrive(Trainer& trainer) override;
	// @throws PokestopInvalidItemException if null arg is passed
	void AddItem(Item* item);
private:
	std::vector<Item*> items;
};
} // pokemongo
} // mtm

#endif
