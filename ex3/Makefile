CFLAGS=-Wall -pedantic-errors -Werror --std=c99 -I. -DNDEBUG
LDFLAGS=-L. -lmtm -lm
modules=pokedex evolutions inventory store trainer pokemon location
objects=$(foreach mod,$(modules), $(mod).o)
tests=$(foreach mod,$(modules), $(mod)_test)
mocks=$(foreach mod,$(modules), $(mod)_mocks.o)

tests: $(tests)
.PHONY: tests

clean:
	rm -f *.o *_test pokemonGo
.PHONY: clean

%_test: CFLAGS+=-g
%_test: %_test.o $(objects) $(mocks) utils.o
	$(CC) -o $@ $^ ${LDFLAGS}
%.o:
	$(CC) ${CFLAGS} -c -o $@ $<
	
evolutions.o: evolutions.c evolutions.h map.h pokedex.h set.h utils.h
inventory.o: inventory.c inventory.h print_utils.h map.h utils.h
location.o: location.c location.h list.h map.h pokemon.h set.h \
  print_utils.h pokedex.h evolutions.h utils.h
pokedex.o: pokedex.c pokedex.h map.h set.h utils.h
pokemon.o: pokemon.c pokemon.h set.h list.h print_utils.h pokedex.h map.h \
  evolutions.h utils.h
store.o: store.c store.h map.h inventory.h print_utils.h
evolutions_test.o: tests/evolutions_test.c evolutions.h map.h pokedex.h \
  set.h tests/mocks/evolutions_mocks.h tests/mocks/pokedex_mocks.h \
  utils.h tests/test_utilities.h
inventory_test.o: tests/inventory_test.c inventory.h print_utils.h map.h \
  tests/test_utilities.h
evolutions_mocks.o: tests/mocks/evolutions_mocks.c \
  tests/mocks/evolutions_mocks.h evolutions.h map.h pokedex.h set.h \
  tests/mocks/pokedex_mocks.h utils.h
inventory_mocks.o: tests/mocks/inventory_mocks.c \
  tests/mocks/inventory_mocks.h inventory.h print_utils.h map.h
location_mocks.o: tests/mocks/location_mocks.c \
  tests/mocks/location_mocks.h location.h list.h map.h pokemon.h set.h \
  print_utils.h pokedex.h evolutions.h tests/mocks/pokemon_mocks.h
pokedex_mocks.o: tests/mocks/pokedex_mocks.c tests/mocks/pokedex_mocks.h \
  pokedex.h map.h set.h utils.h
pokemon_mocks.o: tests/mocks/pokemon_mocks.c pokemon.h set.h list.h \
  print_utils.h pokedex.h map.h evolutions.h
store_mocks.o: tests/mocks/store_mocks.c tests/mocks/store_mocks.h \
  store.h map.h inventory.h print_utils.h
trainer_mocks.o: tests/mocks/trainer_mocks.c tests/mocks/trainer_mocks.h \
  tests/mocks/location_mocks.h location.h list.h map.h pokemon.h set.h \
  print_utils.h pokedex.h evolutions.h tests/mocks/pokemon_mocks.h \
  tests/mocks/pokedex_mocks.h utils.h tests/mocks/evolutions_mocks.h \
  tests/mocks/store_mocks.h store.h inventory.h trainer.h
pokedex_test.o: tests/pokedex_test.c pokedex.h map.h set.h utils.h \
  tests/mocks/pokedex_mocks.h tests/test_utilities.h
pokemon_test.o: tests/pokemon_test.c tests/test_utilities.h \
  tests/../pokemon.h set.h list.h print_utils.h pokedex.h map.h \
  evolutions.h tests/../utils.h tests/mocks/evolutions_mocks.h \
  tests/mocks/pokedex_mocks.h tests/mocks/pokemon_mocks.h
store_test.o: tests/store_test.c store.h map.h inventory.h print_utils.h \
  tests/test_utilities.h
trainer_test.o: tests/trainer_test.c trainer.h map.h pokemon.h set.h \
  list.h print_utils.h pokedex.h evolutions.h location.h store.h \
  inventory.h utils.h tests/mocks/trainer_mocks.h \
  tests/mocks/location_mocks.h tests/mocks/pokemon_mocks.h \
  tests/mocks/pokedex_mocks.h tests/mocks/evolutions_mocks.h \
  tests/mocks/store_mocks.h tests/test_utilities.h
trainer.o: trainer.c trainer.h map.h pokemon.h set.h list.h print_utils.h \
  pokedex.h evolutions.h location.h store.h inventory.h utils.h
utils.o: utils.c utils.h
