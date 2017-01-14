#include "pokemon_go.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "test_utils.h"
#include "trainer.h"

using namespace mtm::pokemongo;
using namespace std;

void SetUpWorld(World* world) {
  ifstream world_in("world.in");
  string line;

  while (std::getline(world_in, line)) {
      istringstream line_stream(line);
      line_stream >> *world;
  }
}

bool TestPokemonGo() {
  World* world = new World();
  SetUpWorld(world);
  
  world->Connect("holmes_place", "aroma", NORTH, SOUTH);
  world->Connect("panorama", "shufersal", NORTH, SOUTH);
  world->Connect("shufersal", "aroma", EAST, WEST);
  world->Connect("panorama", "holmes_place", EAST, WEST);

  PokemonGo pokemon_go(world);
  pokemon_go.AddTrainer("Ash", YELLOW, "aroma");
  pokemon_go.MoveTrainer("Ash", SOUTH);
  pokemon_go.AddTrainer("Clemont", RED, "aroma");

  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);

  pokemon_go.MoveTrainer("Clemont", SOUTH);

  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 14);

  const vector<Trainer*>& trainers_in_holmes =
     pokemon_go.GetTrainersIn("holmes_place");
  const vector<Trainer*>& trainers_in_aroma =
     pokemon_go.GetTrainersIn("aroma");

  ASSERT_TRUE(trainers_in_aroma.empty());
  ASSERT_EQUAL(trainers_in_holmes.size(), 2);

  ostringstream output;

  output << *trainers_in_holmes[0];
  output << *trainers_in_holmes[1];
  
  ASSERT_EQUAL(output.str(),
               "Ash 1 YELLOW\n"
               "pikachu(1/2.5/97)\n"
               "Clemont 2 RED\n"
               "charmander(1/3/97.5) NORMAL BUG FAIRY ICE GHOST\n");

  return true;
}