#include "world.h"
#include "exceptions.h"
#include "gym.h"
#include "pokestop.h"
#include "starbucks.h"
#include <vector>
#include <sstream>

using namespace mtm::pokemongo;

World::World()
	: KGraph(NULL)
{}

World::~World()
{
	std::map<std::string, Node*>::iterator it;
	for (it = KGraph::nodes_.begin(); it != KGraph::nodes_.end(); it++) {
		KGraph::Remove((*it).first);
	}
}

std::istream & mtm::pokemongo::operator >> (std::istream & input, World & world)
{
	std::string line, location_type, location_name;
	std::getline(input, line);
	std::istringstream iss(line);
	iss >> location_type >> location_name;
	if (location_name.empty()) throw WorldInvalidInputLineException();
	try {
		if (location_type == "GYM") {
			world.AddGym(iss, location_name);
		}
		else if (location_type == "POKESTOP") {
			world.AddPokestop(iss, location_name);
		}
		else if (location_type == "STARBUCKS") {
			world.AddStarbucks(iss, location_name);
		}
		else {
			throw WorldInvalidInputLineException();
		}
	}
	catch (KGraphKeyAlreadyExistsExpection) {
		throw WorldLocationNameAlreadyUsed();
	}
	return input;
}

void World::AddGym(std::istringstream & iss, std::string name)
{
	if (!iss.eof()) throw WorldInvalidInputLineException();
	KGraph::Insert(name, new Gym);
}

void World::AddPokestop(std::istringstream & iss, std::string name)
{
	Pokestop* pokestop = new Pokestop;
	while (!iss.eof()) {
		std::string item_type;
		int item_level = -1;
		iss >> item_type >> item_level;
		try {
			if (item_type == "POTION") {
				pokestop->AddItem(new Potion(item_level));
			}
			else if (item_type == "CANDY") {
				pokestop->AddItem(new Candy(item_level));
			}
			else {
				throw ItemInvalidArgException();
			}
		}
		catch (ItemInvalidArgException) {
			delete pokestop;
			throw WorldInvalidInputLineException();
		}
	}
	KGraph::Insert(name, pokestop);
}

void World::AddStarbucks(std::istringstream & iss, std::string name)
{
	std::vector<Pokemon> pokemons;
	while (!iss.eof()) {
		std::string species;
		int cp = -1, level = -1;
		iss >> species >> cp >> level;
		try {
			pokemons.push_back(Pokemon(species, cp, level));
		}
		catch (PokemonInvalidArgsException) {
			throw WorldInvalidInputLineException();
		}
	}
	KGraph::Insert(name, new Starbucks(pokemons));
}