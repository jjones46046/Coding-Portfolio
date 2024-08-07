CXX = g++
CXXFLAGS = -Wall

proj2: Diver.o Game.o proj2.cpp
	$(CXX) $(CXXFLAGS) Diver.o Game.o proj2.cpp -o proj2

Game.o: Diver.o Game.cpp Game.h
	$(CXX) $(CXXFLAGS) -c Game.cpp

Diver.o: Diver.h Diver.cpp Material.h
	$(CXX) $(CXXFLAGS) -c Diver.cpp

clean:
	rm *.o*
	rm *~ 

run:
	./proj2
