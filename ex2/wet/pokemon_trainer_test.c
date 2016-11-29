#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "aux_macros.h"
#include "pokemon_trainer.h"

static Pokemon newPokemon(char* name) {
    return pokemonCreate(name, TYPE_ELECTRIC, 10, 4);
}

static void fillWithdrawList(PokemonTrainer trainer) {
     char *name = malloc(15);
     for (int i = 1; i <= trainer->remote_pokemon->max_length; i++) {
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
     for (int i = 2; i <= trainer->local_pokemon->max_length; i++) {
         snprintf(name, 15, "Local%d", i);
         Pokemon pokemon = newPokemon(name);
         pokemonTrainerAddPokemon(trainer, pokemon);
         pokemonDestroy(pokemon);
     }
     free(name);
}

static PokemonTrainer newTrainer() {
    Pokemon pikachu = newPokemon("Pikachu");
    PokemonTrainer trainer = pokemonTrainerCreate("Ash", pikachu, 10, 10);

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

    // Verify internal fields
    trainer = pokemonTrainerCreate("Ash", pikachu, 10, 10);
    TEST_EQUALS(result, strcmp(trainer->name, "Ash"), 0);
    TEST_EQUALS(result, trainer->local_pokemon->length, 1);
    TEST_EQUALS(result, trainer->remote_pokemon->length, 0);
    TEST_EQUALS(result, trainer->local_pokemon->max_length, 10);
    TEST_EQUALS(result, trainer->remote_pokemon->max_length, 10);
    TEST_EQUALS(result, strcmp(trainer->local_pokemon->list[0]->name, pikachu->name), 0);

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

    // Check run copied everything
    trainer2 = pokemonTrainerCopy(trainer1);
    TEST_DIFFERENT(result, trainer2, NULL);
    TEST_DIFFERENT(result, trainer1->name, trainer2->name);
    TEST_DIFFERENT(result, trainer1->local_pokemon, trainer2->local_pokemon);
    TEST_DIFFERENT(result, trainer1->remote_pokemon, trainer2->remote_pokemon);
    TEST_DIFFERENT(result, trainer1->local_pokemon->list, trainer2->local_pokemon->list);
    TEST_DIFFERENT(result, trainer1->remote_pokemon->list, trainer2->remote_pokemon->list);
    TEST_DIFFERENT(result, trainer1->local_pokemon->list[0], trainer2->local_pokemon->list[0]);

    // Check values are the same
    TEST_EQUALS(result, strcmp(trainer1->name, trainer2->name), 0);
    TEST_EQUALS(result, trainer1->local_pokemon->length, trainer2->local_pokemon->length);
    TEST_EQUALS(result, trainer1->remote_pokemon->length, trainer2->remote_pokemon->length);
    TEST_EQUALS(result, trainer1->local_pokemon->max_length, trainer2->local_pokemon->max_length);
    TEST_EQUALS(result, trainer1->remote_pokemon->max_length, trainer2->remote_pokemon->max_length);
    TEST_EQUALS(result, strcmp(trainer1->local_pokemon->list[0]->name, trainer2->local_pokemon->list[0]->name), 0);

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
        pikachu = pokemonCreate(name, TYPE_ELECTRIC, 20, 4);
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
    Pokemon pikachu2;
    PokemonTrainerResult ret;

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

    pikachu2 = pokemonCreate("Pikachu2", TYPE_ELECTRIC, 20, 4);
    pokemonTrainerAddPokemon(trainer, pikachu2);
    pokemonDestroy(pikachu2);

    // Check new out of bounds
    ret = pokemonTrainerRemovePokemon(trainer, 3);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);

    // Check removal and compression
    ret = pokemonTrainerRemovePokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
    pikachu2 = pokemonTrainerGetPokemon(trainer, 1);
    TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 1);
    TEST_EQUALS(result, strcmp(pikachu2->name, "Pikachu2"), 0);

    pokemonTrainerDestroy(trainer);

    return result;
}

