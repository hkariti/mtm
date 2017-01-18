CXX=g++
CXXFLAGS=-std=c++11 -Wall  -pedantic-errors
LDFLAGS=-lmtm -Llibmtm/mac
DEBUG=-DNDEBUG
modules=item pokemon trainer
objects=pokemon.o trainer.o
tests=item_test

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
