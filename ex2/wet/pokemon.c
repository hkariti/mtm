#include "pokemon.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_EXPERIENCE_VALUE	9901
#define INVALID_INDEX	-1
#define CRITICAL_ATTACK_FACTOR	2
#define NORMAL_ATTACK_FACTOR	1
#define FIRST_LOWERCASE_LETTER 'a'
#define FIRST_UPPERCASE_LETTER 'A'
#define LAST_LOWERCASE_LETTER 'z'
#define LAST_UPPERCASE_LETTER 'Z'
#define SEPERATION_LETTER	'-'
#define NULL_TERMINATION	'\0'

/**
* Checks if the supplied PokemonType is valid * 
* PokemonType is an Enum which has certain limited options of int values,
* However, every number can be casted to be a PokemonType and therefore,
* Invalidates the type design.
* Every function that has PokemonType as one of it's arguments, 
* needs to check if the PokemonType is valid. 
* The check is relying on the structure of the Enum, and therefore it's
* better to make it in a seperated function.
*
* @return
* 	false if type is invalid.
* 	true if type is valid
*/
bool isTypeValid(PokemonType type) {
	if (type < 0 || type > TYPE_ELECTRIC) return false;
	return true;
}

/**
* Initalize the Destination String With the Source String Content *
* This function presumes that the dest string provided is unallocated
* It then allocating memory for the destination string with the same amount
* as the source string has. Then, it copies the content of the source string
* into the dest.
*
* @return
* 	POKEMON_NULL_ARG if one of the args is NULL.
* 	POKEMON_OUT_OF_MEM if dest memory allocation failed
*	POKEMON_SUCCESS	when string initialization succeeded
*/
PokemonResult stringInit(char** src, char** dest) {
	if (NULL == src || NULL == dest || NULL == *src) return POKEMON_NULL_ARG;

	unsigned int len = strlen(*src);
	*dest = malloc(len + 1);
	if (NULL == *dest) {
		return POKEMON_OUT_OF_MEM;
	}
	strcpy(*dest, *src);
	return POKEMON_SUCCESS;
}

/**
* Cut out all non-letters character from the string *
* The function is provided a source string and makes a string identical
* to the source, but without all the non-letters characters
*
* @return
* 	NULL if the source string is NULL or if allocation failed.
* 	char* pointer to allocated non-letters string
*/
char* onlyLettersString(char* src) {
	if (NULL == src) return NULL;
	char* dest = malloc(strlen(src) + 1);
	if (NULL == dest) return NULL;
	unsigned int i = 0, j = 0;
	for (i = 0; i < strlen(src); i++) {
		char current_char = *(src + i);
		if (((current_char >= FIRST_LOWERCASE_LETTER) &&
			(current_char <= LAST_LOWERCASE_LETTER)) ||
			((current_char >= FIRST_UPPERCASE_LETTER) &&
			(current_char <= LAST_UPPERCASE_LETTER))) {
			*(dest + j) = current_char;
			j++;
		}
	}
	*(dest + j) = NULL_TERMINATION;
	return dest;
}

/**
* Finds a PokemonMove in the Pokemon Moves array *
* The function iterates through all the moves the pokemon knows and search
* for the move with the provdided name.
*
* @return
* 	INVALID_INDEX (==-1) if the pokemon doesn't have move with that name.
* 	int (non-negative number)-the found move's index in the moves array
*/
int findMoveByName(Pokemon pokemon, char* move_name) {
	for (int i = 0; i < pokemon->number_of_moves; i++) {
		if (0 == strcmp(pokemon->moves[i]->name, move_name)) {
			return i;
		}
	}
	return INVALID_INDEX;
}

/**
* Get the lowest lexicographical-ordered-name move the pokemon has *
* The function iterates through all the moves the pokemon knows and returns
* the Lowest lexicographical-order one. The comparison is done through
* strcmp function
*
* @return
* 	NULL if the pokemon doesn't know any moves.
* 	PokemonMove the Lowest lexicographical-order move the pokemon knows
*/
// return's lower lexicographic move, if no moves available returns NULL
PokemonMove getLowestLexicographicMove(Pokemon pokemon) {
	if (pokemon->number_of_moves == 0) return NULL;

	PokemonMove lowest_lexi_move = pokemon->moves[0];
	for (unsigned int i = 1; i < pokemon->number_of_moves; i++) {
		if (strcmp(lowest_lexi_move->name, pokemon->moves[i]->name) > 0) {
			lowest_lexi_move = pokemon->moves[i];
		}
	}
	return lowest_lexi_move;
}

int getMoveAttackFactor(PokemonMove move, Pokemon attacked_pokemon) {
	if (TYPE_WATER == move->type && TYPE_FIRE == attacked_pokemon->type) {
		return CRITICAL_ATTACK_FACTOR;
	}
	if (TYPE_FIRE == move->type && TYPE_GRASS == attacked_pokemon->type) {
		return CRITICAL_ATTACK_FACTOR;
	}
	if (TYPE_GRASS == move->type && TYPE_WATER == attacked_pokemon->type) {
		return CRITICAL_ATTACK_FACTOR;
	}
	if (TYPE_ELECTRIC == move->type && TYPE_WATER == attacked_pokemon->type) {
		return CRITICAL_ATTACK_FACTOR;
	}
	return NORMAL_ATTACK_FACTOR;
}