static bool testPokemonTrainerGetPokemon() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon charmander = pokemonCreate("Charmander", TYPE_FIRE, 20, 4);
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
    TEST_EQUALS(result, ret, trainer->local_pokemon->list[0]);

    // Add another and fetch
    pokemonTrainerAddPokemon(trainer, charmander);
    pokemonDestroy(charmander);
    ret = pokemonTrainerGetPokemon(trainer, 2);
    TEST_EQUALS(result, ret, trainer->local_pokemon->list[1]);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerDepositPokemon() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon charmander = newPokemon("Charmander");
    Pokemon temp;
    PokemonTrainerResult ret;

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
    TEST_EQUALS(result, trainer->local_pokemon->length, 1);
    TEST_EQUALS(result, trainer->remote_pokemon->length, 0);

    // Add another pokemon and deposit the first
    pokemonTrainerAddPokemon(trainer, charmander);
    ret = pokemonTrainerDepositPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
    temp = pokemonTrainerGetPokemon(trainer, 1);
    TEST_EQUALS(result, strcmp(temp->name, charmander->name), 0);
    temp = trainer->remote_pokemon->list[0];
    TEST_EQUALS(result, strcmp(temp->name, "Pikachu"), 0);
    pokemonDestroy(charmander);

    fillWithdrawList(trainer);
    charmander = newPokemon("RemoteFull");
    pokemonTrainerAddPokemon(trainer, charmander);
    ret = pokemonTrainerDepositPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_DEPOSIT_FULL);
    pokemonDestroy(charmander);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerWithdrawPokemonNull() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    PokemonTrainerResult ret;
    
    // Test null trainer
    ret = pokemonTrainerWithdrawPokemon(NULL, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);

    // Test bad index
    ret = pokemonTrainerWithdrawPokemon(trainer, 0);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);
    ret = pokemonTrainerWithdrawPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_INVALID_INDEX);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerWithdrawPokemonSingle() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    PokemonTrainerResult ret;

    fillWithdrawList(trainer);

     // Test withdraw and list compress
     ret = pokemonTrainerWithdrawPokemon(trainer, 1);
     TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
     TEST_EQUALS(result, trainer->local_pokemon->length, 2);
     TEST_EQUALS(result, trainer->remote_pokemon->length, trainer->remote_pokemon->max_length - 1);
     char* name = malloc(15);
     for (int i = 1; i <= trainer->remote_pokemon->length; i++) {
         snprintf(name, 15, "Remote%d", i+1);
         TEST_EQUALS(result, strcmp(trainer->remote_pokemon->list[i-1]->name, name), 0);
     }
    free(name);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerWithdrawPokemonFull() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    PokemonTrainerResult ret;

    fillWithdrawList(trainer);
    fillPokemonList(trainer);

    ret = pokemonTrainerWithdrawPokemon(trainer, 1);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_PARTY_FULL);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerGetMostRankedEmpty() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon ret;

    // Test null trainer
    ret = pokemonTrainerGetMostRankedPokemon(NULL);
    TEST_EQUALS(result, ret, NULL);

    // Test with empty party
    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, pokemonTrainerGetPokemon(trainer, 1));

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerGetMostRankedTie() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon ret;

    fillWithdrawList(trainer);
    fillPokemonList(trainer);

    pokemonTeachMove(trainer->local_pokemon->list[0], "Move", TYPE_ELECTRIC, 10, 100);
    pokemonTeachMove(trainer->remote_pokemon->list[0], "Move", TYPE_ELECTRIC, 10, 100);
    
    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, pokemonTrainerGetPokemon(trainer, 1));

    pokemonTeachMove(trainer->local_pokemon->list[2], "Move", TYPE_ELECTRIC, 10, 200);
    pokemonTeachMove(trainer->local_pokemon->list[3], "Move", TYPE_ELECTRIC, 10, 200);

    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, pokemonTrainerGetPokemon(trainer, 3));

    pokemonTeachMove(trainer->remote_pokemon->list[2], "Move", TYPE_ELECTRIC, 10, 300);
    pokemonTeachMove(trainer->remote_pokemon->list[3], "Move", TYPE_ELECTRIC, 10, 300);

    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, trainer->remote_pokemon->list[2]);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testPokemonTrainerGetMostRankedProf() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    Pokemon ret;

    fillWithdrawList(trainer);
    fillPokemonList(trainer);

    pokemonTeachMove(trainer->local_pokemon->list[0], "Move", TYPE_ELECTRIC, 10, 100);
    pokemonTeachMove(trainer->remote_pokemon->list[0], "Move", TYPE_ELECTRIC, 10, 200);
    
    ret = pokemonTrainerGetMostRankedPokemon(trainer);
    TEST_EQUALS(result, ret, trainer->remote_pokemon->list[0]);

    pokemonTrainerDestroy(trainer);
    return result;
}

