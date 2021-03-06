#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include <stdexcept>
#include "k_graph.h"
#include "location.h"
#include "item.h"
#include "pokemon.h"


namespace mtm {
namespace pokemongo {

typedef int Direction;

static const int NORTH = 0;
static const int SOUTH = 1;
static const int EAST = 2;
static const int WEST = 3;

class World : public KGraph<std::string, Location*, 4> {
 public:
  // Constructs a new empty world.
  World();
  
  // A destructor.
  ~World();
  
  // Input iterator. Scans a single line from the input stream. The line can be
  // one of the following three options:
  //
  // (1) "GYM <gym_name>"
  //     e.g. "GYM taub"
  //     Adds a gym with the given name to the world.
  // (2) "POKESTOP <pokestop_name> <item_type1> <item_level1> <item_type2>
  //         <item_level2> ..."
  //     e.g. "POKESTOP mikhlol POTION 10 CANDY 20 CANDY 13"
  //     Creates a Pokestop with the given name that contains the given items by
  //     order, and adds it to the world.
  // (3) "STARBUCKS <starbucks_name> <pokemon_species1> <cp1> <level1>
  //          <pokemon_species2> <cp2> <level2> ..."
  //     e.g. "STARBUCKS shani pikachu 2.5 3 pikachu 1 2 charmander 3.45 4"
  //     Creates a starbucks with the given name that contains Pokemons with the
  //     given parameters by order, and adds it to the world. The Pokemons will
  //     have the default types.
  // You can assume that none of the pieces of information (location name,
  // Pokemon species, etc.) contains a space.
  //
  // @param input the input stream.
  // @param world the world to which to add the locations.
  // @return the input stream.
  // @throw WorldInvalidInputLineException if the input line is not one of the
  //        three options, or one of the parameters is invalid (for example,
  //        negative CP value, etc.).
  // @throw WorldLocationNameAlreadyUsed if there already exists a location with
  //        the given name in the world.
  friend std::istream& operator>>(std::istream& input, World& world);

  // Disable copy constructor.
  World(const World& world) = delete;

  // Disable assignment operator.
  void operator=(const World& world) = delete;

  void Remove(std::string const& key);

protected:

	// Add new Gym to world
	// @param iss suppose to be empty stream (not args for gym)
	// @throw WorldInvalidInputLineException if iss has content
	void AddGym(std::istringstream& iss, std::string name);

	// Add new Pokestop to world
	// @param iss Stream containing all Pokestop items
	// @throw WorldInvalidInputLineException if pokestop args are invalid
	void AddPokestop(std::istringstream& iss, std::string name);

	// Add new Starbucks to world
	// @param iss Stream containing all Starbucks items
	// @throw WorldInvalidInputLineException if starbucks args are invalid
	void AddStarbucks(std::istringstream& iss, std::string name);
};

std::istream& operator>>(std::istream& input, World& world);

}  // namespace pokemongo
}  // namespace mtm

#endif  // WORLD_H
