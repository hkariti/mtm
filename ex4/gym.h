#pragma once
#include "location.h"
#include "exceptions.h"
#include "trainer.h"

namespace mtm {
namespace pokemongo {

class Gym : public Location {

	Trainer* leader;

	// Get's pointer to the most prefered team trainer in gym
	// return's NULL if no trainer for that team exist in gym
	Trainer* PreferedTeamTrainer(Team team);

public:

	~Gym() {}
	Gym() : leader(NULL) {}

	// make copy and assignment operator as compiler's default
	Gym(const Gym& gym) = default;
	Gym& operator=(const Gym& gym) = default;

	// throws LocationTrainerAlreadyInLocationException
	void Arrive(Trainer& trainer) override;

	// throws LocationTrainerNotFoundException
	void Leave(Trainer& trainer) override;

};

}
}