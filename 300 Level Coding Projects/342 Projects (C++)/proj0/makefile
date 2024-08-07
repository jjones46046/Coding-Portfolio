CXX = g++
CXXFLAGS = -Wall

driver:  queue.h mytest.cpp
	$(CXX) $(CXXFLAGS) mytest.cpp -o mytest.out
clean:
	rm *~
	rm *.o
	rm vg*
run:
	./mytest.out
val:
	valgrind ./mytest.out
