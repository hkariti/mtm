#include "test_utilities.h"
#include "../pokemon.h"
#include "../utils.h" //TODO: ugly
#include <string.h>
#include <stdlib.h>

Set demoTypes() {
	Set types;
	types = setCreate((copySetElements)stringCopy, (freeSetElements)free,
		(compareSetElements)strcmp);
	setAdd(types, "Fire");
	setAdd(types, "Electric");
	return types;
}

Pokedex demoPokedex() {
	Pokedex pokedex = createPokedex();
	Set types = demoTypes();
	pokedexAddPokemon(pokedex, "pikachu", 50, types);
	pokedexAddPokemon(pokedex, "raichu", 100, types);
	pokedexAddPokemon(pokedex, "charmander", 30, types);
	pokedexAddPokemon(pokedex, "charmelon", 30, types);
	pokedexAddPokemon(pokedex, "charizard", 800, types);
	pokedexAddPokemon(pokedex, "mew", 20, types);
	pokedexAddPokemon(pokedex, "mewtwo", 30, types);
	return pokedex;
}

Evolutions demoEvolutions(Pokedex pokedex) {
	Evolutions evolutions = createEvolutions();
	evolutionsAddEntry(evolutions, "pikachu", 2, pokedexGetPokemonInfo(pokedex, "raichu"));
	evolutionsAddEntry(evolutions, "charmander", 1, pokedexGetPokemonInfo(pokedex, "charmelon"));
	evolutionsAddEntry(evolutions, "charmelon", 1, pokedexGetPokemonInfo(pokedex, "charizard"));
	evolutionsAddEntry(evolutions, "mew", 4, pokedexGetPokemonInfo(pokedex, "mewtwo"));
	return evolutions;
}

Pokemon demoPokemon(Pokedex pokedex, Evolutions evolutions) {
	Pokemon pokemon = createPokemon(pokedexGetPokemonInfo(pokedex, "pikachu"), evolutions);
	return pokemon;
}

bool testCreatePokemon() {
	Pokedex pokedex = demoPokedex();
	PokedexEntry pokemon_info = pokedexGetPokemonInfo(pokedex, "mew");
	Evolutions evolutions = demoEvolutions(pokedex);
	Pokemon pokemon;

	// test null args
	pokemon = createPokemon(NULL, evolutions);
	ASSERT_TEST(NULL == pokemon);
	pokemon = createPokemon(NULL, NULL);
	ASSERT_TEST(NULL == pokemon);
	pokemon = createPokemon(pokemon_info, NULL);
	ASSERT_TEST(NULL == pokemon);

	// check valid args
	pokemon = createPokemon(pokemon_info, evolutions);
	ASSERT_TEST(NULL != pokemon);
	destroyPokemon(pokemon);

	destroyEvolutions(evolutions);
	destroyPokedex(pokedex);
	return true;
}

bool testDestroyPokemon() {
	Pokedex pokedex = demoPokedex();
	Evolutions evolutions = demoEvolutions(pokedex);
	Pokemon pokemon;

	// Test NULL argument
	destroyPokemon(NULL);

	// Test valid pokemon
	pokemon = demoPokemon(pokedex, evolutions);
	destroyPokemon(pokemon);

	destroyEvolutions(evolutions);
	destroyPokedex(pokedex);
	return true;
}

bool testPokemonCompareByID() {
	int compare_result;
	Pokedex pokedex = demoPokedex();
	Evolutions evolutions = demoEvolutions(pokedex);

	Pokemon pokemon_1 = demoPokemon(pokedex, evolutions);
	Pokemon pokemon_2 = demoPokemon(pokedex, evolutions);

	// Test equal pokemon
	compare_result = pokemonCompareByID(pokemon_1, pokemon_2);
	ASSERT_TEST(0 == compare_result);

	// Test unequal pokemon
	pokemonCaught(pokemon_1, 100);
	pokemonCaught(pokemon_2, 10);
	compare_result = pokemonCompareByID(pokemon_1, pokemon_2);
	ASSERT_TEST(1 == compare_result);

	destroyPokemon(pokemon_1);
	destroyPokemon(pokemon_2);

	destroyEvolutions(evolutions);
	destroyPokedex(pokedex);

	return true;
}

bool testPokemonGivePotion() {
	Pokedex pokedex = demoPokedex();
	Evolutions evolutions = demoEvolutions(pokedex);
	double hp, prev_hp;
	MtmErrorCode result;
	Pokemon pokemon = demoPokemon(pokedex, evolutions);
	Pokemon pokemon_2 = demoPokemon(pokedex, evolutions);

	// Test already at 100 hp
	result = pokemonGivePotion(pokemon, 1);
	hp = pokemonGetHP(pokemon);
	ASSERT_TEST(100 == hp);
	ASSERT_TEST(MTM_HP_IS_AT_MAX == result);

	// Test working scenario
	pokemonBattle(pokemon, pokemon_2, 1);
	prev_hp = pokemonGetHP(pokemon);
	result = pokemonGivePotion(pokemon, 1);
	hp = pokemonGetHP(pokemon);
	ASSERT_TEST(prev_hp + 1 == hp);
	ASSERT_TEST(MTM_SUCCESS == result);

	// Test over-heal
	result = pokemonGivePotion(pokemon, 1000);
	hp = pokemonGetHP(pokemon);
	ASSERT_TEST(100 == hp);
	ASSERT_TEST(MTM_SUCCESS == result);

	destroyPokemon(pokemon);
	destroyPokemon(pokemon_2);
	destroyEvolutions(evolutions);
	destroyPokedex(pokedex);

	return true;
}

