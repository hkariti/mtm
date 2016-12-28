CFLAGS=-Wall -pedantic-errors -Werror -DNDEBUG --std=c99
LDFLAGS=-L. -lmtm -lm
modules=pokedex evolutions inventory store
objects=$(foreach mod,$(modules), $(mod).o)
tests=$(foreach mod,$(modules), $(mod)_test)

evolutions.o: map.h pokedex.h set.h
pokedex.o: map.h set.h
store_test: inventory.o
trainer_test: store.o location.o pokemon.o inventory.o pokedex.o evolutions.o tests/mocks/trainer_mocks.c tests/mocks/*.c
trainer_test.o: tests/trainer_test.c trainer.o .o pokemon.o set.h \
	list.h print_utils.h pokedex.h evolutions.h location.h store.h \
	inventory.h utils.h tests/mocks/pokedex_mocks.h \
	tests/mocks/evolutions_mocks.h tests/mocks/store_mocks.h \
	tests/mocks/pokemon_mocks.h tests/mocks/trainer_mocks.h \
	tests/mocks/location_mocks.h tests/test_utilities.h


%_test: CFLAGS+=-I. -g
%_test: tests/%_test.c %.o utils.o
	gcc ${CFLAGS} -o $@ $^ ${LDFLAGS}
	
tests: $(tests)
.PHONY: tests
