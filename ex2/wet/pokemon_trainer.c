#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "pokemon_trainer.h"

PokemonList pokemonListCreate(int max_length, int min_length) {
    PokemonList base;

    if (max_length < min_length || min_length < 0 || max_length <= 0)
        return NULL;

    base = malloc(sizeof(struct pokemon_list_t));
    if (NULL == base) return NULL;
    base->list = malloc(max_length * sizeof(Pokemon*));
    if (NULL == base->list) return NULL;
    base->max_length = max_length;
    base->min_length = min_length;
    base->length = 0;

    return base;
}

void pokemonListShallowDestroy(PokemonList base) {
    if (NULL == base) return;

    free(base->list);
    free(base);
}

void pokemonListDestroy(PokemonList base) {
    if (NULL == base) return;

    // Free all of the trainer's pokemon
    for (int i=0; i < base->length; i++) {
        pokemonDestroy(base->list[i]);
    }
    pokemonListShallowDestroy(base);
}

Pokemon pokemonListGet(PokemonList base, int index) {
    if (NULL == base) return NULL;
    if (index < 1 || index > base->length) return NULL;

    return base->list[index - 1];
}

void pokemonListMove(PokemonList dest, PokemonList source, int dest_offset, int source_offset) {
    // Check for valid args and no-op cases
    if (dest_offset < 0 || dest_offset > dest->length) return;
    if (source_offset < 0 || source_offset > source->length) return;

    // Calculate how much to move and space required
    int amount_to_move, target_length;
    amount_to_move = source->length - source_offset;
    target_length = dest_offset + amount_to_move;

    // Move less items if we'd go past dest's max length
    if (target_length > dest->max_length) {
        target_length = dest->max_length;
        amount_to_move = target_length - dest_offset;
    }

    // Move items and update dest's length
    for (int i=0; i < amount_to_move; i++) {
        dest->list[dest_offset + i] = source->list[source_offset + i];
    }
    dest->length = target_length;
    return;
}

void pokemonListSort(PokemonList base) {
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i=0; i < base->length - 1; i++) {
            int rank1, rank2;
            rank1 = pokemonGetRank(base->list[i]);
            rank2 = pokemonGetRank(base->list[i+1]);
            if (rank1 < rank2) {
                Pokemon temp;
                temp = base->list[i];
                base->list[i] = base->list[i+1];
                base->list[i+1] = temp;
                sorted = false;
            }
        }
    }
}

PokemonTrainerResult pokemonListAppend(PokemonList base, Pokemon pokemon) {
    Pokemon pokemon_copy;

    // Boundry checks
    if (base->length == base->max_length)
        return POKEMON_TRAINER_PARTY_FULL;

    // Copy pokemon
    pokemon_copy = pokemonCopy(pokemon);
    if (NULL == pokemon_copy) return POKEMON_TRAINER_OUT_OF_MEM;

    // Add pokemon to list
    base->length++;
    base->list[base->length - 1] = pokemon_copy;

    return POKEMON_TRAINER_SUCCESS;
}

PokemonTrainerResult pokemonListRemove(PokemonList base, int index) {
    if (index < 1 || index > base->length)
        return POKEMON_TRAINER_INVALID_INDEX;
    if (base->length <= base->min_length)
        return POKEMON_TRAINER_REMOVE_LAST;

    pokemonDestroy(base->list[index - 1]);
    for (int i=index; i < base->length; i++) {
        base->list[i - 1] = base->list[i];
    }
    base->length--;

    return POKEMON_TRAINER_SUCCESS;
}

PokemonTrainer pokemonTrainerCreate(char* name, Pokemon initial_pokemon, int max_num_local, int max_num_remote) {
    // Check arguments are valid
    if ((NULL == name) || (strcmp(name, "") == 0) || (NULL == initial_pokemon) || (max_num_local <= 0) || (max_num_remote <= 0)) return NULL;

    // Allocate memory
    PokemonTrainer trainer;
    trainer = malloc(sizeof(struct pokemon_trainer_t));
    if (NULL == trainer) return NULL;
    trainer->name = malloc(strlen(name) + 1);
    trainer->local_pokemon = pokemonListCreate(max_num_local, POKEMON_TRAINER_MIN_LENGTH_LOCAL);
    trainer->remote_pokemon = pokemonListCreate(max_num_remote, POKEMON_TRAINER_MIN_LENGTH_REMOTE);
    if ((NULL == trainer->name) || (NULL == trainer->local_pokemon) || (NULL == trainer->remote_pokemon)) return NULL;

    // Fill trainer object
    PokemonTrainerResult result;
    strcpy(trainer->name, name);
    result = pokemonListAppend(trainer->local_pokemon, initial_pokemon);
    if (result != POKEMON_TRAINER_SUCCESS) return NULL;

    return trainer;
}

