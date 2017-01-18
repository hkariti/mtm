#include "pokestop.h"

using namespace mtm::pokemongo;

void Pokestop::Arrive(Trainer& trainer) {
	std::vector<Item*>::iterator current_item;
	// Find the first item the new trainer can take and give it to them
	for (current_item = items.begin(); current_item != items.end();
		 current_item++) {
		if (trainer.AddItem(item)) {
			items.erase(current_item);
			break;
		}
	}

	this->Location::Arrive(trainer);
}

void Pokestop::AddItem(Item* item) {
	if (NULL == item) throw PokestopInvalidItemException();

	items.push_back(item);
}