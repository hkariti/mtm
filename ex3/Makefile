CFLAGS=-Wall -pedantic-errors -Werror -DNDEBUG --std=c99
LDFLAGS=-L. -lmtm -lm
modules=pokedex evolutions
objects=$(foreach mod,$(modules), $(mod).o)
tests=$(foreach mod,$(modules), $(mod)_test)

evolutions.o: map.h pokedex.h set.h
pokedex.o: map.h set.h
store_test: inventory.o

%_test: CFLAGS+=-I. -g
%_test: tests/%_test.c %.o utils.o
	gcc ${CFLAGS} -o $@ $^ ${LDFLAGS}
	
tests: $(tests)
.PHONY: tests
