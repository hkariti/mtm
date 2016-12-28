#include "pokedex_mocks.h"

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
  pokedexAddPokemon(pokedex, "pikachu", 10, types);
  pokedexAddPokemon(pokedex, "Raichu", 12, types);
  setAdd(types, "Ghost");
  pokedexAddPokemon(pokedex, "Bulbasaur", 11, types);
  setDestroy(types);

  return pokedex;
}
