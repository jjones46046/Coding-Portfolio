CXX = g++
CXXFLAGS = -Wall

driver: csr.o csrlist.o driver.cpp
	$(CXX) $(CXXFLAGS) csrlist.o csr.o driver.cpp -o drive.out

csr.o:	csr.h csr.cpp
	$(CXX) $(CXXFLAGS) -c csr.cpp

csrlist.o: csrlist.h csrlist.cpp csr.o
	$(CXX) $(CXXFLAGS) -c csrlist.cpp

clean:
	rm *~
	rm *.o
	rm vg*
run:
	./drive.out
val:
	valgrind ./drive.out 

