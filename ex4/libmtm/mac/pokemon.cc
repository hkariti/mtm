#include "pokemon.h"

#include <set>
#include <stdexcept>
#include <string>

#include "exceptions.h"

#define NUM_TYPES 14

using std::set;
using std::string;

namespace mtm {
namespace pokemongo {

set<PokemonType> Pokemon::GetDefaultTypes(const string& species) {
  int hash = 7;
  const int primes[14] = {5, 7, 7, 11, 11, 17, 17, 17, 17, 19, 19, 19, 19, 19};

  for (int i = 0; i < species.size(); i++) {
      hash = hash*31 + species.at(i);
  }
  set<PokemonType> types;
  for (int i = 0; i < NUM_TYPES; ++i) {
    if (hash % primes[i] == i % primes[i] ||
        hash % primes[i] == (i + 1) % primes[i] ||
        hash % primes[i] == (i + 2) % primes[i] ||
        hash % primes[i] == (i + 3) % primes[i]) {
      types.insert(static_cast<PokemonType>(i));
    }
  }
  return types;
}

}  // namespace pokemongo
}  // namespace mtm