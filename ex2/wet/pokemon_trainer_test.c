#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "aux_macros.h"
#include "pokemon_trainer.h"

#define TRAINER_MAX_LOCAL 10
#define TRAINER_MAX_REMOTE 10

static Pokemon newPokemon(char* name) {
    return pokemonCreate(name, TYPE_ELECTRIC, 10, 4);
}

static void addMove(Pokemon pokemon, int strength) {
    pokemonTeachMove(pokemon, "Move", TYPE_ELECTRIC, 10, strength);
}

static void fillWithdrawList(PokemonTrainer trainer) {
     char *name = malloc(15);
     for (int i = 1; i <= TRAINER_MAX_REMOTE; i++) {
         snprintf(name, 15, "Remote%d", i);
         Pokemon pokemon = newPokemon(name);
         pokemonTrainerAddPokemon(trainer, pokemon);
         pokemonTrainerDepositPokemon(trainer, 2);
         pokemonDestroy(pokemon);
     }
     free(name);
}

static void fillPokemonList(PokemonTrainer trainer) {
     char *name = malloc(15);
     for (int i = 2; i <= TRAINER_MAX_LOCAL; i++) {
         snprintf(name, 15, "Local%d", i);
         Pokemon pokemon = newPokemon(name);
         pokemonTrainerAddPokemon(trainer, pokemon);
         pokemonDestroy(pokemon);
     }
     free(name);
}

static PokemonTrainer newTrainer() {
    Pokemon pikachu = newPokemon("Pikachu");
    PokemonTrainer trainer = pokemonTrainerCreate("Ash", pikachu, TRAINER_MAX_LOCAL, TRAINER_MAX_REMOTE);

    pokemonDestroy(pikachu);
    return trainer;
}

static bool testPokemonTrainerCreate() {
    bool result = true;
    char* name = "Ash";
    PokemonTrainer trainer;
    Pokemon pikachu = newPokemon("Pikachu");

    // Test null args
    trainer = pokemonTrainerCreate(NULL, pikachu, 10, 10);
    TEST_EQUALS(result, trainer, NULL); 
    trainer = pokemonTrainerCreate("", pikachu, 10, 10);
    TEST_EQUALS(result, trainer, NULL); 
    trainer = pokemonTrainerCreate(name, NULL, 10, 10);
    TEST_EQUALS(result, trainer, NULL); 
    trainer = pokemonTrainerCreate(name, pikachu, 0, 10);
    TEST_EQUALS(result, trainer, NULL); 
    trainer = pokemonTrainerCreate(name, pikachu, 10, 0);
    TEST_EQUALS(result, trainer, NULL); 

    // Verify basic things
    trainer = pokemonTrainerCreate("Ash", pikachu, 10, 10);
    TEST_DIFFERENT(result, trainer, NULL);
    TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 1);

    pokemonDestroy(pikachu);
    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerCopy() {
    bool result = true;

    PokemonTrainer trainer1 = newTrainer();
    PokemonTrainer trainer2;

    // Check null argument
    trainer2 = pokemonTrainerCopy(NULL);
    TEST_EQUALS(result, trainer2, NULL);

    // Fill up pokemon list
    fillWithdrawList(trainer1);
    fillPokemonList(trainer1);

    // Test basic things
    trainer2 = pokemonTrainerCopy(trainer1);
    TEST_DIFFERENT(result, trainer2, NULL);
    int pokemon_count1, pokemon_count2;
    pokemon_count1 = pokemonTrainerGetNumberOfPokemons(trainer1);
    pokemon_count2 = pokemonTrainerGetNumberOfPokemons(trainer2);
    TEST_EQUALS(result, pokemon_count1, pokemon_count2);

    // Test local pokemon exist and are copies of the originals
    for (int i = 1; i <= TRAINER_MAX_LOCAL; i++) {
        Pokemon pokemon1, pokemon2;
        pokemon1 = pokemonTrainerGetPokemon(trainer1, i);
        pokemon2 = pokemonTrainerGetPokemon(trainer2, i);
        TEST_DIFFERENT(result, pokemon2, NULL);
        TEST_DIFFERENT(result, pokemon1, pokemon2);
    }

    pokemonTrainerDestroy(trainer1);
    pokemonTrainerDestroy(trainer2);

    return result;
}

