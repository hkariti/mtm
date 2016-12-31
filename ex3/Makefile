CFLAGS=-Wall -pedantic-errors -Werror --std=c99 -DNDEBUG
LDFLAGS=-L. -lmtm -lm
modules=pokedex evolutions inventory store trainer pokemon location pokemonGo
objects=$(addsuffix .o,$(modules))
tests=$(addsuffix _test,$(modules))
mocks=$(foreach mod,$(modules),$(if $(wildcard tests/mocks/$(mod)_mocks.c),$(mod)_mocks.o))

pokemon_go_mtm: pokemon_go_mtm.o utils.o parser.o $(objects)
	$(CC) ${CFLAGS} -o $@ $^ $(LDFLAGS)

map_mtm_test: map_mtm.o map_mtm_test.o utils.o
	$(CC) $(CFLAGS) -o $@ $^

mtm_run:
	$(CC) $(CFLAGS) *.c $(LDFLAGS) -o pokemon_go_mtm
.PHONY: mtm_run

tests: $(tests) map_mtm_test
.PHONY: tests

clean:
	rm -f *.o *_test pokemon_go_mtm
.PHONY: clean


%_test: CFLAGS+=-g
%_test: %_test.o utils.o $(objects) $(mocks)
	$(CC) -o $@ $^ ${LDFLAGS}

%.o:
	$(CC) ${CFLAGS} -c -o $@ $<
	
map_mtm_test.o: tests/map_mtm_test.c tests/test_utilities.h \
	  tests/../map_mtm/map_mtm.h tests/../utils.h
map_mtm.o: map_mtm/map_mtm.c map_mtm/map_mtm.h
evolutions.o: evolutions.c evolutions.h map.h pokedex.h set.h utils.h
inventory.o: inventory.c inventory.h print_utils.h map.h utils.h
location.o: location.c location.h list.h set.h pokemon.h print_utils.h \
  pokedex.h map.h evolutions.h utils.h
pokemon_go_mtm.o: pokemon_go_mtm.c parser.h map.h pokedex.h set.h evolutions.h location.h \
  list.h pokemon.h print_utils.h pokemonGo.h trainer.h store.h \
  inventory.h
parser.o: parser.c parser.h map.h pokedex.h set.h evolutions.h location.h \
  list.h pokemon.h print_utils.h utils.h
pokedex.o: pokedex.c pokedex.h map.h set.h utils.h
pokemon.o: pokemon.c pokemon.h set.h list.h print_utils.h pokedex.h map.h \
  evolutions.h utils.h
pokemonGo.o: pokemonGo.c pokemonGo.h location.h list.h set.h pokemon.h \
  print_utils.h pokedex.h map.h evolutions.h trainer.h store.h \
  inventory.h utils.h
store.o: store.c store.h map.h inventory.h print_utils.h
evolutions_test.o: tests/evolutions_test.c tests/../evolutions.h \
  tests/../map.h tests/../pokedex.h tests/../set.h \
  tests/mocks/evolutions_mocks.h tests/mocks/pokedex_mocks.h \
  tests/mocks/../../utils.h tests/test_utilities.h
inventory_test.o: tests/inventory_test.c tests/../inventory.h \
  tests/../print_utils.h tests/../map.h tests/mocks/inventory_mocks.h \
  tests/test_utilities.h
location_test.o: tests/location_test.c tests/../location.h \
  tests/../list.h tests/../set.h tests/../pokemon.h \
  tests/../print_utils.h tests/../pokedex.h tests/../map.h \
  tests/../evolutions.h tests/../utils.h tests/mocks/evolutions_mocks.h \
  tests/mocks/pokedex_mocks.h tests/mocks/location_mocks.h \
  tests/mocks/pokemon_mocks.h tests/test_utilities.h
evolutions_mocks.o: tests/mocks/evolutions_mocks.c \
  tests/mocks/evolutions_mocks.h tests/mocks/../../evolutions.h \
  tests/mocks/../../map.h tests/mocks/../../pokedex.h \
  tests/mocks/../../set.h tests/mocks/pokedex_mocks.h \
  tests/mocks/../../utils.h
inventory_mocks.o: tests/mocks/inventory_mocks.c \
  tests/mocks/inventory_mocks.h tests/mocks/../../inventory.h \
  tests/mocks/../../print_utils.h tests/mocks/../../map.h
