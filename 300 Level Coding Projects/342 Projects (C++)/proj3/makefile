CXX = g++
CXXFLAGS = -g

test: mytest.cpp pqueue.o
	$(CXX) $(CXXFLAGS) mytest.cpp pqueue.o -o test.out

pqueue.o:  pqueue.h pqueue.cpp
	$(CXX) $(CXXFLAGS) -c pqueue.cpp

clean:
	rm *~
	rm *.o
	rm vg*
run:
	./test.out
val:
	valgrind -s --track-origins=yes ./test.out
