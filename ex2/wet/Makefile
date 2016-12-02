.PHONY: all all_debug test test_debug

CFLAGS:=--std=c99 -Wall -pedantic-errors -Werror -I.

all: pokemon_test pokemon_trainer_test
all_debug: pokemon_test_debug pokemon_trainer_test_debug

test: all
	./pokemon_test
	./pokemon_trainer_test

test_debug: all_debug
	./pokemon_test_debug
	./pokemon_trainer_test_debug

pokemon_test: pokemon.h pokemon.c test/pokemon_test.c
	gcc -o pokemon_test ${CFLAGS} -DNDEBUG pokemon.c test/pokemon_test.c
pokemon_trainer_test: pokemon_trainer.c pokemon_trainer.h test/pokemon_trainer_test.c
	gcc -o pokemon_trainer_test ${CFLAGS} -DNDEBUG pokemon.c pokemon_trainer.c test/pokemon_trainer_test.c

pokemon_test_debug: pokemon.h pokemon.c pokemon_trainer.c
	gcc -o pokemon_test ${CLFAGS} -g pokemon.c test/pokemon_test.c
pokemon_trainer_test_debug: pokemon_trainer.c pokemon_trainer.h test/pokemon_trainer_test.c
	gcc -o pokemon_trainer_test ${CFLAGS} -g pokemon.c pokemon_trainer.c test/pokemon_trainer_test.c
