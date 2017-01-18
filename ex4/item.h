#ifndef ITEM_H
#define ITEM_H
#include "pokemon.h"

namespace mtm {
namespace pokemongo {
class Item {
public:
	int level;
	Item(int level) : level(level) {}
	virtual ~Item() {}
	virtual void Use(Pokemon&) = 0;
};

class Candy : public Item {
public:
	Candy(int level) : Item(level) {}
	~Candy() {}
	void Use(Pokemon& pokemon) override {
		int pokemon_level = pokemon.Level();
		double cp_multiplier = 1 + (double)pokemon_level/10;
		pokemon.Train(cp_multiplier);
	}
};

class Potion: public Item {
public:
	Potion(int level) : Item(level) {}
	~Potion() {}
	void Use(Pokemon& pokemon) override {
		pokemon.Heal();
	}
};
} // pokemongo namespace
} // mtm namespace
#endif
