#include "starbucks.h"

using namespace mtm::pokemongo;

Starbucks::Starbucks(const std::vector<Pokemon> pokemons) 
	: pokemons(pokemons) {}

void Starbucks::Arrive(Trainer & trainer) {
	Location::Arrive(trainer);
	std::vector<Pokemon>::iterator current_pokemon;
	for (current_pokemon = pokemons.begin(); current_pokemon != pokemons.end();
		 current_pokemon++) {
		if (trainer.TryToCatch(*current_pokemon)) {
			pokemons.erase(current_pokemon);
			break;
		}
	}
}

void Starbucks::Leave(Trainer & trainer) {
	Location::Leave(trainer);
	
	// Do Nothing
}
