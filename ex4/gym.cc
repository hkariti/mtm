#include "gym.h"

using namespace mtm::pokemongo;

Trainer * Gym::PreferedTeamTrainer(Team team) {
	Trainer* prefered_trainer = NULL;
	for (Trainer* trainer : trainers_) {
		if (trainer->GetTeam() == team) {
			if (NULL == prefered_trainer) {
				prefered_trainer = trainer;
			}
			if (*trainer > *prefered_trainer) {
				prefered_trainer = trainer;
			}
		}
	}
	return prefered_trainer;
}

void Gym::Arrive(Trainer & trainer) {
	Location::Arrive(trainer);
	if (trainers_.size() == 1) {
		// If gym was empty - new trainer is the leader
		leader = &trainer;
	} else if (leader->GetTeam() != trainer.GetTeam()) {
		// If new trainer is from a different team, fight for leadership
		leader->is_leader = false;
		leader = TrainersBattle(*leader, trainer);
	}

	leader->is_leader = true;
}


void Gym::Leave(Trainer & trainer) {
	Location::Leave(trainer);
	if (leader != &trainer) return;

	trainer.is_leader = false;
	if (trainers_.empty()) {
		leader = NULL;
		return;
	}
	// Look for a new leader
	leader = PreferedTeamTrainer(trainer.GetTeam());
	// No trainers from the same team, take the strongest from the other teams
	if (NULL == leader) {
		Trainer *first_prefered = NULL, *second_prefered = NULL;
		for (int team = BLUE; team <= RED; team++) {
			if (team != trainer.GetTeam()) {
				if (NULL == first_prefered) {
					first_prefered = PreferedTeamTrainer((Team)team);
				} else {
					second_prefered = PreferedTeamTrainer((Team)team);
				}
			}
		}
		if (second_prefered == NULL) {
			leader =  first_prefered;
		} else {
			leader = TrainersBattle(*first_prefered, *second_prefered);
		}
	}
	leader->is_leader = true;
}
