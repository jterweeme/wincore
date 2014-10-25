
%.o: %.cpp
	gpp -g -Wall -Wno-unused -Wno-parentheses -c -o $@ $<

.PHONY: all

all: kompakt.exe test1.exe test2.exe test3.exe test4.exe test5.exe test6.exe

kompakt.exe: main.o kompakt.o filesys.o mystl.o
	gpp -o $@ $(CXXFLAGS) $^

test1.exe: test1.o kompakt.o filesys.o mystl.o
	gpp -o $@ $(CXXFLAGS) $^

test2.exe: test2.o kompakt.o filesys.o mystl.o
	gpp -o $@ $(CXXFLAGS) $^

test3.exe: test3.o kompakt.o filesys.o mystl.o
	gpp -o $@ $(CXXFLAGS) $^

test4.exe: test4.o kompakt.o filesys.o mystl.o test4.o
	gpp -o $@ $(CXXFLAGS) $^

test5.exe: test5.o mystl.o
	gpp -o $@ $(CXXFLAGS) $^

test6.exe: test6.o mystl.o
	gpp -o $@ $(CXXFLAGS) $^

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

test: ttests

testisos:
	bunzip2 -kf *.iso.bz2
	valgrind $(VALGRIND) ./kompakt -l -v -f cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -l -v -s < cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -d -f cd1.iso > /dev/null

ttests: ttest1 ttest2 ttest3 ttest4 ttest5 ttest6

cd1.iso: cd1~1.bz2
	bzcat $< > $@

cd5.iso: cd5~1.bz2
	bzcat $< > $@

cd6.iso: cd6~1.bz2
	bzcat $< > $@

ttest1: cd1.iso cd6.iso
	test1

ttest2:
	test2

ttest3:
	test3

ttest4: cd5.iso
	test4

ttest5:
	test5

ttest6:
	test6

testbattery:
	kompakt -x -f battery.iso -o battery
	md5sum -c battery.md5
	rm -Rvf battery

clean:
	rm -Rvf *.o test1 test2 test3 test4 test5 test6 kompakt