//static bool testPokemonTrainerMakeMostRankedPartyBase() {
//    bool result = true;
//    PokemonTrainer trainer = newTrainer();
//    PokemonTrainerResult ret;
//    PokemonList local_ref, remote_ref;
//
//    // Test null
//    ret = pokemonTrainerMakeMostRankedParty(NULL);
//    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);
//
//    fillWithdrawList(trainer);
//    fillPokemonList(trainer);
//
//    local_ref = pokemonListCreate(trainer->local_pokemon->length, 0);
//    remote_ref = pokemonListCreate(trainer->remote_pokemon->length, 0);
//
//    // Test no-op
//    pokemonListMove(local_ref, trainer->local_pokemon, 0, 0);
//    pokemonListMove(remote_ref, trainer->remote_pokemon, 0, 0);
//    ret = pokemonTrainerMakeMostRankedParty(trainer);
//    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
//    for (int i = 0; i < trainer->local_pokemon->length; i++) {
//        TEST_EQUALS(result, local_ref->list[i], trainer->local_pokemon->list[i]);
//    }
//    for (int i = 0; i < trainer->remote_pokemon->length; i++) {
//        TEST_EQUALS(result, remote_ref->list[i], trainer->remote_pokemon->list[i]);
//    }
//
//    pokemonListShallowDestroy(local_ref);
//    pokemonListShallowDestroy(remote_ref);
//    pokemonTrainerDestroy(trainer);
//    return result;
//}

//static bool testPokemonTrainerMakeMostRankedPartyOrder() {
//    bool result = true;
//    PokemonTrainer trainer = newTrainer();
//    PokemonTrainerResult ret;
//    PokemonList local_ref, remote_ref;
//
//    fillWithdrawList(trainer);
//    fillPokemonList(trainer);
//
//    local_ref = pokemonListCreate(trainer->local_pokemon->length, 0);
//    remote_ref = pokemonListCreate(trainer->remote_pokemon->length, 0);
//
//    pokemonTeachMove(trainer->local_pokemon->list[0], "Move", TYPE_ELECTRIC, 10, 100);
//    pokemonTeachMove(trainer->local_pokemon->list[1], "Move", TYPE_ELECTRIC, 10, 100);
//    pokemonTeachMove(trainer->local_pokemon->list[2], "Move", TYPE_ELECTRIC, 10, 150);
//    pokemonTeachMove(trainer->remote_pokemon->list[1], "Move", TYPE_ELECTRIC, 10, 200);
//    pokemonTeachMove(trainer->remote_pokemon->list[2], "Move", TYPE_ELECTRIC, 10, 100);
//    pokemonTeachMove(trainer->remote_pokemon->list[3], "Move", TYPE_ELECTRIC, 10, 150);
//
//    local_ref->list[0] = trainer->remote_pokemon->list[1];
//    local_ref->list[1] = trainer->local_pokemon->list[2];
//    local_ref->list[2] = trainer->remote_pokemon->list[3];
//    local_ref->list[3] = trainer->local_pokemon->list[0];
//    local_ref->list[4] = trainer->local_pokemon->list[1];
//    local_ref->list[5] = trainer->remote_pokemon->list[2];
//    local_ref->list[6] = trainer->local_pokemon->list[3];
//    local_ref->list[7] = trainer->local_pokemon->list[4];
//    local_ref->list[8] = trainer->local_pokemon->list[5];
//    local_ref->list[9] = trainer->local_pokemon->list[6];
//    remote_ref->list[0] = trainer->local_pokemon->list[7];
//    remote_ref->list[1] = trainer->local_pokemon->list[8];
//    remote_ref->list[2] = trainer->local_pokemon->list[9];
//    remote_ref->list[3] = trainer->remote_pokemon->list[0];
//    remote_ref->list[4] = trainer->remote_pokemon->list[4];
//    remote_ref->list[5] = trainer->remote_pokemon->list[5];
//    remote_ref->list[6] = trainer->remote_pokemon->list[6];
//    remote_ref->list[7] = trainer->remote_pokemon->list[7];
//    remote_ref->list[8] = trainer->remote_pokemon->list[8];
//    remote_ref->list[9] = trainer->remote_pokemon->list[9];
//    ret = pokemonTrainerMakeMostRankedParty(trainer);
//    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS);
//    for (int i = 0; i < trainer->local_pokemon->length; i++) {
//        TEST_EQUALS(result, local_ref->list[i], trainer->local_pokemon->list[i]);
//    }
//    for (int i = 0; i < trainer->remote_pokemon->length; i++) {
//        TEST_EQUALS(result, remote_ref->list[i], trainer->remote_pokemon->list[i]);
//    }
//
//    pokemonListShallowDestroy(local_ref);
//    pokemonListShallowDestroy(remote_ref);
//    pokemonTrainerDestroy(trainer);
//    return result;
//}

