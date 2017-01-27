#include "pokemon.h"
#include "exceptions.h"

using namespace mtm::pokemongo;

#define MAX_POKEMON_HP		100
#define STRING_TYPES	{"NORMAL", "ROCK", "BUG", "FAIRY", "GROUND", \
						"GRASS", "WATER", "ICE", "GHOST", "POISON", \
						"ELECTRIC", "FIRE", "FLYING", "PSYCHIC"}


Pokemon::Pokemon(const std::string & species,
				 const std::set<PokemonType>& types, const double & cp,
				 const int & level)
	: species(species), types(types), cp(cp), level(level), hp(MAX_POKEMON_HP) {
	if (cp <= 0 || level <= 0 || species.size() == 0) {
		throw PokemonInvalidArgsException();
	}
}

Pokemon::Pokemon(const std::string & species, const double & cp,
				 const int & level)
	: Pokemon(species, GetDefaultTypes(species), cp, level) {}

Pokemon::Pokemon(const Pokemon & pokemon)
	: Pokemon(pokemon.species, pokemon.types, pokemon.cp, pokemon.level) {}

Pokemon & mtm::pokemongo::Pokemon::operator=(const Pokemon & pokemon) {
	types = pokemon.types;
	species = pokemon.species;
	cp = pokemon.cp;
	level = pokemon.level;
	hp = pokemon.hp;
	return *this;
}

double Pokemon::HitPower() const {
	return cp * level;
}

double Pokemon::comparePokemon(const Pokemon& rhs) const {
	if (HitPower() != rhs.HitPower()) return HitPower() - rhs.HitPower();
	int lhs_types_sum = 0, rhs_types_sum = 0;
	for (const PokemonType& lhs_type : types) {
		lhs_types_sum += lhs_type;
	}
	for (const PokemonType& rhs_type : rhs.types) {
		rhs_types_sum += rhs_type;
	}
	return lhs_types_sum - rhs_types_sum;
}

bool Pokemon::operator==(const Pokemon& rhs) const {
	return comparePokemon(rhs) == 0;
}
bool Pokemon::operator!=(const Pokemon& rhs) const {
	return comparePokemon(rhs) != 0;
}
bool Pokemon::operator<(const Pokemon& rhs) const {
	return comparePokemon(rhs) < 0;
}
bool Pokemon::operator>(const Pokemon& rhs) const {
	return comparePokemon(rhs) > 0;
}
bool Pokemon::operator<=(const Pokemon& rhs) const {
	return comparePokemon(rhs) <= 0;
}
bool Pokemon::operator>=(const Pokemon& rhs) const {
	return comparePokemon(rhs) >= 0;
}

int Pokemon::Level() const {
	return level;
}

bool Pokemon::Hit(Pokemon & victim) {
	victim.hp -= HitPower();
	if (victim.hp <= 0) {
		victim.hp = 0;
		return true;
	}
	return false;
}

void Pokemon::Heal() {
	hp = MAX_POKEMON_HP;
}

void Pokemon::Train(const double & boost) {
	if (boost <= 1) throw PokemonInvalidArgsException();
	cp *= boost;
}

std::ostream & mtm::pokemongo::operator<<(std::ostream & output,
										  const Pokemon & pokemon) {
	std::string string_types[] = STRING_TYPES;
	output << pokemon.species << "(" << pokemon.level << "/" << pokemon.cp \
		   << "/" << pokemon.hp << ")";
	for (const PokemonType& type : pokemon.types) {
		output << " " << string_types[type];
	}
	output << std::endl;
	return output;
}
