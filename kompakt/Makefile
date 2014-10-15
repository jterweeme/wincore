CXXFLAGS = -g -Wall -Wno-unused

all:
	g++ -o kompakt $(CXXFLAGS) main.cpp kompakt.cpp mystl.cpp
	g++ -o test1 $(CXXFLAGS) test1.cpp kompakt.cpp mystl.cpp
	g++ -o test2 $(CXXFLAGS) test2.cpp mystl.cpp
	g++ -o test3 $(CXXFLAGS) test3.cpp mystl.cpp

test:
	./test1
	./test2