static bool testPokemonTrainerPrintEnumeration() {
    bool result = true;
    PokemonTrainer trainer = newTrainer();
    PokemonTrainerResult ret;
    FILE* file = fopen("/tmp/enumeration", "w");

    // Test NULL
    ret = pokemonTrainerPrintEnumeration(NULL, file);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);
    ret = pokemonTrainerPrintEnumeration(trainer, NULL);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_NULL_ARG);
    
    // Test success
    fillPokemonList(trainer);
    ret = pokemonTrainerPrintEnumeration(trainer, file);
    TEST_EQUALS(result, ret, POKEMON_TRAINER_SUCCESS); 
    fclose(file);

    pokemonTrainerDestroy(trainer);
    return result;
}

static bool testCombo() {
        bool result = true;
        Pokemon pikachu = newPokemon("Pikachu");
        PokemonTrainer trainer = pokemonTrainerCreate("Ash", pikachu, 6, 20);
        TEST_DIFFERENT(result, trainer, NULL);
        printf("pokemonTrainerCreate: %d\n", result);
        pokemonDestroy(pikachu);
        PokemonTrainer trainer_copy = pokemonTrainerCopy(trainer);
        TEST_DIFFERENT(result, trainer_copy, NULL);
        printf("pokemonTrainerCopy: %d\n", result);
        pokemonTrainerDestroy(trainer_copy);
        Pokemon squirtle = pokemonCreate("Squirtle", TYPE_WATER, 10, 4);
        TEST_EQUALS(result, pokemonTrainerAddPokemon(trainer, squirtle),
                POKEMON_TRAINER_SUCCESS);
        printf("pokemonTrainerAddPokemon: %d\n", result);
        pokemonDestroy(squirtle);
        TEST_EQUALS(result, pokemonTrainerGetNumberOfPokemons(trainer), 2);
        printf("pokemonTrainerGetNumber: %d\n", result);
        TEST_DIFFERENT(result, pokemonTrainerGetPokemon(trainer, 2), NULL);
        printf("pokemonTrainerGetPokemon: %d\n", result);
        TEST_EQUALS(result, pokemonTrainerDepositPokemon(trainer, 2),
                POKEMON_TRAINER_SUCCESS);
        printf("pokemonTrainerDeposit: %d\n", result);
        TEST_EQUALS(result, pokemonTrainerWithdrawPokemon(trainer, 1),
                POKEMON_TRAINER_SUCCESS);
        printf("pokemonTrainerWithdraw: %d\n", result);
        TEST_EQUALS(result, pokemonTrainerRemovePokemon(trainer, 2),
                POKEMON_TRAINER_SUCCESS);
        printf("pokemonTrainerRemove: %d\n", result);
        TEST_EQUALS(result, pokemonTrainerGetMostRankedPokemon(trainer),
                pokemonTrainerGetPokemon(trainer, 1));
        printf("pokemonTrainerGetMostRanked: %d\n", result);
        TEST_EQUALS(result, pokemonTrainerMakeMostRankedParty(trainer),
                POKEMON_TRAINER_SUCCESS);
        printf("pokemonTrainerMakeMostRankedParty: %d\n", result);
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
    RUN_TEST(testPokemonTrainerWithdrawPokemonNull);
    RUN_TEST(testPokemonTrainerWithdrawPokemonSingle);
    RUN_TEST(testPokemonTrainerWithdrawPokemonFull);
    RUN_TEST(testPokemonTrainerGetMostRankedEmpty);
    RUN_TEST(testPokemonTrainerGetMostRankedTie);
    RUN_TEST(testPokemonTrainerGetMostRankedProf);
//    RUN_TEST(testPokemonTrainerMakeMostRankedPartyBase);
//    RUN_TEST(testPokemonTrainerMakeMostRankedPartyOrder);
    RUN_TEST(testPokemonTrainerPrintEnumeration);
	return 0;
}
