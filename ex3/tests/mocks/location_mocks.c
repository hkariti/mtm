#include "location_mocks.h"

Location demoLocationWithPokemon(char* name, int num_of_pokemon, Pokedex pokedex,
                                 Evolutions evolutions) {
  Location location = createLocation(name);
  Pokemon pokemon = demoPokemon(pokedex, evolutions);

  for (int i = 0; i < num_of_pokemon; i++) {
    locationAppendPokemon(location, pokemon);
  }

  pokemonDestroy(pokemon);
  return location;
}
