#ifndef TRAINER_H
#define TRAINER_H

#include <iostream>
#include <string>
#include <vector>

#include "pokemon.h"
#include "item.h"

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

	// Destroys a trainer
	//
	// Destroys all of the trainer's items
	~Trainer();
	// Returns a reference to the strongest Pokemon the trainer owns. Strongest 
	// Pokemon is determined using the comparison operators provided by the
	// class Pokemon. If two Pokemons are of equal strength, the function
	// returns the one that was caught earlier by the trainer.
	//
	// @return the strongest pokemon.
	// @throw TrainerNoPokemonsException if trainer has no Pokemons.
	Pokemon& GetStrongestPokemon();
	const Pokemon& GetStrongestPokemon() const;

	// Kills the strongest Pokemon. Removes the Pokemon that is returned from
	// GetStrongestPokemon() from the collection of Pokemons owned by the
	// trainer.
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



	/*				Part C functions				*/

	// Perform a battle between 2 mightly trainers!
	//
	// @param trainer_1 first trainer in battle
	// @param trainer_2 second trainer in battle
	// @return pointer to the winning trainer!
	friend Trainer* TrainersBattle(Trainer& trainer_1, Trainer& trainer_2);


	// Helper function which performs a battle with 2 trainers with pokemons!
	// This function assumes both trainers have pokemons
	//
	// @param trainer_1 first trainer in battle
	// @param trainer_2 second trainer in battle
	// @return pointer to the winning trainer!
	friend Trainer* TrainersBattleWithPokemons(Trainer & trainer_1,
		Trainer & trainer_2);


	// Trainer's score for the team!
	//
	// @return total score the trainer is worth, taking into account
	//		   it's level, if he's gym leader and battle score history.
	int TotalScore();


	// Adds item to trainer's Inventory!
	//
	// @param the item to add
	// @return false if trainer cannot recieve the item due to it's level
	//		   true if item was added successfully
	bool AddItem(Item* item);


	/*				Part C Members					*/
	
	// Counter for how many gym trainer is their leader
	bool is_leader;

	// Trainer's current location name
	std::string current_location_name;

private:

	// Compare between 2 trainers, according to sheet instructions
	//
	// @param 2nd trainer to compare to
	// @return 0 if trainers are equal
	//		   1 if this trainer is better
	//		   -1 if rhs trainer is better
	int compareTrainer(const Trainer& rhs) const;


	/*				Part C functions				*/

	// Boost pokemon with oldest item
	// Doesn't do anything if trainer has no items
	//
	// @param Pokemon to boost
	void BoostPokemon(Pokemon& pokemon);

	// Raising Trainer's level after winning battle!
	//
	// @param The other loser trainer
	void RaiseLevel(Trainer& loser);

	// Updating Trainer's Battle Score Log
	//
	// @param The winner of the battle, 
	//		  if current instance is equal to winner score will be added
	//		  else score will be reduced
	void UpdateBattleScoreHistory(Trainer& winner);

	/*				Part A Members					*/

	// Trainer's name
	std::string name;

	// Trainer's Team
	Team team;

	// Trainer's level
	int level;

	// Trainer's list of pokemons
	std::vector<Pokemon> pokemons;

	/*				Part C Members					*/
	
	// Trainer's battle history score log
	int battle_score_history;

	// Trainer's Inventory
	std::vector<Item*> items;
};

Trainer* TrainersBattle(Trainer& trainer_1, Trainer& trainer_2);
	// TODO: This is ugly being here, but gcc complains otherwise
Trainer* TrainersBattleWithPokemons(Trainer& trainer_1, Trainer& trainer_2);
std::ostream& operator<<(std::ostream& output, const Trainer& trainer);

}  // namespace mtm
}  // namespace pokemongo

#endif  // TRAINER_H