void pokemonTrainerDestroy(PokemonTrainer trainer) {
    if (NULL == trainer) return;

    free(trainer->name);
    pokemonListDestroy(trainer->local_pokemon);
    pokemonListDestroy(trainer->remote_pokemon);
    free(trainer);
}

PokemonTrainer pokemonTrainerCopy(PokemonTrainer trainer) {
    // Check given trainer is valid
    if (NULL == trainer) return NULL;

    // Create a trainer using the given trainer's first pokemon
    PokemonTrainer new_trainer = pokemonTrainerCreate(trainer->name, pokemonListGet(trainer->local_pokemon, 1), trainer->local_pokemon->max_length, trainer->remote_pokemon->max_length);
    if (NULL == new_trainer) return NULL;

    // Add the rest of the local pokemon
    for (int i=2; i <= trainer->local_pokemon->length; i++) {
        PokemonTrainerResult append_result;
        append_result = pokemonListAppend(new_trainer->local_pokemon, pokemonListGet(trainer->local_pokemon, i));
        if (append_result == POKEMON_TRAINER_OUT_OF_MEM) return NULL;
        assert(append_result == POKEMON_TRAINER_SUCCESS);
    }
    // Add the remote pokemon
    // TODO: code dedup?
    for (int i=1; i <= trainer->remote_pokemon->length; i++) {
        PokemonTrainerResult append_result;
        append_result = pokemonListAppend(new_trainer->remote_pokemon, pokemonListGet(trainer->remote_pokemon, i));
        if (append_result == POKEMON_TRAINER_OUT_OF_MEM) return NULL;
        assert(append_result == POKEMON_TRAINER_SUCCESS);
    }

    return new_trainer;
}

PokemonTrainerResult pokemonTrainerAddPokemon(PokemonTrainer trainer, Pokemon pokemon) {
    // Check args
    if (NULL == trainer || NULL == pokemon) return POKEMON_TRAINER_NULL_ARG;

    return pokemonListAppend(trainer->local_pokemon, pokemon);
}

Pokemon pokemonTrainerGetPokemon(PokemonTrainer trainer, int pokemon_index) {
    // Check args
    if (NULL == trainer) return NULL;

    // Return pokemon
    return pokemonListGet(trainer->local_pokemon, pokemon_index);
}

PokemonTrainerResult pokemonTrainerRemovePokemon(PokemonTrainer trainer, int pokemon_index) {
    // Check args
    if (NULL == trainer) return POKEMON_TRAINER_NULL_ARG;

    // TODO: Check remote_last is the right error instead of deposit_last
    return pokemonListRemove(trainer->local_pokemon, pokemon_index);
}

PokemonTrainerResult pokemonTrainerDepositPokemon(PokemonTrainer trainer, int pokemon_index) {
    if (NULL == trainer) return POKEMON_TRAINER_NULL_ARG;

    Pokemon pokemon_to_deposit = pokemonListGet(trainer->local_pokemon, pokemon_index);
    PokemonTrainerResult result;
    
    if (NULL == pokemon_to_deposit) return POKEMON_TRAINER_INVALID_INDEX;
    
    result = pokemonListAppend(trainer->remote_pokemon, pokemon_to_deposit);
    // Translate error codes to the deposit use case. ugh.
    if (result != POKEMON_TRAINER_SUCCESS) {
        if (result == POKEMON_TRAINER_PARTY_FULL)
            return POKEMON_TRAINER_DEPOSIT_FULL;
        return result;
    }

    result = pokemonListRemove(trainer->local_pokemon, pokemon_index);
    if (result != POKEMON_TRAINER_SUCCESS) {
        // Undo the append above in case of error when removing from local_pokemon list
        pokemonListRemove(trainer->remote_pokemon, trainer->remote_pokemon->length);;
        // Translate error codes for the deposit use case. ugh again.
        if (result == POKEMON_TRAINER_REMOVE_LAST)
            return POKEMON_TRAINER_DEPOSIT_LAST;
    }

    return result;
}

