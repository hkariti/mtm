#include "pokemon.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Pokemon_t {
	Map evolutions_list;
	PokedexEntry pokemon_info;
	double hp;
	int cp_extra;
	int level;
	int id;
};

Pokemon createPokemon(PokedexEntry pokemon_info, Evolutions evolutions_list)
{
	if (NULL == pokemon_info || NULL == evolutions_list) return NULL;
	Pokemon pokemon = malloc(sizeof(*pokemon));
	if (NULL == pokemon) return NULL;
	pokemon->pokemon_info = pokemon_info;
	pokemon->evolutions_list = evolutions_list;
	pokemon->hp = MAX_POKEMON_HP;
	pokemon->level = START_POKEMON_LEVEL;
	pokemon->cp_extra = 0;
	pokemon->id = UNASSIGNED_POKEMON_ID;
	return pokemon;
}

void destroyPokemon(Pokemon pokemon)
{
	free(pokemon);
}

Pokemon copyPokemon(Pokemon pokemon) {
	if (NULL == pokemon) return NULL;
	Pokemon new_pokemon = malloc(sizeof(*new_pokemon));
	if (NULL == new_pokemon) return NULL;
	new_pokemon->cp_extra = pokemon->cp_extra;
	new_pokemon->evolutions_list = pokemon->evolutions_list;
	new_pokemon->hp = pokemon->hp;
	new_pokemon->id = pokemon->id;
	new_pokemon->level = pokemon->level;
	new_pokemon->pokemon_info = pokemon->pokemon_info;
	return new_pokemon;
}

void printPokemon(Pokemon pokemon, FILE* output_file)
{
	assert(pokemon);
	assert(output_file);
	mtmPrintPokemon(output_file, pokemon->id, pokedexEntryGetSpecies(pokemon->pokemon_info),
		pokemon->hp, pokemonGetCP(pokemon), pokemon->level);
}

int pokemonCompareByID(Pokemon pokemon_1, Pokemon pokemon_2)
{
	assert(pokemon_1);
	assert(pokemon_2);
	if (pokemon_1->id == pokemon_2->id) {
		return 0;
	}
	else if (pokemon_1->id > pokemon_2->id) {
		return 1; //is 1 or -1?
	}
	else {
		return -1;
	}
}

int pokemonGetCP(Pokemon pokemon)
{
	assert(pokemon);
	return pokemon->cp_extra + pokedexEntryGetCp(pokemon->pokemon_info);
}

double pokemonGetHP(Pokemon pokemon)
{
	assert(pokemon);
	return pokemon->hp;
}

int pokemonGetLevel(Pokemon pokemon)
{
	assert(pokemon);
	return pokemon->level;
}

char* pokemonGetSpecies(Pokemon pokemon)
{
	assert(pokemon);
	return pokedexEntryGetSpecies(pokemon->pokemon_info);
}

void pokemonGiveCandy(Pokemon pokemon, int candy_value)
{
	assert(pokemon);
	assert(candy_value > 0);
	pokemon->cp_extra += candy_value;
}

MtmErrorCode pokemonGivePotion(Pokemon pokemon, int potion_value)
{
	assert(pokemon);
	assert(potion_value > 0);
	if (pokemon->hp == MAX_POKEMON_HP) {
		return MTM_HP_IS_AT_MAX;
	}
	pokemon->hp += potion_value;
	if (pokemon->hp > MAX_POKEMON_HP) {
		pokemon->hp = MAX_POKEMON_HP;
	}
	return MTM_SUCCESS;
}

int pokemonPokecoinsValue(Pokemon pokemon) { // private function
	assert(pokemon);

	Set types = pokedexEntryGetTypes(pokemon->pokemon_info);
	int pokecoins_value = REGULAR_TYPE_POKECOINS;
	SET_FOREACH(char*, current_type, types) {
		if (0 == strcmp(current_type, "fire") ||
			0 == strcmp(current_type, "flying") ||
			0 == strcmp(current_type, "poison"))
		{
			return DOUBLE_STAR_TYPE_POKECOINS;
		}
		else if (0 == strcmp(current_type, "rock") ||
			0 == strcmp(current_type, "electric") ||
			0 == strcmp(current_type, "water") ||
			0 == strcmp(current_type, "fairy") ||
			0 == strcmp(current_type, "ice"))
		{
			pokecoins_value = ONE_STAR_TYPE_POKECOINS;
		}
	}
	return pokecoins_value;
}

int pokemonCaught(Pokemon pokemon, int new_pokemon_id)
{
	assert(pokemon);
	assert(new_pokemon_id > 0);

	pokemon->id = new_pokemon_id;
	return pokemonPokecoinsValue(pokemon);
}

double calculateBattleDelta(Pokemon pokemon, Pokemon opponent_pokemon, double opponent_trainer_xp) {
	assert(pokemon);
	assert(opponent_pokemon);
	assert(opponent_trainer_xp > 0);

	return opponent_trainer_xp * pokemonGetCP(opponent_pokemon) * (1 + opponent_pokemon->hp / 200);
}

void pokemonBattle(Pokemon pokemon, Pokemon opponent_pokemon, double opponent_trainer_xp)
{
	assert(pokemon);
	assert(opponent_pokemon);
	assert(opponent_trainer_xp > 0);

	pokemon->level += (pokemon->level + opponent_pokemon->level) / 4 +
		((pokemon->level + opponent_pokemon->level) % 4 != 0);

	pokemon->hp -= calculateBattleDelta(pokemon, opponent_pokemon, opponent_trainer_xp);
	if (pokemon->hp < 0) {
		pokemon->hp = 0;
	}

	PokedexEntry evolved_pokemon_info = getEvolution(pokemon->evolutions_list,
		pokedexEntryGetSpecies(pokemon->pokemon_info), pokemon->level);
	if (evolved_pokemon_info != NULL) {
		pokemon->pokemon_info = evolved_pokemon_info;
	}
}

bool isPokemonDead(Pokemon pokemon)
{
	assert(pokemon);

	return (pokemon->hp == 0);
}

