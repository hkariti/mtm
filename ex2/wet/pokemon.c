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
#define NULL_CHARACTER	'\0'


bool isTypeValid(PokemonType type) {
	if (type < 0 || type > TYPE_ELECTRIC) return false; //UGLY: other way to check if type is ok?
	return true;
}

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

Pokemon pokemonCreate(char* name, PokemonType type, int experience,
	int max_number_of_moves) {

	if (NULL == name) return NULL;
	if (strlen(name) == 0) return NULL;
	if (!isTypeValid(type)) return NULL;
	if (experience > MAX_EXPERIENCE_VALUE || experience < 0) return NULL; // what about 0?
	if (max_number_of_moves < 0) return NULL; // what about 0?

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
	Pokemon new_pokemon = pokemonCreate(pokemon->name, pokemon->type, pokemon->experience, pokemon->max_number_of_moves);
	if (NULL == new_pokemon) return NULL;
	new_pokemon->health_points = pokemon->health_points;
    for (int i = 0; i < pokemon->number_of_moves; i++) {
        pokemonTeachMove(new_pokemon, pokemon->moves[i]->name, pokemon->moves[i]->type, pokemon->moves[i]->max_power_points, pokemon->moves[i]->strength);
    }
	return new_pokemon;
}	

// returns move's index in moves array. if doesn't exist returns -1
int findMoveByName(Pokemon pokemon, char* move_name) {

	for (int i = 0; i < pokemon->number_of_moves; i++) {
		if (0 == strcmp(pokemon->moves[i]->name, move_name)) {
			return i;
		}
	}
	return INVALID_INDEX;
}

// return's lower lexicographic move, if no moves available returns NULL
PokemonMove getLowestLexicographicMove(Pokemon pokemon) {

	if (pokemon->number_of_moves == 0) return NULL;

	PokemonMove lowest_lexicographic_move = pokemon->moves[0];
	for (unsigned int i = 1; i < pokemon->number_of_moves; i++) {
		if (strcmp(lowest_lexicographic_move->name, pokemon->moves[i]->name) > 0) {
			lowest_lexicographic_move = pokemon->moves[i];
		}
	}
	return lowest_lexicographic_move;
}

