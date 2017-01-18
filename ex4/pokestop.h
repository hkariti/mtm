#ifndef POKESTOP_H
#define POKESTOP_H
#include "location.h"
#include "item.h"

namespace mtm {
namespace pokemongo{
class Pokestop: public Location {
public:
	~Pokestop() {};
	Pokestop() : Location() {};
	void Arrive(Trainer& trainer) override;
	void AddItem(Item* item);
private:
	std::vector<Item*> items;
};
} // pokemongo
} // mtm

#endif POKESTOP_H
