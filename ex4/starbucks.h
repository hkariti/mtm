#pragma once
#include "location.h"
#include "exceptions.h"
#include "trainer.h"
#include "pokemon.h"

namespace mtm {
	namespace pokemongo {

		class Starbucks : public Location {

			std::vector<Pokemon> pokemons;

		public:
			// main ctor which provided vector of pokemons
			Starbucks(const std::vector<Pokemon> pokemons);

			~Starbucks() {}

			// make copy and assignment operator as compiler's default
			Starbucks(const Starbucks& gym) = default;
			Starbucks& operator=(const Starbucks& gym) = default;

			// throws LocationTrainerAlreadyInLocationException
			void Arrive(Trainer& trainer) override;

			// throws LocationTrainerNotFoundException
			void Leave(Trainer& trainer) override;

		};

	}
}