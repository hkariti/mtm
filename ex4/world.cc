#include "world.h"

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
	return input;
}
