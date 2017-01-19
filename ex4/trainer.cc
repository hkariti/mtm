#include "trainer.h"
#include "exceptions.h"

#define LOSER_TRAINER_LEVEL_FACTOR	2
#define TRAINER_WIN_POINTS			2
#define TRAINER_LOSE_POINTS			-1
#define GYM_LEADER_POINTS			10

using namespace mtm::pokemongo;

Trainer::Trainer(const std::string & name, const Team & team)
	: name(name), team(team), level(1), pokemons() 
	: name(name), team(team), pokemons(), level(1), battle_score_history(0)
{
	if (name.size() == 0) throw TrainerInvalidArgsException();
}

Trainer::~Trainer() {
	std::vector<Item*>::iterator current_item;
	// Delete all items
	for (current_item = items.begin(); current_item != items.end();
		 current_item++) {
		delete *current_item;
	}
}

Pokemon& Trainer::GetStrongestPokemon()
{
	return const_cast<Pokemon&>(static_cast<const Trainer*>(this)->GetStrongestPokemon());
}

const Pokemon & Trainer::GetStrongestPokemon() const
{
	if (pokemons.empty()) throw TrainerNoPokemonsFoundException();
	const Pokemon *strongestPokemon = &pokemons[0];
	for (const Pokemon& pokemon : pokemons) {
		if (pokemon > *strongestPokemon) {
			strongestPokemon = &pokemon;
		}
	}
	return *strongestPokemon;
}

void Trainer::KillStrongestPokemon()
{
	if (pokemons.empty()) throw TrainerNoPokemonsFoundException();
	pokemons.erase(std::find(pokemons.begin(), pokemons.end(), GetStrongestPokemon()));
}

int Trainer::compareTrainer(const Trainer & rhs) const
{
	if (pokemons.empty() && rhs.pokemons.empty()) return 0;
	if (pokemons.empty()) return -1;
	if (rhs.pokemons.empty()) return 1;
	if (GetStrongestPokemon() > rhs.GetStrongestPokemon()) return 1;
	if (GetStrongestPokemon() < rhs.GetStrongestPokemon()) return -1;
	return 0;
}
bool Trainer::operator==(const Trainer& rhs) const {
	return compareTrainer(rhs) == 0;
}
bool Trainer::operator!=(const Trainer& rhs) const {
	return compareTrainer(rhs) != 0;
}
bool Trainer::operator<(const Trainer& rhs) const {
	return compareTrainer(rhs) < 0;
}
bool Trainer::operator>(const Trainer& rhs) const {
	return compareTrainer(rhs) > 0;
}
bool Trainer::operator<=(const Trainer& rhs) const {
	return compareTrainer(rhs) <= 0;
}
bool Trainer::operator>=(const Trainer& rhs) const {
	return compareTrainer(rhs) >= 0;
}

bool Trainer::IsAlly(const Trainer & trainer) const
{
	return team == trainer.team;
}

Team Trainer::GetTeam() const
{
	return team;
}

bool Trainer::TryToCatch(Pokemon & pokemon)
{
	if (pokemon.Level() > level) return false;
	pokemons.push_back(pokemon);
	return true;
}

int Trainer::TotalScore()
{
	return level + battle_score_history +
		gym_leader_counter * GYM_LEADER_POINTS;
}

bool Trainer::AddItem(Item* item) {
	if (NULL == item) throw TrainerInvalidArgsException();

	if (item->level > level) return false;
	items.push_back(item);
	return true;
}

void Trainer::BoostPokemon(Pokemon& pokemon) {
	if (items.empty()) return;
	std::vector<Item*>::iterator first_item = items.begin();
	(*first_item)->Use(pokemon);
	delete *first_item;
	items.erase(first_item);
}

std::ostream & mtm::pokemongo::operator<<(std::ostream & output, const Trainer & trainer)
{
	output << trainer.name << " (" << trainer.level << ") " << trainer.team << std::endl;
	for (const Pokemon& pokemon : trainer.pokemons) {
		output << pokemon;
	}
	return output;
}

void Trainer::RaiseLevel(Trainer& loser)
{
	level += loser.level / LOSER_TRAINER_LEVEL_FACTOR + 
		loser.level % LOSER_TRAINER_LEVEL_FACTOR;
}

void Trainer::UpdateBattleScoreHistory(Trainer & winner)
{
	if (*this == winner) {
		battle_score_history += TRAINER_WIN_POINTS;
	}
	else {
		battle_score_history += TRAINER_LOSE_POINTS;
	}
}

Trainer* mtm::pokemongo::PreferedTrainerByTeam(Trainer& trainer_1,
			Trainer& trainer_2) {
	
	if (trainer_1.GetTeam() == trainer_2.GetTeam()) {
		throw TrainerInvalidArgsException();
	}
	if (trainer_1.GetTeam() == BLUE) {
		if (trainer_2.GetTeam() == YELLOW) {
			return &trainer_2;
		}
		else {
			return &trainer_1;
		}
	}
	if (trainer_1.GetTeam() == RED) {
		if (trainer_2.GetTeam() == YELLOW) {
			return &trainer_1;
		}
		else {
			return &trainer_2;
		}
	}
	else { // trainer_1 team is yellow
		if (trainer_2.GetTeam() == BLUE) {
			return &trainer_1;
		}
		else {
			return &trainer_2;
		}
	}
}

Trainer* mtm::pokemongo::TrainersBattle(Trainer& trainer_1, Trainer& trainer_2)
{
	Trainer *winner = NULL;
	if (!trainer_1.pokemons.empty() && !trainer_2.pokemons.empty()) {
		winner = TrainersBattleWithPokemons(trainer_1, trainer_2);
	}
	else if (trainer_1.pokemons.empty() && !trainer_2.pokemons.empty()) {
		winner = &trainer_2;
	}
	else if (!trainer_1.pokemons.empty() && trainer_2.pokemons.empty()) {
		winner = &trainer_1;
	}
	if (winner == &trainer_1) {
		trainer_1.RaiseLevel(trainer_2);
	}
	else if (winner == &trainer_2) {
		trainer_2.RaiseLevel(trainer_1);
	}
	else {
		winner = PreferedTrainerByTeam(trainer_1, trainer_2);
	}
	trainer_1.UpdateBattleScoreHistory(*winner);
	trainer_2.UpdateBattleScoreHistory(*winner);
	return winner;
}

Trainer * mtm::pokemongo::TrainersBattleWithPokemons(Trainer & trainer_1, Trainer & trainer_2)
{
	Trainer* winner = NULL;
	Pokemon *pokemon_1 = &trainer_1.GetStrongestPokemon();
	Pokemon *pokemon_2 = &trainer_2.GetStrongestPokemon();
	trainer_1.BoostPokemon(*pokemon_1);
	trainer_2.BoostPokemon(*pokemon_2);
	if (pokemon_1 > pokemon_2) {
		winner = &trainer_1;
	}
	if (pokemon_2 > pokemon_1) {
		winner = &trainer_2;
	}
	if ((*pokemon_1).Hit(*pokemon_2)) {
		trainer_2.KillStrongestPokemon();
	}
	if ((*pokemon_2).Hit(*pokemon_1)) {
		trainer_1.KillStrongestPokemon();
	}
	return winner;
}
