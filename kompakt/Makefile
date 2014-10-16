CXXFLAGS = -g -Wall -Wno-unused

all:
	g++ -o kompakt $(CXXFLAGS) main.cpp kompakt.cpp mystl.cpp
	g++ -o test1 $(CXXFLAGS) test1.cpp kompakt.cpp mystl.cpp
	g++ -o test2 $(CXXFLAGS) test2.cpp mystl.cpp
	g++ -o test3 $(CXXFLAGS) test3.cpp mystl.cpp

test: force
	./test1
	valgrind -q --error-exitcode=1 --leak-check=full ./test1
	./test2

force:

clean:
	rm -Rvf *.o test1 test2 test3 kompakt