PokemonTrainerResult pokemonTrainerWithdrawPokemon(PokemonTrainer trainer, int pokemon_index) {
    if (NULL == trainer) return POKEMON_TRAINER_NULL_ARG;

    Pokemon pokemon_to_withdraw;
    PokemonTrainerResult result;

    pokemon_to_withdraw = pokemonListGet(trainer->remote_pokemon, pokemon_index);
    if (NULL == pokemon_to_withdraw) return POKEMON_TRAINER_INVALID_INDEX;
    
    result = pokemonListAppend(trainer->local_pokemon, pokemon_to_withdraw);
    if (result != POKEMON_TRAINER_SUCCESS) return result;

    result = pokemonListRemove(trainer->remote_pokemon, pokemon_index);
    if (result != POKEMON_TRAINER_SUCCESS) {
        // Undo the append above if there was an error removing from the remote_pokemon list
        PokemonTrainerResult undo_result;
        undo_result = pokemonListRemove(trainer->local_pokemon, trainer->local_pokemon->length);
        assert(undo_result == POKEMON_TRAINER_SUCCESS);
    }
    return result;
}

int pokemonTrainerGetNumberOfPokemons(PokemonTrainer trainer) {
    assert(NULL != trainer);
    return trainer->local_pokemon->length + trainer->remote_pokemon->length;
}

Pokemon pokemonTrainerGetMostRankedPokemon(PokemonTrainer trainer) {
    if (NULL == trainer) return NULL;
    int max_rank = -1;
    Pokemon max_rank_pokemon;

    for (int i=1, rank; i <= trainer->local_pokemon->length; i++) {
        Pokemon current_pokemon = pokemonListGet(trainer->local_pokemon, i);
        rank = pokemonGetRank(current_pokemon);
        if (rank > max_rank) {
            max_rank = rank;
            max_rank_pokemon = current_pokemon;
        }
    }
    for (int i=1, rank; i <= trainer->remote_pokemon->length; i++) {
        Pokemon current_pokemon = pokemonListGet(trainer->remote_pokemon, i);
        rank = pokemonGetRank(current_pokemon);
        if (rank > max_rank) {
            max_rank = rank;
            max_rank_pokemon = current_pokemon;
        }
    }

    return max_rank_pokemon;
}

PokemonTrainerResult pokemonTrainerMakeMostRankedParty(PokemonTrainer trainer) {
    if (NULL == trainer) return POKEMON_TRAINER_NULL_ARG;

    // Create a combined list of local and remote pokemon, we'll sort that.
    int combined_party_length = trainer->local_pokemon->length + trainer->remote_pokemon->length;
    PokemonList combined_party = pokemonListCreate(combined_party_length, 0);
    if (NULL == combined_party) return POKEMON_TRAINER_OUT_OF_MEM;
    pokemonListMove(combined_party, trainer->local_pokemon, 0, 0);
    pokemonListMove(combined_party, trainer->remote_pokemon, trainer->local_pokemon->length, 0);

    // Sort
    pokemonListSort(combined_party);

    // Fill up the local_pokemon list up to the max
    pokemonListMove(trainer->local_pokemon, combined_party, 0, 0);

    // Put the remaining pokemon in the remote_pokemon list
    pokemonListMove(trainer->remote_pokemon, combined_party, 0, trainer->local_pokemon->length);;

    // Free the combined_party list. Use ShallowDestroy because we didn't copy the actual pokemon while moving them.
    pokemonListShallowDestroy(combined_party);

    return POKEMON_TRAINER_SUCCESS;
}

PokemonTrainerResult pokemonTrainerPrintEnumeration(PokemonTrainer trainer, FILE* file) {
    if (NULL == trainer || NULL == file) return POKEMON_TRAINER_NULL_ARG;

    for (int i = 1; i <= trainer->local_pokemon->length; i++) {
        Pokemon pokemon = pokemonListGet(trainer->local_pokemon, i);
        PokemonResult result;
        fprintf(file, "%s: ", trainer->name);
        result = pokemonPrintName(pokemon, file);
        assert(result == POKEMON_SUCCESS);
        fprintf(file, "\n");
        result = pokemonPrintName(pokemon, file);
        assert(result == POKEMON_SUCCESS);
        fprintf(file, ": ");
        result = pokemonPrintVoice(pokemon, file);
        assert(result == POKEMON_SUCCESS);
        fprintf(file, "\n");
    }

    return POKEMON_TRAINER_SUCCESS;
}