Pokemon pokemonCreate(char* name, PokemonType type, int experience,
	int max_number_of_moves) {

	if (NULL == name) return NULL;
	if (strlen(name) == 0) return NULL;
	if (!isTypeValid(type)) return NULL;
	if (experience > MAX_EXPERIENCE_VALUE || experience <= 0) return NULL; 
	if (max_number_of_moves <= 0) return NULL;

	char* name_clone;
	if (POKEMON_SUCCESS != stringInit(&name, &name_clone)) return NULL;

	Pokemon created_pokemon = malloc(sizeof(struct pokemon_t));
	if (NULL == created_pokemon) return NULL;

	created_pokemon->experience = experience;
	created_pokemon->name = name_clone;
	created_pokemon->max_number_of_moves = max_number_of_moves;
	created_pokemon->type = type;
	created_pokemon->number_of_moves = 0;
	created_pokemon->moves = malloc(sizeof(PokemonMove)*max_number_of_moves);

	pokemonHeal(created_pokemon);

	return created_pokemon;
}

void pokemonDestroy(Pokemon pokemon) {
	if (NULL == pokemon) return;

	free(pokemon->name);
	while (pokemon->number_of_moves > 0) {
		pokemonUnteachMove(pokemon, pokemon->moves[0]->name);
	}
	free(pokemon->moves);
	free(pokemon);
}

Pokemon pokemonCopy(Pokemon pokemon) {
	if (NULL == pokemon) return NULL;
	Pokemon new_pokemon = pokemonCreate(pokemon->name, pokemon->type,
		pokemon->experience, pokemon->max_number_of_moves);
	if (NULL == new_pokemon) return NULL;
	new_pokemon->health_points = pokemon->health_points;
    for (int i = 0; i < pokemon->number_of_moves; i++) {
        pokemonTeachMove(new_pokemon, pokemon->moves[i]->name, 
			pokemon->moves[i]->type, pokemon->moves[i]->max_power_points, 
			pokemon->moves[i]->strength);
    }
	return new_pokemon;
}	

PokemonResult pokemonTeachMove(Pokemon pokemon, char* move_name,
	PokemonType type, int max_power_points, int strength) { 

	if (NULL == pokemon || NULL == move_name) return POKEMON_NULL_ARG;
	if (strlen(move_name) == 0) return POKEMON_INVALID_MOVE_NAME;
	if (!isTypeValid(type)) return POKEMON_INVALID_TYPE;
	if (max_power_points <= 0) return POKEMON_INVALID_POWER_POINTS;
	if (strength <= 0) return POKEMON_INVALID_STRENGTH;
	if (TYPE_NORMAL != type && pokemon->type != type) {
		return POKEMON_INCOMPATIBLE_MOVE_TYPE;
	}
	if (INVALID_INDEX != findMoveByName(pokemon, move_name)) {
		return POKEMON_MOVE_ALREADY_EXISTS;
	}
	PokemonMove new_move = malloc(sizeof(struct pokemon_move_t));
	if (NULL == new_move) return POKEMON_OUT_OF_MEM;
	if (POKEMON_SUCCESS != stringInit(&move_name, &new_move->name)) {
		free(new_move);
		return POKEMON_OUT_OF_MEM;
	}
	new_move->max_power_points = max_power_points;
	new_move->strength = strength;
	new_move->type = type;
	new_move->power_points = max_power_points;
	if (pokemon->number_of_moves == pokemon->max_number_of_moves) {
		PokemonMove lowest_lexico_move = getLowestLexicographicMove(pokemon);
		pokemonUnteachMove(pokemon, lowest_lexico_move->name);
	}
	pokemon->moves[pokemon->number_of_moves] = new_move;
	pokemon->number_of_moves++;
	return POKEMON_SUCCESS;
}

PokemonResult pokemonUnteachMove(Pokemon pokemon, char* move_name) {
	if (NULL == pokemon || NULL == move_name) return POKEMON_NULL_ARG;
	if (strlen(move_name) == 0) return POKEMON_INVALID_MOVE_NAME;
	int move_to_unteach_index = findMoveByName(pokemon, move_name);
	if (INVALID_INDEX == move_to_unteach_index) {
		return POKEMON_MOVE_DOES_NOT_EXIST;
	}

	free(pokemon->moves[move_to_unteach_index]->name);
	free(pokemon->moves[move_to_unteach_index]);

	// If move is not the last node in array
	if (move_to_unteach_index < pokemon->number_of_moves - 1) {
		//	Squeeze the array by moving the last node to the node index
		//  we just removed.	we can do it (and not decrease all moves
		//	indexes) because the moves order in the array is not important
		pokemon->moves[move_to_unteach_index] = 
			pokemon->moves[pokemon->number_of_moves - 1];
	}
	pokemon->number_of_moves--;
	return POKEMON_SUCCESS;
}

