#ifndef ITEM_H
#define ITEM_H
#include "pokemon.h"
#include "exceptions.h"


namespace mtm {
namespace pokemongo {

// Used in cp_multiplier formula below
static const int CP_LEVEL_FACTOR = 10;

// Abstract Item class
class Item {
public:
	// The item level. Set when item is created.
	const int level;
	// Creates a new item
	// @param level - the new item's level
	// @throws ItemInvalidArgException if level is negative
	Item(int level) : level(level) {
		if (level < 0) throw ItemInvalidArgException();
	}
	// Destructor
	virtual ~Item() {}
	// Use the item on a pokemon
	// @param pokemon - the pokemon to use the item on
	virtual void Use(Pokemon&) = 0;
};

// Candy. Used to train a pokemon
class Candy : public Item {
public:
	Candy(int level) : Item(level) {}
	~Candy() {}
	void Use(Pokemon& pokemon) override {
		int pokemon_level = pokemon.Level();
		double cp_multiplier = 1 + ((double)pokemon_level)/CP_LEVEL_FACTOR;
		pokemon.Train(cp_multiplier);
	}
};

// Potion. Used to heal a pokemon
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
