CXX = g++
CXXFLAGS = -Wall

proj3: Pokemon.o PokemonList.o Game.o proj3.cpp
	$(CXX) $(CXXFLAGS) PokemonList.o Pokemon.o Game.o proj3.cpp -o proj3

Game.o: Game.h Game.cpp PokemonList.o Pokemon.o
	$(CXX) $(CXXFLAGS) -c Game.cpp

PokemonList.o: PokemonList.h PokemonList.cpp Pokemon.o
	$(CXX) $(CXXFLAGS) -c PokemonList.cpp

Pokemon.o: Pokemon.h Pokemon.cpp
	$(CXX) $(CXXFLAGS) -c Pokemon.cpp

run:
	./proj3 proj3_data.txt

clean:
	-rm -f *~ *.o vg*

val:
	valgrind ./proj3 proj3_data.txt