bool testPokemonCaught() {
	Pokedex pokedex = demoPokedex();
	Evolutions evolutions = demoEvolutions(pokedex);
	Set types = setCreate((copySetElements)stringCopy, (freeSetElements)free,
		(compareSetElements)strcmp);
	int pokecoins;

	// Test regular types
	setAdd(types, "bug");
	setAdd(types, "ground");
	pokedexAddPokemon(pokedex, "weedle", 1000, types);
	Pokemon weedle = createPokemon(pokedexGetPokemonInfo(pokedex, "weedle"), evolutions);
	pokecoins = pokemonCaught(weedle, 10);
	destroyPokemon(weedle);
	ASSERT_TEST(pokecoins == REGULAR_TYPE_POKECOINS);

	// Test 2 one star types
	setAdd(types, "water");
	setAdd(types, "fairy");
	pokedexAddPokemon(pokedex, "tinkertoice", 1000, types);
	Pokemon tinkertoice = createPokemon(pokedexGetPokemonInfo(pokedex, "tinkertoice"), evolutions);
	pokecoins = pokemonCaught(tinkertoice, 10);
	destroyPokemon(tinkertoice);
	ASSERT_TEST(pokecoins == ONE_STAR_TYPE_POKECOINS);


	// Test 2 double star types
	setAdd(types, "fire");
	setAdd(types, "lying");
	pokedexAddPokemon(pokedex, "moltres", 1000, types);
	Pokemon moltres = createPokemon(pokedexGetPokemonInfo(pokedex, "moltres"), evolutions);
	pokecoins = pokemonCaught(moltres, 10);
	destroyPokemon(moltres);
	ASSERT_TEST(pokecoins == DOUBLE_STAR_TYPE_POKECOINS);

	setDestroy(types);
	destroyEvolutions(evolutions);
	destroyPokedex(pokedex);
	return true;
}

bool testCalculateBattleDelta() {
	Pokedex pokedex = demoPokedex();
	Evolutions evolutions = demoEvolutions(pokedex);
	Pokemon mew = createPokemon(pokedexGetPokemonInfo(pokedex,"mew"), evolutions);
	Pokemon mewtwo = createPokemon(pokedexGetPokemonInfo(pokedex, "mewtwo"), evolutions);
	double result;

	// Test mewtwo on mew battle
	result = calculateBattleDelta(mew, mewtwo, 2);
	ASSERT_TEST(90 == result);

	// Test mew on mewtwo battle
	result = calculateBattleDelta(mewtwo, mew, 3);
	ASSERT_TEST(90 == result);
	
	destroyPokemon(mew);
	destroyPokemon(mewtwo);
	destroyEvolutions(evolutions);
	destroyPokedex(pokedex);

	return true;
}

bool testPokemonBattle() {
	Pokedex pokedex = demoPokedex();
	Evolutions evolutions = demoEvolutions(pokedex);
	Pokemon mew = createPokemon(pokedexGetPokemonInfo(pokedex, "mew"), evolutions);
	Pokemon mewtwo = createPokemon(pokedexGetPokemonInfo(pokedex, "mewtwo"), evolutions);
	double hp;
	int level;

	// Test mewtwo on mew battle
	pokemonBattle(mew, mewtwo, 2);
	hp = pokemonGetHP(mew);
	level = pokemonGetLevel(mew);
	ASSERT_TEST(10 == hp);
	ASSERT_TEST(2 == level);

	// Test mew on mewtwo battle
	pokemonBattle(mewtwo, mew, 2);
	hp = pokemonGetHP(mewtwo);
	level = pokemonGetLevel(mewtwo);
	ASSERT_TEST(58 == hp);
	ASSERT_TEST(2 == level);

	destroyPokemon(mew);
	destroyPokemon(mewtwo);
	destroyEvolutions(evolutions);
	destroyPokedex(pokedex);

	return true;
}

int main() {
	RUN_TEST(testCreatePokemon);
	RUN_TEST(testDestroyPokemon);
	RUN_TEST(testPokemonCompareByID);
	RUN_TEST(testPokemonGivePotion);
	RUN_TEST(testPokemonCaught);
	RUN_TEST(testCalculateBattleDelta);
	RUN_TEST(testPokemonBattle);
	return 0;
}
