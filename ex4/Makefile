CXX=g++
CXXFLAGS=-std=c++11 -Wall  -pedantic-errors
LDFLAGS=-lmtm -Llibmtm/mac
DEBUG=-DNDEBUG
modules=item pokemon trainer pokestop
objects=pokemon.o trainer.o pokestop.o gym.o pokemon_go.o starbucks.o world.o
tests=item_test pokemon_test trainer_test pokestop_test k_graph_mtm_test starbucks_test world_test gym_test pokemon_go_test

.PHONY: tests clean zip


tests: DEBUG=-g
tests: $(tests)
%_test: %_test.o test_utils.o $(objects)
	$(CXX) -o $@ $^ $(LDFLAGS)

zip:
	rm -f ex4.zip
	find . -name '*.cc' -o -name '*.h' | xargs -I '{}' zip ex4.zip '{}' -x '*test_utils*' k_graph.h location.h example_tests/'*' 'libmtm/*'

%.o:
	$(CXX) $(CXXFLAGS) $(DEBUG) -c -o $@ $<

clean:
	rm -f *.o *_test

gym_test.o: tests/gym_test.cc tests/test_utils.h tests/../trainer.h \
	tests/../pokemon.h tests/../item.h tests/../exceptions.h \
	tests/../gym.h tests/../location.h
item_test.o: tests/item_test.cc tests/../item.h tests/../pokemon.h \
	tests/../exceptions.h tests/test_utils.h
k_graph_mtm_test.o: tests/k_graph_mtm_test.cc tests/test_utils.h \
	tests/../k_graph_mtm.h tests/../exceptions.h
pokemon_go_test.o: tests/pokemon_go_test.cc tests/../pokemon_go.h \
	tests/../world.h tests/../k_graph.h tests/../location.h \
	tests/../exceptions.h tests/../trainer.h tests/../pokemon.h \
	tests/../item.h tests/test_utils.h
pokemon_test.o: tests/pokemon_test.cc tests/test_utils.h \
	tests/../pokemon.h tests/../exceptions.h
pokestop_test.o: tests/pokestop_test.cc tests/../pokestop.h \
	tests/../location.h tests/../exceptions.h tests/../trainer.h \
	tests/../pokemon.h tests/../item.h tests/test_utils.h
starbucks_test.o: tests/starbucks_test.cc tests/test_utils.h \
	tests/../trainer.h tests/../pokemon.h tests/../item.h \
	tests/../exceptions.h tests/../starbucks.h tests/../location.h
trainer_test.o: tests/trainer_test.cc tests/test_utils.h \
	tests/../trainer.h tests/../pokemon.h tests/../item.h \
	tests/../exceptions.h
world_test.o: tests/world_test.cc tests/test_utils.h tests/../world.h \
	tests/../k_graph.h tests/../location.h tests/../exceptions.h \
	tests/../trainer.h tests/../pokemon.h tests/../item.h
gym.o: gym.cc gym.h location.h exceptions.h trainer.h pokemon.h item.h
pokemon.o: pokemon.cc pokemon.h exceptions.h
pokemon_go.o: pokemon_go.cc pokemon_go.h world.h k_graph.h location.h \
	exceptions.h trainer.h pokemon.h item.h
pokestop.o: pokestop.cc pokestop.h location.h exceptions.h trainer.h \
	pokemon.h item.h
starbucks.o: starbucks.cc starbucks.h location.h exceptions.h trainer.h \
	pokemon.h item.h
trainer.o: trainer.cc trainer.h pokemon.h item.h exceptions.h
world.o: world.cc world.h k_graph.h location.h exceptions.h trainer.h \
	pokemon.h item.h gym.h pokestop.h starbucks.h
test_utils.o: tests/test_utils.cc tests/test_utils.h
