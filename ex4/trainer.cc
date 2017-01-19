#include "trainer.h"
#include "exceptions.h"

using namespace mtm::pokemongo;

Trainer::Trainer(const std::string & name, const Team & team)
	: name(name), team(team), level(1), pokemons() 
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
