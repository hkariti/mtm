#include <algorithm>
#include "trainer.h"
#include "exceptions.h"

using namespace mtm::pokemongo;

Trainer::Trainer(const std::string& name, const Team& team) {
  if (name.empty()) {
    throw pokemongo::TrainerInvalidArgsException();
  }
  this->name = name;
  this->team = team;
  this->level = 1;
}

Pokemon& Trainer::GetStrongestPokemon() {
  PokemonList::const_iterator strongest_pokemon_iterator;
  strongest_pokemon_iterator = this->GetStrongestPokemonIterator();

  return (Pokemon&)*strongest_pokemon_iterator;
}

const Pokemon& Trainer::GetStrongestPokemon() const {
  PokemonList::const_iterator strongest_pokemon_iterator;
  strongest_pokemon_iterator = this->GetStrongestPokemonIterator();

  return *strongest_pokemon_iterator;
}

void Trainer::KillStrongestPokemon() {
  PokemonList::const_iterator strongest_pokemon;
  strongest_pokemon = this->GetStrongestPokemonIterator();
  this->pokemon_list.erase(strongest_pokemon);
}

PokemonList::const_iterator Trainer::GetStrongestPokemonIterator() const {
  if (this->pokemon_list.empty()) {
    throw pokemongo::TrainerNoPokemonsFoundException();
  }
  return std::max_element(this->pokemon_list.begin(), this->pokemon_list.end());
} 

bool Trainer::operator==(const Trainer& rhs) const {
  return this->compare(rhs) == 0;
}

bool Trainer::operator!=(const Trainer& rhs) const {
  return this->compare(rhs) != 0;
}

bool Trainer::operator!=(const Trainer& rhs) const {
  return this->compare(rhs) != 0;
}

bool Trainer::operator<(const Trainer& rhs) const {
  return this->compare(rhs) < 0;
}

bool Trainer::operator>(const Trainer& rhs) const {
  return this->compare(rhs) > 0;
}

bool Trainer::operator<=(const Trainer& rhs) const {
  return this->compare(rhs) <= 0;
}

bool Trainer::operator>=(const Trainer& rhs) const {
  return this->compare(rhs) >= 0;
}

bool Trainer::IsAlly(const Trainer& trainer) const {
  return this->team == trainer.team;
}

Team Trainer::GetTeam() const {
  return this->team;
}

bool Trainer::TryToCatch(Pokemon& pokemon) {
  if (this->level < pokemon.Level()) return false;
  this->pokemon_list.push_back(pokemon);
  return true;
}

std::ostream& operator<<(std::ostream& output, const Trainer& trainer) {
  output << trainer.name << " (" << trainer.level << ") " << trainer.team << std::endl;
}
int Trainer::compare(const Trainer& rhs) const {
  if (this->pokemon_list.empty()) {
    if (rhs.pokemon_list.empty()) return 0;
    return -1;
  }
  if (rhs.pokemon_list.empty()) return 1;

  Pokemon const strongest_mine = this->GetStrongestPokemon();
  Pokemon const strongest_rhs = rhs.GetStrongestPokemon();
  if (strongest_mine > strongest_rhs) return 1;
  if (strongest_mine < strongest_rhs) return -1;
  return 0;
}
