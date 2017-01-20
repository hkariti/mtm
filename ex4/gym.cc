#include "gym.h"

using namespace mtm::pokemongo;

Trainer * Gym::PreferedTeamTrainer(Team team)
{
	Trainer* prefered_trainer = NULL;
	for (Trainer* trainer : trainers_) {
		if (trainer->GetTeam() == team) {
			if (NULL == prefered_trainer) {
				prefered_trainer = trainer;
			}
			if (trainer > prefered_trainer) {
				prefered_trainer = trainer;
			}
		}
	}
	return prefered_trainer;
}

void Gym::Arrive(Trainer & trainer)
{
	Location::Arrive(trainer);
	if (trainers_.size() == 1) {
		leader = &trainer;
	}
	else if (leader->GetTeam() != trainer.GetTeam()) {
		leader = TrainersBattle(*leader, trainer);
	}
}


void Gym::Leave(Trainer & trainer)
{
	Location::Leave(trainer);
	if (leader == &trainer) {
		trainer.gym_leader_counter--;
		if (trainers_.empty()) {
			leader = NULL;
			return;
		}
		leader = PreferedTeamTrainer(trainer.GetTeam());
		if (NULL == leader) { //no trainers from the same team exist
			Trainer *first_prefered = NULL, *second_prefered = NULL;
			for (int team = BLUE; team <= RED; team++) {
				if (team != trainer.GetTeam()) {
					if (NULL == first_prefered) {
						first_prefered = PreferedTeamTrainer((Team)team);
					}
					else {
						second_prefered = PreferedTeamTrainer((Team)team);
					}
				}
			}
			if (second_prefered == NULL) { // only one team exist in gym
				leader =  first_prefered;
			}
			else { // 2 other teams exist in gym
				leader = TrainersBattle(*first_prefered, *second_prefered);
			}
		}
		leader->gym_leader_counter++;
	}
}
