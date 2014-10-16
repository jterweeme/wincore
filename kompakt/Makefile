CXXFLAGS = -g -Wall -Wno-unused
VALGRIND = -q --error-exitcode=1 --leak-check=full

all: kompakt.o
	g++ -o kompakt $(CXXFLAGS) main.cpp kompakt.o mystl.cpp
	g++ -o test1 $(CXXFLAGS) test1.cpp kompakt.o mystl.cpp
	g++ -o test2 $(CXXFLAGS) test2.cpp mystl.cpp
	g++ -o test3 $(CXXFLAGS) test3.cpp mystl.cpp
	g++ -o test4 $(CXXFLAGS) test4.cpp kompakt.o

kompakt.o: kompakt.cpp
	g++ -c $(CXXFLAGS) $<

test: force
	bunzip2 -kf *.iso.bz2
	valgrind $(VALGRIND) ./kompakt -l -v -f cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -l -v -s < cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -d -f cd1.iso > /dev/null
	valgrind $(VALGRIND) ./test1
	valgrind $(VALGRIND) ./test2
	valgrind $(VALGRIND) ./test3 > /dev/null
	valgrind $(VALGRIND) ./test4 > /dev/null

force:

clean:
	rm -Rvf *.o test1 test2 test3 kompakt

