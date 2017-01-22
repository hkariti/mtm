#pragma once
#include "location.h"
#include "exceptions.h"
#include "trainer.h"

namespace mtm {
namespace pokemongo {

class Gym : public Location {

	Trainer* leader;

	// Gets pointer to the most preferred team trainer in gym.
	// A preferred trainer is the strongest trainer from the same
	// team given.
	// @param team - Team to find the preferred trainer from
	// return NULL if no trainer for that team exist in gym
	Trainer* PreferedTeamTrainer(Team team);

public:

	~Gym() {}

	// Constructs a new gym with no leader.
	Gym() : leader(NULL) {}

	// make copy and assignment operator as compiler's default
	Gym(const Gym& gym) = default;
	Gym& operator=(const Gym& gym) = default;

	// Handle a new trainer arriving to Gym, trying his luck to be the leader!
	//
	// @param trainer the trainer arriving.
	// @throw LocationTrainerAlreadyInLocationException if trainer is already
	//		  in the gym.
	void Arrive(Trainer& trainer) override;

	// Handle an old trainer leaving the gym for a new adventure
	// If the trainer was the leader, making other trainer (if exist) the
	// leader according to the exercise sheet.
	//
	// @param trainer the trainer leaving.
	// @throw LocationTrainerNotFoundException if trainer is not
	//		  in the gym.
	void Leave(Trainer& trainer) override;

};

}
}
