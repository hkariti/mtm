#include <pokemon.h>

Pokemon demoPokemon(Pokedex pokedex, Evolutions evolutions) {
	Pokemon pokemon = createPokemon(pokedexGetPokemonInfo(pokedex, "pikachu"), evolutions);
	return pokemon;
}
