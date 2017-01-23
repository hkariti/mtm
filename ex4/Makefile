CXX=g++
CXXFLAGS=-std=c++11 -Wall  -pedantic-errors
LDFLAGS=-lmtm -Llibmtm/mac
DEBUG=-DNDEBUG
modules=item pokemon trainer pokestop
objects=pokemon.o trainer.o pokestop.o
tests=item_test pokemon_test trainer_test pokestop_test k_graph_mtm_test

.PHONY: tests clean

tests: DEBUG=-g
tests: $(tests)
%_test: %_test.o test_utils.o $(objects)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o:
	$(CXX) $(CXXFLAGS) $(DEBUG) -c -o $@ $<

clean:
	rm -f *.o *_test

pokemon.o: pokemon.cc pokemon.h exceptions.h
item_test.o: tests/item_test.cc tests/../item.h tests/../pokemon.h \
  tests/test_utils.h
pokemon_test.o: tests/pokemon_test.cc tests/test_utils.h \
  tests/../pokemon.h tests/../exceptions.h
trainer_test.o: tests/trainer_test.cc tests/test_utils.h \
  tests/../trainer.h tests/../pokemon.h tests/../exceptions.h
trainer.o: trainer.cc trainer.h pokemon.h exceptions.h
test_utils.o: tests/test_utils.cc tests/test_utils.h
pokestop.o: pokestop.cc pokestop.h location.h exceptions.h trainer.h \
	  pokemon.h item.h
pokestop_test.o: tests/pokestop_test.cc tests/../pokestop.h \
	  tests/../location.h tests/../exceptions.h tests/../trainer.h \
	  tests/../pokemon.h tests/../item.h tests/test_utils.h
k_graph_mtm_test.o: tests/k_graph_mtm_test.cc tests/test_utils.h \
	  tests/../k_graph_mtm.h tests/../exceptions.h
