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

			// Constructs a new Starbucks coffee shop 
			// with pokemons and no coffee!
			//
			// @param pokemons the list of pokemons in the shop
			Starbucks(const std::vector<Pokemon> pokemons);

			~Starbucks() {}

			// make copy and assignment operator as compiler's default
			Starbucks(const Starbucks& gym) = default;
			Starbucks& operator=(const Starbucks& gym) = default;

			// Handle a new trainer arriving to Starbucks,
			// Buying coffee and try to catch the first pokemon he sees!
			//
			// @param trainer the trainer arriving.
			// @throw LocationTrainerAlreadyInLocationException 
			//			if trainer is already in the location.
			void Arrive(Trainer& trainer) override;

			// Handle an old trainer leaving the starbucks for a new adventure
			//
			// @param trainer the trainer leaving.
			// @throw LocationTrainerNotFoundException if trainer is not
			//		  in the location.
			void Leave(Trainer& trainer) override;

		};

	}
}