static bool testPokemonTrainerAddPokemon() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon pikachu;
    PokemonTrainerResult ret;

    pikachu = newPokemon("Pikachu");
    // Test NULL trainer/pokemon
    ret = pokemonTrainerAddPokemon(NULL, pikachu);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);
    ret = pokemonTrainerAddPokemon(trainer, NULL);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);
    pokemonDestroy(pikachu);

    // Test normal usage
    char *name = malloc(10);
    for (int i = 2; i <= 10; i++) {
        snprintf(name, 10, "Pikachu%d", i);
        pikachu = newPokemon(name);
        ret = pokemonTrainerAddPokemon(trainer, pikachu);
        TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
        // Test we added a copy
        TEST_DIFFERENT(result, pokemonTrainerGetPokemon(trainer, i), pikachu);
        pokemonDestroy(pikachu);
    }
    free(name);

    // Test party full
    pikachu = pokemonCreate("Pikachu11", TYPE_ELECTRIC, 20, 4);
    ret = pokemonTrainerAddPokemon(trainer, pikachu);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_PARTY_FULL);
    pokemonDestroy(pikachu);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerRemovePokemon() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    PokemonTrainerResult ret;
    Pokemon pikachu2;

    // Test NULL trainer
    ret = pokemonTrainerRemovePokemon(NULL, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);

    // Test bounds
    ret = pokemonTrainerRemovePokemon(trainer, 0);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);
    ret = pokemonTrainerRemovePokemon(trainer, 2);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);

    // Test minimum check
    ret = pokemonTrainerRemovePokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_REMOVE_LAST);

    pikachu2 = newPokemon("Pikachu2");
    pokemonTrainerAddPokemon(trainer, pikachu2);
    pokemonDestroy(pikachu2);

    // Test new out of bounds
    ret = pokemonTrainerRemovePokemon(trainer, 3);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);

    // Check removal and compression
    pikachu2 = pokemonTrainerGetPokemon(trainer, 2);
    ret = pokemonTrainerRemovePokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 1);
    TEST_EQUALS(result, pokemonTrainerGetPokemon(trainer, 1), pikachu2);

    pokemonTrainerDestroy(trainer);

    return result;
}

