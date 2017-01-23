#include "pokemon_go.h"

using namespace mtm::pokemongo;

PokemonGo::PokemonGo(const World * world) : world(world) {}

PokemonGo::~PokemonGo() {
	delete world;
}

void PokemonGo::AddTrainer(const std::string & name, const Team & team,
						   const std::string & location) {
	try {
		Trainer trainer(name, team);
		if (trainers.find(name) != trainers.end()) {
			throw PokemonGoTrainerNameAlreadyUsedExcpetion();
		}
		if (!world->Contains(location)) {
			throw PokemonGoLocationNotFoundException();
		}
		trainers.insert({ name, trainer });
		trainers.at(name).current_location_name = location;
		(*world)[location]->Arrive(trainers.at(name));
	}
	catch (TrainerInvalidArgsException) {
		throw PokemonGoInvalidArgsException();
	}
}

void PokemonGo::MoveTrainer(const std::string & trainer_name,
							const Direction & dir) {
	try {
		Trainer& trainer = trainers.at(trainer_name);
		World::const_iterator it = \
			world->BeginAt(trainer.current_location_name);
		it.Move(dir);
		if (it == world->End()) throw PokemonGoReachedDeadEndException();
		(*world)[trainer.current_location_name]->Leave(trainer);
		trainer.current_location_name = (*it);
		(*world)[*it]->Arrive(trainer);
	}
	catch (std::out_of_range) {
		throw PokemonGoTrainerNotFoundExcpetion();
	}
}

std::string PokemonGo::WhereIs(const std::string & trainer_name) {
	try {
		return trainers.at(trainer_name).current_location_name;
	}
	catch (std::out_of_range) {
		throw PokemonGoTrainerNotFoundExcpetion();
	}
}

const std::vector<Trainer*>& PokemonGo::GetTrainersIn(
												const std::string & location) {
	try {
		World::const_iterator it = world->BeginAt(location);
		return (*world)[*it]->GetTrainers();
	}
	catch (KGraphKeyNotFoundException) {
		throw PokemonGoLocationNotFoundException();
	}
}

int PokemonGo::GetScore(const Team & team) {
	int score = 0;
	std::unordered_map<std::string, Trainer>::iterator it;
	for (it = trainers.begin(); it != trainers.end(); it++) {
		Trainer& trainer = it->second;
		if (trainer.GetTeam() == team) {
			score += trainer.TotalScore();
		}
	}
	return score;
}

