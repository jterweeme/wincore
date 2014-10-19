CXXFLAGS = -g -Wall -Wno-unused -Wno-sign-compare -Wno-parentheses
VALGRIND = -q --error-exitcode=1 --leak-check=full

all: kompakt test1 test2 test4 kompakt.o main.o mystl.o
	g++ -o test3 $(CXXFLAGS) test3.cpp mystl.cpp

kompakt: main.o kompakt.o mystl.o
	g++ -o kompakt $(CXXFLAGS) $^

test1: test1.o kompakt.o mystl.o
	g++ -o $@ $(CXXFLAGS) $^

test2: test2.o kompakt.o mystl.o
	g++ -o $@ $(CXXFLAGS) $^

test4: test4.o kompakt.o mystl.o test4.o
	g++ -o $@ $(CXXFLAGS) $^

kompakt.o: kompakt.cpp kompakt.h common.h mystl.h
	g++ -c $(CXXFLAGS) $<

main.o: main.cpp kompakt.h common.h mystl.h
	g++ -c $(CXXFLAGS) $<

mystl.o: mystl.cpp mystl.h
	g++ -c $(CXXFLAGS) $<

test1.o: test1.cpp kompakt.h common.h mystl.h
	g++ -c $(CXXFLAGS) $<

test2.o: test2.cpp kompakt.h common.h mystl.h
	g++ -c $(CXXFLAGS) $<

test4.o: test4.cpp kompakt.h common.h mystl.h
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

