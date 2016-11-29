#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "pokemon_trainer.h"

#define POKEMON_TRAINER_MIN_LENGTH_LOCAL 1
#define POKEMON_TRAINER_MIN_LENGTH_REMOTE 0

PokemonList pokemonListCreate(int max_length, int min_length) {
    PokemonList base;

    if (max_length < min_length || min_length < 0 || max_length <= 0)
        return NULL;

    base = malloc(sizeof(struct pokemon_list_t));
    if (NULL == base) return NULL;
    base->list = malloc(max_length * sizeof(Pokemon*));
    if (NULL == base->list) {
        free(base);
        return NULL;
    }
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

PokemonTrainerResult pokemonListCopy(PokemonList dest, PokemonList source,
        int source_offset) {
    int amount_to_copy;

    amount_to_copy = source->length - source_offset;
    for (int i=1; i <= amount_to_copy; i++) {
        PokemonTrainerResult append_result;
        Pokemon current_pokemon, current_copy;
        current_pokemon = pokemonListGet(source, i + source_offset);
        current_copy = pokemonCopy(current_pokemon);
        if (NULL == current_copy) {
            return POKEMON_TRAINER_OUT_OF_MEM;
        }
        append_result = pokemonListAppend(dest, current_copy);
        if (append_result != POKEMON_TRAINER_SUCCESS) {
            pokemonDestroy(current_copy);
            return append_result;
        }
    }

    return POKEMON_TRAINER_SUCCESS;
}

void pokemonListMove(PokemonList dest, PokemonList source, int dest_offset,
        int source_offset) {
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
    // Boundry checks
    if (base->length == base->max_length)
        return POKEMON_TRAINER_PARTY_FULL;

    // Add pokemon to list
    base->length++;
    base->list[base->length - 1] = pokemon;

    return POKEMON_TRAINER_SUCCESS;
}

PokemonTrainerResult pokemonListRemove(PokemonList base, int index, bool keep) {
    if (index < 1 || index > base->length)
        return POKEMON_TRAINER_INVALID_INDEX;
    if (base->length <= base->min_length)
        return POKEMON_TRAINER_REMOVE_LAST;

    if (!keep) {
        pokemonDestroy(base->list[index -1]);
    }
    for (int i=index; i < base->length; i++) {
        base->list[i - 1] = base->list[i];
    }
    base->length--;

    return POKEMON_TRAINER_SUCCESS;
}

PokemonTrainer pokemonTrainerCreate(char* name, Pokemon initial_pokemon,
        int max_num_local, int max_num_remote) {
    // Check arguments are valid
    if ((NULL == name) || (strlen(name) == 0) || (NULL == initial_pokemon)
            || (max_num_local <= 0) || (max_num_remote <= 0)) return NULL;
    // Allocate memory
    PokemonTrainer trainer;
    trainer = malloc(sizeof(struct pokemon_trainer_t));
    if (NULL == trainer) return NULL;
    trainer->name = malloc(strlen(name) + 1);
    trainer->local_pokemon = pokemonListCreate(max_num_local,
            POKEMON_TRAINER_MIN_LENGTH_LOCAL);
    trainer->remote_pokemon = pokemonListCreate(max_num_remote,
            POKEMON_TRAINER_MIN_LENGTH_REMOTE);
    if ((NULL == trainer->name) || (NULL == trainer->local_pokemon) ||
            (NULL == trainer->remote_pokemon)) {
        free(trainer->name);
        free(trainer);
        return NULL;
    }
    // Fill trainer object
    Pokemon initial_pokemon_copy;
    PokemonTrainerResult result;
    strcpy(trainer->name, name);
    initial_pokemon_copy = pokemonCopy(initial_pokemon);
    result = pokemonListAppend(trainer->local_pokemon, initial_pokemon_copy);
    if (result != POKEMON_TRAINER_SUCCESS) {
        pokemonDestroy(initial_pokemon_copy);
        pokemonTrainerDestroy(trainer);
        return NULL;
    } 
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
    PokemonTrainer new_trainer;
    Pokemon first_pokemon;
    int max_num_local, max_num_remote;
    first_pokemon = pokemonListGet(trainer->local_pokemon, 1);
    max_num_local = trainer->local_pokemon->max_length;
    max_num_remote = trainer->remote_pokemon->max_length;
    new_trainer  = pokemonTrainerCreate(trainer->name, first_pokemon,
            max_num_local, max_num_remote);
    if (NULL == new_trainer) return NULL;

    // Add the rest of the pokemon
    PokemonTrainerResult local_result, remote_result;
    local_result = pokemonListCopy(new_trainer->local_pokemon,
            trainer->local_pokemon, 1);
    remote_result = pokemonListCopy(new_trainer->remote_pokemon,
            trainer->remote_pokemon, 0);
    if (local_result != POKEMON_TRAINER_SUCCESS ||
            remote_result != POKEMON_TRAINER_SUCCESS) {
        pokemonTrainerDestroy(new_trainer);
        return NULL;
    }

    return new_trainer;
}

PokemonTrainerResult pokemonTrainerAddPokemon(PokemonTrainer trainer,
        Pokemon pokemon) {
    // Check args
    if (NULL == trainer || NULL == pokemon) return POKEMON_TRAINER_NULL_ARG;

    Pokemon pokemon_copy = pokemonCopy(pokemon);
    PokemonTrainerResult append_result;
    if (NULL == pokemon_copy) return POKEMON_TRAINER_OUT_OF_MEM;
    append_result = pokemonListAppend(trainer->local_pokemon, pokemon_copy);
    if (append_result != POKEMON_TRAINER_SUCCESS) {
        pokemonDestroy(pokemon_copy);
    }
    return append_result;
}

Pokemon pokemonTrainerGetPokemon(PokemonTrainer trainer, int pokemon_index) {
    // Check args
    if (NULL == trainer) return NULL;

    // Return pokemon
    return pokemonListGet(trainer->local_pokemon, pokemon_index);
}

PokemonTrainerResult pokemonTrainerRemovePokemon(PokemonTrainer trainer,
        int pokemon_index) {
    // Check args
    if (NULL == trainer) return POKEMON_TRAINER_NULL_ARG;
    
    return pokemonListRemove(trainer->local_pokemon, pokemon_index, false);
}

PokemonTrainerResult pokemonTrainerDepositPokemon(PokemonTrainer trainer,
        int pokemon_index) {
    if (NULL == trainer) return POKEMON_TRAINER_NULL_ARG;

    Pokemon pokemon_to_deposit;
    PokemonTrainerResult result;

    pokemon_to_deposit =  pokemonListGet(trainer->local_pokemon, pokemon_index);
    if (NULL == pokemon_to_deposit) return POKEMON_TRAINER_INVALID_INDEX;
    
    // Perform the move via append from one list and remove from the other
    result = pokemonListAppend(trainer->remote_pokemon, pokemon_to_deposit);
    if (result != POKEMON_TRAINER_SUCCESS) {
        // Translate error codes to the deposit use case.
        if (result == POKEMON_TRAINER_PARTY_FULL)
            return POKEMON_TRAINER_DEPOSIT_FULL;
        return result;
    }
    result = pokemonListRemove(trainer->local_pokemon, pokemon_index, true);
    if (result != POKEMON_TRAINER_SUCCESS) {
        // Undo the append above in case of error when removing from
        // local_pokemon list
        int last_pokemon = trainer->remote_pokemon->length;
        PokemonTrainerResult undo_result = pokemonListRemove(
                trainer->remote_pokemon, last_pokemon, true);
        assert(undo_result == POKEMON_TRAINER_SUCCESS);
        // Translate error codes for the deposit use case.
        if (result == POKEMON_TRAINER_REMOVE_LAST)
            return POKEMON_TRAINER_DEPOSIT_LAST;
    }

    return result;
}

PokemonTrainerResult pokemonTrainerWithdrawPokemon(PokemonTrainer trainer,
        int pokemon_index) {
    if (NULL == trainer) return POKEMON_TRAINER_NULL_ARG;

    Pokemon pokemon_to_withdraw;
    PokemonTrainerResult result;

    pokemon_to_withdraw = pokemonListGet(trainer->remote_pokemon,
            pokemon_index);
    if (NULL == pokemon_to_withdraw) return POKEMON_TRAINER_INVALID_INDEX;
    
    // Perform the move via append from one list and remove from the other
    result = pokemonListAppend(trainer->local_pokemon, pokemon_to_withdraw);
    if (result != POKEMON_TRAINER_SUCCESS) return result;
    result = pokemonListRemove(trainer->remote_pokemon, pokemon_index, true);
    if (result != POKEMON_TRAINER_SUCCESS) {
        // Undo the append above if there was an error removing from the
        // remote_pokemon list
        int last_pokemon = trainer->local_pokemon->length; 
        PokemonTrainerResult undo_result = pokemonListRemove(
                trainer->local_pokemon, last_pokemon, true);
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
    int combined_party_length, local_length, remote_length;
    local_length = trainer->local_pokemon->length;
    remote_length = trainer->remote_pokemon->length;
    combined_party_length = local_length + remote_length;
    PokemonList combined_party = pokemonListCreate(combined_party_length, 0);
    if (NULL == combined_party) return POKEMON_TRAINER_OUT_OF_MEM;
    pokemonListMove(combined_party, trainer->local_pokemon, 0, 0);
    pokemonListMove(combined_party, trainer->remote_pokemon, local_length, 0);

    // Sort
    pokemonListSort(combined_party);

    // Fill up the local_pokemon list up to the max
    pokemonListMove(trainer->local_pokemon, combined_party, 0, 0);

    // Put the remaining pokemon in the remote_pokemon list
    pokemonListMove(trainer->remote_pokemon, combined_party, 0, local_length);

    // Free the combined_party list. Use ShallowDestroy because we didn't copy
    // the actual pokemon while moving them.
    pokemonListShallowDestroy(combined_party);

    return POKEMON_TRAINER_SUCCESS;
}

PokemonTrainerResult pokemonTrainerPrintEnumeration(PokemonTrainer trainer,
        FILE* file) {
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
