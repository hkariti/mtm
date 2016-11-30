.PHONY: all all_debug test test_debug zip

CFLAGS:=--std=c99 -Wall -pedantic-errors -Werror

all: pokemon_test pokemon_trainer_test

all_debug: pokemon_test_debug pokemon_trainer_test_debug

zip: pokemon.c pokemon_trainer.c pokemon_trainer.h test/*
	rm -f ex1.zip
	zip -r ex1.zip pokemon.c pokemon_trainer.c pokemon_trainer.h test

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

pokemon_test_debug: pokemon.h pokemon.c test/pokemon_test.c
	gcc -o pokemon_test ${CLFAGS} -g pokemon.c test/pokemon_test.c
pokemon_trainer_test_debug: pokemon_trainer.c pokemon_trainer.h test/pokemon_trainer_test.c
	gcc -o pokemon_trainer_test ${CFLAGS} -g pokemon.c pokemon_trainer.c test/pokemon_trainer_test.c
