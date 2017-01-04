#include "pokedex_mocks.h"

Set demoTypes() {
  Set types;

  types = setCreate((copySetElements)stringCopy, (freeSetElements)free,
                    (compareSetElements)stringCompare);
  setAdd(types, "Fire");
  setAdd(types, "Electric");

  return types;
}

Pokedex demoPokedex() {
  Pokedex pokedex = pokedexCreate();
  Set types = demoTypes();
  pokedexAddPokemon(pokedex, "pikachu", 10, types);
  pokedexAddPokemon(pokedex, "Raichu", 7, types);
  pokedexAddPokemon(pokedex, "charmander", 12, types);
  pokedexAddPokemon(pokedex, "Charmelon", 15, types);
  pokedexAddPokemon(pokedex, "Charizard", 300, types);
  setAdd(types, "Ghost");
  pokedexAddPokemon(pokedex, "Bulbasaur", 11, types);
  pokedexAddPokemon(pokedex, "mew", 20, types);
  pokedexAddPokemon(pokedex, "mewtwo", 30, types);
  setDestroy(types);

  return pokedex;
}
