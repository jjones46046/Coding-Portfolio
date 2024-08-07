CXX = g++
CXXFLAGS = -Wall

driver: bNode.o wordTree.o bookTree.o driver.cpp
	$(CXX) $(CXXFLAGS) wordTree.o bNode.o bookTree.o driver.cpp -o drive.out

bNode.o:  bNode.h bNode.cpp
	$(CXX) $(CXXFLAGS) -c bNode.cpp

wordTree.o: wordTree.h wordTree.cpp
	$(CXX) $(CXXFLAGS) -c wordTree.cpp

bookTree.o: bookTree.h bookTree.cpp
	$(CXX) $(CXXFLAGS) -c bookTree.cpp

clean:
	rm *~
	rm *.o
	rm vg*
run:
	./drive.out
val:
	valgrind ./drive.out

