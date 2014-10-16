CXXFLAGS = -g -Wall -Wno-unused
VALGRIND = -q --error-exitcode=1 --leak-check=full

all:
	g++ -o kompakt $(CXXFLAGS) main.cpp kompakt.cpp mystl.cpp
	g++ -o test1 $(CXXFLAGS) test1.cpp kompakt.cpp mystl.cpp
	g++ -o test2 $(CXXFLAGS) test2.cpp mystl.cpp
	g++ -o test3 $(CXXFLAGS) test3.cpp mystl.cpp

test: force
	valgrind $(VALGRIND) ./test1
	valgrind $(VALGRIND) ./test2
	valgrind $(VALGRIND) ./test3

force:

clean:
	rm -Rvf *.o test1 test2 test3 kompakt