location_mocks.o: tests/mocks/location_mocks.c \
  tests/mocks/location_mocks.h tests/mocks/../../location.h \
  tests/mocks/../../list.h tests/mocks/../../set.h \
  tests/mocks/../../pokemon.h tests/mocks/../../print_utils.h \
  tests/mocks/../../pokedex.h tests/mocks/../../map.h \
  tests/mocks/../../evolutions.h tests/mocks/pokemon_mocks.h
pokedex_mocks.o: tests/mocks/pokedex_mocks.c tests/mocks/pokedex_mocks.h \
  tests/mocks/../../pokedex.h tests/mocks/../../map.h \
  tests/mocks/../../set.h tests/mocks/../../utils.h
pokemon_mocks.o: tests/mocks/pokemon_mocks.c tests/mocks/pokemon_mocks.h \
  tests/mocks/../../pokemon.h tests/mocks/../../set.h \
  tests/mocks/../../list.h tests/mocks/../../print_utils.h \
  tests/mocks/../../pokedex.h tests/mocks/../../map.h \
  tests/mocks/../../evolutions.h
store_mocks.o: tests/mocks/store_mocks.c tests/mocks/store_mocks.h \
  tests/mocks/../../store.h tests/mocks/../../map.h \
  tests/mocks/../../inventory.h tests/mocks/../../print_utils.h
trainer_mocks.o: tests/mocks/trainer_mocks.c tests/mocks/trainer_mocks.h \
  tests/mocks/location_mocks.h tests/mocks/../../location.h \
  tests/mocks/../../list.h tests/mocks/../../set.h \
  tests/mocks/../../pokemon.h tests/mocks/../../print_utils.h \
  tests/mocks/../../pokedex.h tests/mocks/../../map.h \
  tests/mocks/../../evolutions.h tests/mocks/pokemon_mocks.h \
  tests/mocks/pokedex_mocks.h tests/mocks/../../utils.h \
  tests/mocks/evolutions_mocks.h tests/mocks/store_mocks.h \
  tests/mocks/../../store.h tests/mocks/../../inventory.h \
  tests/mocks/../../trainer.h
pokedex_test.o: tests/pokedex_test.c tests/../pokedex.h tests/../map.h \
  tests/../set.h tests/../utils.h tests/mocks/pokedex_mocks.h \
  tests/test_utilities.h
pokemonGo_test.o: tests/pokemonGo_test.c tests/../pokemonGo.h \
  tests/../location.h tests/../list.h tests/../set.h tests/../pokemon.h \
  tests/../print_utils.h tests/../pokedex.h tests/../map.h \
  tests/../evolutions.h tests/../trainer.h tests/../store.h \
  tests/../inventory.h tests/../utils.h tests/mocks/evolutions_mocks.h \
  tests/mocks/pokedex_mocks.h tests/mocks/location_mocks.h \
  tests/mocks/pokemon_mocks.h tests/mocks/trainer_mocks.h \
  tests/mocks/store_mocks.h tests/test_utilities.h
pokemon_test.o: tests/pokemon_test.c tests/test_utilities.h \
  tests/../pokemon.h tests/../set.h tests/../list.h \
  tests/../print_utils.h tests/../pokedex.h tests/../map.h \
  tests/../evolutions.h tests/../utils.h tests/mocks/evolutions_mocks.h \
  tests/mocks/pokedex_mocks.h tests/mocks/pokemon_mocks.h
store_test.o: tests/store_test.c tests/../store.h tests/../map.h \
  tests/../inventory.h tests/../print_utils.h tests/mocks/store_mocks.h \
  tests/test_utilities.h
trainer_test.o: tests/trainer_test.c tests/../trainer.h tests/../map.h \
  tests/../pokemon.h tests/../set.h tests/../list.h \
  tests/../print_utils.h tests/../pokedex.h tests/../evolutions.h \
  tests/../location.h tests/../store.h tests/../inventory.h \
  tests/../utils.h tests/mocks/trainer_mocks.h \
  tests/mocks/location_mocks.h tests/mocks/pokemon_mocks.h \
  tests/mocks/pokedex_mocks.h tests/mocks/evolutions_mocks.h \
  tests/mocks/store_mocks.h tests/test_utilities.h
trainer.o: trainer.c trainer.h map.h pokemon.h set.h list.h print_utils.h \
  pokedex.h evolutions.h location.h store.h inventory.h utils.h
utils.o: utils.c utils.h