PokemonResult pokemonTeachMove(Pokemon pokemon, char* move_name,
	PokemonType type, int max_power_points, int strength) { //TODO: if max_number_of_moves can be 0!??

	if (NULL == pokemon || NULL == move_name) return POKEMON_NULL_ARG;
	if (strlen(move_name) == 0) return POKEMON_INVALID_MOVE_NAME;
	if (!isTypeValid(type)) return POKEMON_INVALID_TYPE;
	if (max_power_points < 0) return POKEMON_INVALID_POWER_POINTS; //Q: what about 0??
	if (strength < 0) return POKEMON_INVALID_STRENGTH; // what about 0?
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
	if (INVALID_INDEX == move_to_unteach_index) return POKEMON_MOVE_DOES_NOT_EXIST;

	free(pokemon->moves[move_to_unteach_index]->name);
	free(pokemon->moves[move_to_unteach_index]);
	// if move is not the last node in array
	if (move_to_unteach_index < pokemon->number_of_moves - 1) {
		// squeeze the array by moving the last node to the node index we just removed.
		pokemon->moves[move_to_unteach_index] = pokemon->moves[pokemon->number_of_moves - 1];
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

PokemonResult pokemonUseMove(Pokemon pokemon, Pokemon opponent_pokemon, char* move_name) {
	if (NULL == pokemon || NULL == opponent_pokemon || NULL == move_name) return POKEMON_NULL_ARG;
	if (!strlen(move_name)) return POKEMON_INVALID_MOVE_NAME;
	PokemonMove move = pokemon->moves[findMoveByName(pokemon, move_name)];
	if (NULL == move) return POKEMON_MOVE_DOES_NOT_EXIST;
	if (0 == move->power_points) return POKEMON_NO_POWER_POINTS;
	if (0 == pokemon->health_points || 0 == opponent_pokemon->health_points) {
		return POKEMON_NO_HEALTH_POINTS;
	}
	int factor = getMoveAttackFactor(move, opponent_pokemon);
	int attack_points = factor * (pokemonGetLevel(pokemon) * 2 + move->strength);
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

PokemonResult pokemonHeal(Pokemon pokemon) { //Q: What if current hp is high enoguh??
	if (NULL == pokemon) return POKEMON_NULL_ARG;
	pokemon->health_points = (100 + pokemonGetLevel(pokemon)) * 10; //TOOD: defines?
	for (int i = 0; i < pokemon->number_of_moves; i++) {
		pokemon->moves[i]->power_points = pokemon->moves[i]->max_power_points;
	}
	return POKEMON_SUCCESS;
}

PokemonResult pokemonEvolve(Pokemon pokemon, char* new_name) {
	if (NULL == pokemon || NULL == new_name) return POKEMON_NULL_ARG;
	if (!strlen(new_name)) return POKEMON_INVALID_NAME;

	int prev_experience = pokemon->experience;
	int previous_level = pokemonGetLevel(pokemon);
	while (previous_level == pokemonGetLevel(pokemon)) {
		pokemon->experience++;
		if (MAX_EXPERIENCE_VALUE <= pokemon->experience) {
			pokemon->experience = prev_experience;
			return POKEMON_CANNOT_EVOLVE;
		}
	}

	char* new_name_clone;
	stringInit(&new_name, &new_name_clone);
	if (NULL == new_name_clone) {
		pokemon->experience = prev_experience;
		return POKEMON_OUT_OF_MEM;
	}

	free(pokemon->name);
	pokemon->name = new_name_clone;
	return POKEMON_SUCCESS;
}

PokemonResult pokemonPrintName(Pokemon pokemon, FILE* file) {	//Q: need to check if file as write access?
	if (NULL == pokemon || NULL == file) return POKEMON_NULL_ARG;

	fputs(pokemon->name, file); //Q: need to add \n ?

	return POKEMON_SUCCESS;
}

char* removeNonLetterChars(char* src) {
	char* dest = malloc(strlen(src) + 1);
	if (NULL == dest) return NULL;
	unsigned int i = 0, j = 0;
	for (i = 0; i < strlen(src); i++) {
		char current_char = *(src + i);
		if (((current_char >= FIRST_LOWERCASE_LETTER) && (current_char <= LAST_LOWERCASE_LETTER)) ||
			((current_char >= FIRST_UPPERCASE_LETTER) && (current_char <= LAST_UPPERCASE_LETTER))) {
			*(dest + j) = current_char;
            j++;
		}
	}
	*(dest + j) = NULL_CHARACTER;
	return dest;
}

PokemonResult pokemonPrintVoice(Pokemon pokemon, FILE* file) { //Q: pokemon name can be empty?? Q: file access checking?
	if (NULL == pokemon || NULL == file) return POKEMON_NULL_ARG;
	char* only_letters_name = removeNonLetterChars(pokemon->name);
	if (NULL == only_letters_name) {
		return POKEMON_OUT_OF_MEM;
	}
	int voice_half_length = strlen(only_letters_name);
	if (voice_half_length >= 4) {
		voice_half_length = voice_half_length / 2 + voice_half_length % 2;
	}
	int voice_length = voice_half_length * 2 + 1; // 2 halfs + seperation letter
	char* pokemon_voice = malloc(voice_length + 1);
	if (NULL == pokemon_voice) {
		return POKEMON_OUT_OF_MEM;
	}

	memcpy(pokemon_voice, only_letters_name, voice_half_length);
	memcpy(pokemon_voice + voice_half_length + 1, only_letters_name, voice_half_length);
	pokemon_voice[voice_half_length] = SEPERATION_LETTER;
	pokemon_voice[voice_length] = NULL_CHARACTER;
	fputs(pokemon_voice, file); //Q: need to add \n ?
    free(pokemon_voice);
	free(only_letters_name);

	return POKEMON_SUCCESS;
}

int main0() {
	char *str ="";
	char* dest;
	Pokemon pokemon = pokemonCreate("Hohoyobogo", (PokemonType)10, 101, 30);
	//Pokemon pokemon = pokemonCreate("Hohoyobogo", TYPE_GRASS, 101, 30);
	pokemonPrintVoice(pokemon, fopen("text.txt","w"));
	printf("strlen %d\n",strlen(""));
	printf("%d\n", pokemonGetLevel(pokemon));
	printf("%d", pokemonTeachMove(pokemon, "Hit", TYPE_GRASS, 10, 3));
	printf("%d", pokemonUnteachMove(pokemon, "Hit"));
	pokemonDestroy(pokemon);
	printf("%d\n", stringInit(&str, &dest));
	printf("strlen %d\n", strlen(str));
	printf("string: %s\n", dest);
	return 0;
 }
