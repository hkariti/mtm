#include "pokestop.h"

using namespace mtm::pokemongo;

Pokestop::~Pokestop() {
	std::vector<Item*>::iterator current_item;
	// Destroy all items
	for (current_item = items.begin(); current_item != items.end();
		 current_item++) {
		delete *current_item;
	}
}

void Pokestop::Arrive(Trainer& trainer) {
	// Call the parent's arrive function
	this->Location::Arrive(trainer);

	std::vector<Item*>::iterator current_item;
	// Find the first item the new trainer can take and give it to them
	for (current_item = items.begin(); current_item != items.end();
		 current_item++) {
		if (trainer.AddItem(*current_item)) {
			items.erase(current_item);
			break;
		}
	}
}

void Pokestop::AddItem(Item* item) {
	if (NULL == item) throw PokestopInvalidItemException();

	items.push_back(item);
}
