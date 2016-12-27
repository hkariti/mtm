#include "pokemon.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Pokemon_t {
	Map evolutions_map;
	PokedexEntry pokemon_info;
	double hp;
	int cp_extra;
	int level;
	int id;
};

Pokemon createPokemon(PokedexEntry pokemon_info, Evolutions evolutions_map)
{
	if (NULL == pokemon_info || NULL == evolutions_map) return NULL;
	Pokemon pokemon = malloc(sizeof(*pokemon));
	if (NULL == pokemon) return NULL;
	pokemon->pokemon_info = pokemon_info;
	pokemon->evolutions_map = evolutions_map;
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
	new_pokemon->evolutions_map = pokemon->evolutions_map;
	new_pokemon->hp = pokemon->hp;
	new_pokemon->id = pokemon->id;
	new_pokemon->level = pokemon->level;
	new_pokemon->pokemon_info = pokemon->pokemon_info;
	return new_pokemon;
}

void printPokemon(Pokemon pokemon, FILE* output_channel)
{
	assert(pokemon);
	assert(output_channel);
	char* pokemon_species = pokedexEntryGetSpecies(pokemon->pokemon_info);
	mtmPrintPokemon(output_channel, pokemon->id, pokemon_species,
		pokemon->hp, pokemonGetCP(pokemon), pokemon->level);
	free(pokemon_species);
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
	if (NULL == pokemon) return -1;
	return pokemon->cp_extra + pokedexEntryGetCp(pokemon->pokemon_info);
}

double pokemonGetHP(Pokemon pokemon)
{
	if (NULL == pokemon) return -1;
	return pokemon->hp;
}

int pokemonGetLevel(Pokemon pokemon)
{
	if (NULL == pokemon) return -1;
	return pokemon->level;
}

char* pokemonGetSpecies(Pokemon pokemon)
{
	if (NULL == pokemon) return NULL;
	return pokedexEntryGetSpecies(pokemon->pokemon_info);
}

PokemonErrorCode pokemonGiveCandy(Pokemon pokemon, int candy_value)
{
	if (NULL == pokemon) return POKEMON_INVALID_ARGUMENT;
	if (candy_value <= 0) return POKEMON_INVALID_ARGUMENT;
	pokemon->cp_extra += candy_value;
	return POKEMON_SUCCESS;
}

PokemonErrorCode pokemonGivePotion(Pokemon pokemon, int potion_value)
{
	if (NULL == pokemon) return POKEMON_INVALID_ARGUMENT;
	if (potion_value <= 0) return POKEMON_INVALID_ARGUMENT;

	if (pokemon->hp == MAX_POKEMON_HP) {
		return POKEMON_HP_IS_AT_MAX;
	}
	pokemon->hp += potion_value;
	if (pokemon->hp > MAX_POKEMON_HP) {
		pokemon->hp = MAX_POKEMON_HP;
	}
	return POKEMON_SUCCESS;
}

static int pokemonPokecoinsValue(Pokemon pokemon) { // need to verify all types are lowercase
	assert(pokemon);

	Set types = pokedexEntryGetTypes(pokemon->pokemon_info);
	int pokecoins_value = REGULAR_TYPE_POKECOINS;
	SET_FOREACH(char*, current_type, types) {
		if (0 == strcmp(current_type, "fire") ||
			0 == strcmp(current_type, "flying") ||
			0 == strcmp(current_type, "poison"))
		{
			setDestroy(types);
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
	setDestroy(types);
	return pokecoins_value;
}

int pokemonCaught(Pokemon pokemon, int new_pokemon_id)
{
	if (NULL == pokemon) return -1;
	if (new_pokemon_id <= 0) return -1;

	pokemon->id = new_pokemon_id;
	return pokemonPokecoinsValue(pokemon);
}

PokemonErrorCode pokemonBattle(Pokemon pokemon, Pokemon opponent_pokemon, double battle_delta)
{
	if (NULL == pokemon || NULL == opponent_pokemon) return POKEMON_INVALID_ARGUMENT;
	if (battle_delta < 0) return POKEMON_INVALID_ARGUMENT;

	pokemon->level += (pokemon->level + opponent_pokemon->level) / 4 +
		((pokemon->level + opponent_pokemon->level) % 4 != 0);

	pokemon->hp -= battle_delta;
	if (pokemon->hp < 0) {
		pokemon->hp = 0;
	}

	char* pokemon_spceies = pokedexEntryGetSpecies(pokemon->pokemon_info);
	PokedexEntry evolved_pokemon_info = getEvolution(pokemon->evolutions_map,
		pokemon_spceies, pokemon->level);
	while (evolved_pokemon_info != NULL) {
		pokemon->pokemon_info = evolved_pokemon_info;
		evolved_pokemon_info = getEvolution(pokemon->evolutions_map,
			pokemon_spceies, pokemon->level);
	}
}

PokemonErrorCode isPokemonDead(Pokemon pokemon, bool* is_dead)
{
	if (NULL == pokemon) return POKEMON_INVALID_ARGUMENT;
	if (NULL == is_dead) return POKEMON_INVALID_ARGUMENT;
	(*is_dead) = (pokemon->hp == 0);
	return POKEMON_SUCCESS;
}

