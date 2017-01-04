#include "evolutions_mocks.h"

Evolutions demoEvolutions(Pokedex pokedex) {
  Evolutions evolutions = evolutionsCreate();
  PokedexEntry entry;

  entry = pokedexGetPokemonInfo(pokedex, "Raichu");
  evolutionsAddEntry(evolutions, "pikachu", 2, entry);;
  entry = pokedexGetPokemonInfo(pokedex, "Ghost");
  evolutionsAddEntry(evolutions, "raichu", 1, entry);
  entry = pokedexGetPokemonInfo(pokedex, "Bulbasaur");
  evolutionsAddEntry(evolutions, "mew", 4, entry);
  return evolutions;
}
