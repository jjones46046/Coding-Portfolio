CXX = g++
CXXFLAGS = -Wall

proj5: proj5.cpp WordCloud.o LL.cpp
	$(CXX) $(CXXFLAGS) proj5.cpp WordCloud.o LL.cpp -o proj5

WordCloud.o: WordCloud.h WordCloud.cpp LL.cpp
	$(CXX) $(CXXFLAGS) -c WordCloud.cpp

clean:
	rm *~
	rm *.o
	rm vg*

run1:
	./proj5 proj5_test1.txt

run2:
	./proj5 proj5_test2.txt

run3:
	./proj5 proj5_test3.txt

run4:
	./proj5 proj5_test4.txt

val1:
	valgrind ./proj5 proj5_test1.txt

val2:
	valgrind ./proj5 proj5_test2.txt

val3:
	valgrind ./proj5 proj5_test3.txt

val4:
	valgrind ./proj5 proj5_test4.txt
