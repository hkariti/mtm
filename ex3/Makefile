LDFLAGS=-L libmtm/mac -lmtm
modules=pokedex evolutions
objects=$(foreach mod,$(modules), $(mod).o)
tests=$(foreach mod,$(modules), test_$(mod))

evolutions.o: map.h pokedex.h set.h
pokedex.o: map.h set.h

test_%: CFLAGS+=-I. -g
test_%: tests/test_%.c %.o
	gcc ${CFLAGS} -o $@ $^ ${LDFLAGS}
	
tests: $(tests)
.PHONY: tests
