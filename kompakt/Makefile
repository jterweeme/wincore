VALGRIND = -q --error-exitcode=1 --leak-check=full

%.o: %.cpp
	@g++ -g -Wall -Wno-unused -Wno-sign-compare -Wno-parentheses -c -o $@ $<

.PHONY: all

all: kompakt test1 test2 test3 test4 test5 test6

kompakt: main.o kompakt.o filesys.o mystl.o
	@g++ -o $@ $(CXXFLAGS) $^

test1: test1.o kompakt.o filesys.o mystl.o
	@g++ -o $@ $(CXXFLAGS) $^

test2: test2.o kompakt.o filesys.o mystl.o
	@g++ -o $@ $(CXXFLAGS) $^

test3: test3.o kompakt.o filesys.o mystl.o
	@g++ -o $@ $(CXXFLAGS) $^

test4: test4.o kompakt.o filesys.o mystl.o test4.o
	@g++ -o $@ $(CXXFLAGS) $^

test5: test5.o mystl.o
	@g++ -o $@ $(CXXFLAGS) $^

test6: test6.o mystl.o
	@g++ -o $@ $(CXXFLAGS) $^

kompakt.o: kompakt.cpp kompakt.h common.h mystl.h
filesys.o: filesys.cpp filesys.h common.h mystl.h
main.o: main.cpp kompakt.h common.h mystl.h
mystl.o: mystl.cpp mystl.h
test1.o: test1.cpp kompakt.h common.h mystl.h
test2.o: test2.cpp kompakt.h common.h mystl.h
test3.o: test3.cpp kompakt.h common.h mystl.h
test4.o: test4.cpp kompakt.h common.h mystl.h
test5.o: test5.cpp kompakt.h common.h mystl.h
test6.o: test6.cpp common.h

test: force ttest5 ttest6 testbattery
	bunzip2 -kf *.iso.bz2
	valgrind $(VALGRIND) ./kompakt -l -v -f cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -l -v -s < cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -d -f cd1.iso > /dev/null
	@valgrind $(VALGRIND) ./test1
	@valgrind $(VALGRIND) ./test2
	@valgrind $(VALGRIND) ./test3 > /dev/null
	@valgrind $(VALGRIND) ./test4 > /dev/null

ttest5:
	@valgrind $(VALGRIND) ./test5 > /dev/null

ttest6:
	@valgrind $(VALGRIND) ./test6 > /dev/null

testbattery:
	valgrind $(VALGRIND) ./kompakt -x -f battery.iso -o battery > /dev/null
	md5sum -c battery.md5
	rm -Rvf battery

force:

clean:
	rm -Rvf *.o test1 test2 test3 test4 test5 test6 kompakt

