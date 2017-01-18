#ifndef TRAINER_H
#define TRAINER_H

#include <iostream>
#include <string>
#include <vector>

#include "pokemon.h"

namespace mtm {
namespace pokemongo {

// Teams in game.
typedef enum {
	BLUE,
	YELLOW,
	RED,
} Team;

class Trainer {
public:
	// Constructs a new trainer with the given name and team.
	//
	// @param name the name of the trainer.
	// @param team the team to which the trainer belongs.
	// @throw TrainerInvalidArgsException if name is an empty string.
	Trainer(const std::string& name, const Team& team);

	// Returns a reference to the strongest Pokemon the trainer owns. Strongest 
	// Pokemon is determined using the comparison operators provided by the class
	// Pokemon. If two Pokemons are of equal strength, the function returns the
	// one that was caught earlier by the trainer.
	//
	// @return the strongest pokemon.
	// @throw TrainerNoPokemonsException if trainer has no Pokemons.
	Pokemon& GetStrongestPokemon();
	const Pokemon& GetStrongestPokemon() const;

	// Kills the strongest Pokemon. Removes the Pokemon that is returned from
	// GetStrongestPokemon() from the collection of Pokemons owned by the trainer
	//
	// @throw TrainerNoPokemonsException if trainer has no Pokemons.
	void KillStrongestPokemon();

	// Comparison operators for trainers. Trainers are compared as described in
	// the exercise sheet.
	//
	// @param rhs the right-hand side operand.
	bool operator==(const Trainer& rhs) const;
	bool operator!=(const Trainer& rhs) const;
	bool operator<(const Trainer& rhs) const;
	bool operator>(const Trainer& rhs) const;
	bool operator<=(const Trainer& rhs) const;
	bool operator>=(const Trainer& rhs) const;

	// Checks whether a given trainer is in the same team as this trainer.
	//
	// @param trainer
	// @return true iff trainer (argument) and this trainer belong to the same
	//         team.
	bool IsAlly(const Trainer& trainer) const;

	// Returns the team to which the trainer belongs.
	//
	// @return the team to which the trainer belongs.
	Team GetTeam() const;

	// Tries to catch a Pokemon.
	//
	// @param pokemon the Pokemon the trainer wishes to catch.
	// @return true if the attempt succeeded.
	bool TryToCatch(Pokemon& pokemon);

	// Prints the data of the trainer in the following format:
	//
	//     "<name> (<level>) <team>\n
	//      <pokemon1>\n
	//      <pokemon2>\n
	//      ..."
	//
	// where the Pokemons are ordered from least recently caught to most
	// recently. e.g.
	// 
	//     ash (4) YELLOW
	//     pikachu (3/5.5/87) NORMAL FIRE FLYING
	//     charmander (2/2.5/100) NORMAL BUG
	//     pikachu (7/6.5/56) FIRE
	// 
	//
	// @param output the output stream to which the data is printed.
	// @param pokemon the pokemon whose data is printed.
	// @return the output stream to which the data is printed.
	friend std::ostream& operator<<(std::ostream& output,
									const Trainer& trainer);

	// Part C functions
	int TotalScore();
	friend Trainer* TrainersBattle(Trainer trainer_1, Trainer trainer_2);

	// Part C members
	int gym_leader_counter;

private:

	int compareTrainer(const Trainer& rhs) const;

	// Part A members
	std::string name;
	Team team;
	int level;
	std::vector<Pokemon> pokemons;

	// Part C members
	std::string current_location_name;
	int battle_score_history;
};

Trainer* TrainersBattle(Trainer trainer_1, Trainer trainer_2);

std::ostream& operator<<(std::ostream& output, const Trainer& trainer);

}  // namespace mtm
}  // namespace pokemongo

#endif  // TRAINER_H
