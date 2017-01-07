#include "pokemon_mocks.h"

Pokemon demoPokemon(Pokedex pokedex, Evolutions evolutions) {
	Pokemon pokemon;
    pokemon = pokemonCreate(pokedexGetPokemonInfo(pokedex, "pikachu"),
                            evolutions);
	return pokemon;
}