int pokemonGetLevel(Pokemon pokemon) {
	assert(pokemon);
	int level = pokemon->experience / 100;
	if (0 != pokemon->experience % 100) {
		level++;
	}
	return level;
}

int pokemonGetRank(Pokemon pokemon) {
	assert(pokemon);
	if (0 == pokemon->number_of_moves) {
		return 0;
	}
	int average_move_strength = 0;
	for (int i = 0; i < pokemon->number_of_moves; i++) {
		average_move_strength += pokemon->moves[i]->strength;
	}
	average_move_strength /= pokemon->number_of_moves;
	return average_move_strength + pokemonGetLevel(pokemon);
}

PokemonResult pokemonUseMove(Pokemon pokemon, Pokemon opponent_pokemon, 
	char* move_name) {

	if (NULL == pokemon || NULL == opponent_pokemon || NULL == move_name) {
		return POKEMON_NULL_ARG;
	}
	if (!strlen(move_name)) return POKEMON_INVALID_MOVE_NAME;
	if (INVALID_INDEX == findMoveByName(pokemon, move_name)) {
		return POKEMON_MOVE_DOES_NOT_EXIST;
	}
	PokemonMove move = pokemon->moves[findMoveByName(pokemon, move_name)];
	if (0 == move->power_points) return POKEMON_NO_POWER_POINTS;
	if (0 >= pokemon->health_points || 0 >= opponent_pokemon->health_points) {
		return POKEMON_NO_HEALTH_POINTS;
	}
	int factor = getMoveAttackFactor(move, opponent_pokemon);
	int attack_points = factor*(pokemonGetLevel(pokemon)*2 + move->strength);
	if (attack_points > opponent_pokemon->health_points) {
		attack_points = opponent_pokemon->health_points;
	}
	opponent_pokemon->health_points -= attack_points;
	pokemon->experience += attack_points;
	if (pokemon->experience > MAX_EXPERIENCE_VALUE) {
		pokemon->experience = MAX_EXPERIENCE_VALUE;
	}
	move->power_points--;
	return POKEMON_SUCCESS;
}

PokemonResult pokemonHeal(Pokemon pokemon) {
	if (NULL == pokemon) return POKEMON_NULL_ARG;
	pokemon->health_points = (100 + pokemonGetLevel(pokemon)) * 10;
	for (int i = 0; i < pokemon->number_of_moves; i++) {
		pokemon->moves[i]->power_points = pokemon->moves[i]->max_power_points;
	}
	return POKEMON_SUCCESS;
}

PokemonResult pokemonEvolve(Pokemon pokemon, char* new_name) {
	if (NULL == pokemon || NULL == new_name) return POKEMON_NULL_ARG;
	if (!strlen(new_name)) return POKEMON_INVALID_NAME;
	if (MAX_EXPERIENCE_VALUE <= pokemon->experience) {
		return POKEMON_CANNOT_EVOLVE;
	}

	int prev_experience = pokemon->experience;
	int previous_level = pokemonGetLevel(pokemon);
	while (previous_level == pokemonGetLevel(pokemon)) {
		pokemon->experience++;
	}

	char* new_name_clone;
	if (stringInit(&new_name, &new_name_clone) != POKEMON_SUCCESS) {
		pokemon->experience = prev_experience;
		return POKEMON_OUT_OF_MEM;
	}

	free(pokemon->name);
	pokemon->name = new_name_clone;
	return POKEMON_SUCCESS;
}

PokemonResult pokemonPrintName(Pokemon pokemon, FILE* file) {
	if (NULL == pokemon || NULL == file) return POKEMON_NULL_ARG;

	fputs(pokemon->name, file); 

	return POKEMON_SUCCESS;
}

PokemonResult pokemonPrintVoice(Pokemon pokemon, FILE* file) {
	if (NULL == pokemon || NULL == file) return POKEMON_NULL_ARG;
	char* only_letters_name = onlyLettersString(pokemon->name);
	if (NULL == only_letters_name) {
		return POKEMON_OUT_OF_MEM;
	}
	int voice_half_length = strlen(only_letters_name);
	if (voice_half_length >= 4) {
		voice_half_length = voice_half_length / 2 + voice_half_length % 2;
	}
	int voice_length = voice_half_length * 2 + 1; //2 halfs+seperation letter
	char* pokemon_voice = malloc(voice_length + 1);
	if (NULL == pokemon_voice) {
		free(only_letters_name);
		return POKEMON_OUT_OF_MEM;
	}

	memcpy(pokemon_voice, only_letters_name, voice_half_length);
	memcpy(pokemon_voice + voice_half_length + 1, only_letters_name,
		voice_half_length);
	pokemon_voice[voice_half_length] = SEPERATION_LETTER;
	pokemon_voice[voice_length] = NULL_TERMINATION;
	fputs(pokemon_voice, file);
    free(pokemon_voice);
	free(only_letters_name);

	return POKEMON_SUCCESS;
}