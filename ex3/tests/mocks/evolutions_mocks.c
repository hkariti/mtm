#include "evolutions_mocks.h"

Evolutions demoEvolutions() {
  Evolutions evolutions = createEvolutions();

  evolutionsAddEntry(evolutions, "pikachu", 2, (PokedexEntry)0xdeadbeef);
  evolutionsAddEntry(evolutions, "raichu", 1, (PokedexEntry)0xdeadbeef);
  evolutionsAddEntry(evolutions, "mew", 4, (PokedexEntry)0xdeadbeef);
  return evolutions;
}