static bool testPokemonTrainerGetPokemon() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon ret;

    // Test null trainer
    ret = pokemonTrainerGetPokemon(NULL, 1); 
    TEST_EQUALS(result, ret, NULL);

    // Check boundries
    ret = pokemonTrainerGetPokemon(trainer, 0);
    TEST_EQUALS(result, ret, NULL);
    ret = pokemonTrainerGetPokemon(trainer, 2);
    TEST_EQUALS(result, ret, NULL);
    
    // Fetch
    ret = pokemonTrainerGetPokemon(trainer, 1);
    TEST_DIFFERENT(result, ret, NULL);

    // Fill pokemon list and fetch
    fillPokemonList(trainer);
    ret = pokemonTrainerGetPokemon(trainer, 2);
    TEST_DIFFERENT(result, ret, NULL);
    ret = pokemonTrainerGetPokemon(trainer, TRAINER_MAX_LOCAL);
    TEST_DIFFERENT(result, ret, NULL);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerDepositPokemon() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon charmander;
    PokemonTrainerResult ret;
    int pokemon_count_before, pokemon_count_after;

    // Test null arg
    ret = pokemonTrainerDepositPokemon(NULL, 1); 
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);

    // Test bad index
    ret = pokemonTrainerDepositPokemon(trainer, 0);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);
    ret = pokemonTrainerDepositPokemon(trainer, 2);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);

    // Test deposit last; no changes should have occured
    ret = pokemonTrainerDepositPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_DEPOSIT_LAST);
    TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 1);

    // Add another pokemon and deposit the first
    charmander = newPokemon("Charmander");
    pokemonTrainerAddPokemon(trainer, charmander);
    pokemonDestroy(charmander);
    pokemon_count_before = pokemonTrainerGetNumberOfPokemons(trainer);
    charmander = pokemonTrainerGetPokemon(trainer, 2);
    ret = pokemonTrainerDepositPokemon(trainer, 1);
    pokemon_count_after = pokemonTrainerGetNumberOfPokemons(trainer);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result, pokemonTrainerGetPokemon(trainer, 1), charmander);
    TEST_EQUALS(result, pokemon_count_before, pokemon_count_after);

    // Fill remote list and try to deposit another; nothing should change
    fillWithdrawList(trainer);
    charmander = newPokemon("CharmanderFull");
    pokemonTrainerAddPokemon(trainer, charmander);
    pokemonDestroy(charmander);
    pokemon_count_before = pokemonTrainerGetNumberOfPokemons(trainer);
    charmander = pokemonTrainerGetPokemon(trainer, 2);
    ret = pokemonTrainerDepositPokemon(trainer, 1);
    pokemon_count_after = pokemonTrainerGetNumberOfPokemons(trainer);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_DEPOSIT_FULL);
    TEST_EQUALS(result, pokemonTrainerGetPokemon(trainer, 2), charmander);
    TEST_EQUALS(result, pokemon_count_before, pokemon_count_after);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerWithdrawPokemon() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    PokemonTrainerResult ret;
    Pokemon charmander;
    int pokemon_count_before, pokemon_count_after;
    
    // Test null trainer
    ret = pokemonTrainerWithdrawPokemon(NULL, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);

    // Test bad index
    ret = pokemonTrainerWithdrawPokemon(trainer, 0);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);
    ret = pokemonTrainerWithdrawPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);

    // Test basic usage
    charmander = newPokemon("Charmander");
    pokemonTrainerAddPokemon(trainer, charmander);
    pokemonDestroy(charmander);
    pokemonTrainerDepositPokemon(trainer, 2);
    ret = pokemonTrainerWithdrawPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
    TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 2);

    // Test withdraw and list compress
    pokemonTrainerDepositPokemon(trainer, 2);
    charmander = newPokemon("Charmander2");
    pokemonTrainerAddPokemon(trainer, charmander);
    pokemonDestroy(charmander);
    pokemon_count_before = pokemonTrainerGetNumberOfPokemons(trainer);
    charmander = pokemonTrainerGetPokemon(trainer, 2);
    pokemonTrainerDepositPokemon(trainer, 2);
    ret = pokemonTrainerWithdrawPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
    ret = pokemonTrainerWithdrawPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
    pokemon_count_after = pokemonTrainerGetNumberOfPokemons(trainer);
    TEST_EQUALS(result, pokemonTrainerGetPokemon(trainer, 3), charmander);
    TEST_EQUALS(result, pokemon_count_before, pokemon_count_after);

    // Test withdraw to full party
    fillWithdrawList(trainer);
    fillPokemonList(trainer);

    ret = pokemonTrainerWithdrawPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_PARTY_FULL);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerGetNumberOfPokemons() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon pokemon;
    int pokemon_count;

    // Test value on init
    pokemon_count = pokemonTrainerGetNumberOfPokemons(trainer);
    TEST_EQUALS(result, pokemon_count, 1);

    // Add one to local and remote
    pokemon = newPokemon("Remote");
    pokemonTrainerAddPokemon(trainer, pokemon);
    pokemonDestroy(pokemon);
    pokemonTrainerDepositPokemon(trainer, 2);
    pokemon = newPokemon("Local");
    pokemonTrainerAddPokemon(trainer, pokemon);
    pokemonDestroy(pokemon);
    pokemon_count = pokemonTrainerGetNumberOfPokemons(trainer);
    TEST_EQUALS(result, pokemon_count, 3);

    // Fill up both lists
    fillWithdrawList(trainer);
    fillPokemonList(trainer);
    pokemon_count = pokemonTrainerGetNumberOfPokemons(trainer);
    TEST_EQUALS(result, pokemon_count, TRAINER_MAX_REMOTE + TRAINER_MAX_LOCAL);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerGetMostRankedPokemon() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon ret, pokemon1, pokemon2;

    // Test null trainer
    ret = pokemonTrainerGetMostRankedPokemon(NULL);
    TEST_EQUALS(result, ret, NULL);

    // Test with empty party
    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, pokemonTrainerGetPokemon(trainer, 1));

    fillPokemonList(trainer);

    // Test tie in local list
    pokemon1 = pokemonTrainerGetPokemon(trainer, 1);
    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, pokemon1);

    // Test tie with remote list
    pokemon1 = pokemonTrainerGetPokemon(trainer, 1);
    pokemon2 = pokemonTrainerGetPokemon(trainer, 2);
    addMove(pokemon1, 100);
    addMove(pokemon2, 100);
    pokemonTrainerDepositPokemon(trainer, 2);
    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, pokemon1);
    
    // Test tie in remote list, pokemon2 is now first
    pokemonTrainerDepositPokemon(trainer, 1);
    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, pokemon2);

    // Test most ranked is in remote list.
    pokemon1 = pokemonTrainerGetPokemon(trainer, 1);
    addMove(pokemon1, 50);
    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, pokemon2);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerMakeMostRankedParty() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    PokemonTrainerResult ret;
    Pokemon order[TRAINER_MAX_LOCAL];

    // Test null
    ret = pokemonTrainerMakeMostRankedParty(NULL);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);

    fillPokemonList(trainer);
    for (int i = 0; i < TRAINER_MAX_LOCAL; i++) {
        order[i] = pokemonTrainerGetPokemon(trainer, i+1);
    }

    // Test no-op
    ret = pokemonTrainerMakeMostRankedParty(trainer);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
    for (int i = 0; i < TRAINER_MAX_LOCAL; i++) {
        TEST_EQUALS(result, pokemonTrainerGetPokemon(trainer, i+1), order[i]);
    }

    // Test specific order, across both lists:
    // First set the desired order
    addMove(order[0], 300);
    addMove(order[1], 300);
    addMove(order[2], 200);
    addMove(order[3], 100);
    addMove(order[4], 100);

    // Then shuffle the pokemon around
    pokemonTrainerDepositPokemon(trainer, 1);
    pokemonTrainerDepositPokemon(trainer, 1);
    pokemonTrainerDepositPokemon(trainer, 1);
    pokemonTrainerDepositPokemon(trainer, 2);
    pokemonTrainerWithdrawPokemon(trainer, 1);

    // Now test
    ret = pokemonTrainerMakeMostRankedParty(trainer);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
    for (int i = 0; i < TRAINER_MAX_LOCAL; i++) {
        TEST_EQUALS(result, pokemonTrainerGetPokemon(trainer, i+1), order[i]);
    }
    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerPrintEnumeration() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    PokemonTrainerResult ret;
    FILE* file = (FILE*)1; // Dummy value

    // Test NULL
    ret = pokemonTrainerPrintEnumeration(NULL, file);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);
    ret = pokemonTrainerPrintEnumeration(trainer, NULL);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);
    
    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testCombo() {
        bool result = true;
        Pokemon pikachu = newPokemon("Pikachu");
        PokemonTrainer trainer = pokemonTrainerCreate("Ash", pikachu, 6, 20);
        TEST_DIFFERENT(result, trainer, NULL);
        pokemonDestroy(pikachu);
        PokemonTrainer trainer_copy = pokemonTrainerCopy(trainer);
        TEST_DIFFERENT(result, trainer_copy, NULL);
        pokemonTrainerDestroy(trainer_copy);
        Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 10, 4);
        TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, squirtle),
                POKEMON_TRAINER_SUCCESS);
        pokemonDestroy(squirtle);
        TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 2);
        TEST_DIFFERENT(result, pokemonTrainerGetPokemon(trainer, 2), NULL);
        TEST_EQUALS(result, pokemonTrainerDepositPokemon(trainer, 2),
                POKEMON_TRAINER_SUCCESS);
        TEST_EQUALS(result, pokemonTrainerWithdrawPokemon(trainer, 1),
                POKEMON_TRAINER_SUCCESS);
        TEST_EQUALS(result, pokemonTrainerRemovePokemon(trainer, 2),
                POKEMON_TRAINER_SUCCESS);
        TEST_EQUALS(result, pokemonTrainerGetMostRankedPokemon(trainer),
                pokemonTrainerGetPokemon(trainer, 1));
        TEST_EQUALS(result, pokemonTrainerMakeMostRankedParty(trainer),
                POKEMON_TRAINER_SUCCESS);
        pokemonTrainerDestroy(trainer);
        return result;
}

int main() {
	RUN_TEST(testCombo);
    RUN_TEST(testPokemonTrainerCreate);
    RUN_TEST(testPokemonTrainerCopy);
    RUN_TEST(testPokemonTrainerAddPokemon);
    RUN_TEST(testPokemonTrainerRemovePokemon);
    RUN_TEST(testPokemonTrainerGetPokemon);
    RUN_TEST(testPokemonTrainerDepositPokemon);
    RUN_TEST(testPokemonTrainerWithdrawPokemon);
    RUN_TEST(testPokemonTrainerGetNumberOfPokemons);
    RUN_TEST(testPokemonTrainerGetMostRankedPokemon);
    RUN_TEST(testPokemonTrainerMakeMostRankedParty);
    RUN_TEST(testPokemonTrainerPrintEnumeration);
	return 0;
}
