#include "starbucks.h"

using namespace mtm::pokemongo;

Starbucks::Starbucks(const std::vector<Pokemon> pokemons) 
	: pokemons(pokemons) {}

void Starbucks::Arrive(Trainer & trainer) {
	Location::Arrive(trainer);
	if (!pokemons.empty()) {
		if (trainer.TryToCatch(pokemons.front())) {
			//  catch succeeded - remove pokemon from list
			pokemons.erase(pokemons.begin());
		}
	}
}

void Starbucks::Leave(Trainer & trainer) {
	Location::Leave(trainer);
	
	// Do Nothing
}
