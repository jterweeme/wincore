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

test: ttests testisos testbattery testfdfullcd

testisos: cd1.iso
	valgrind $(VALGRIND) ./kompakt -l -v -f cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -l -v -s < cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -d -f cd1.iso > /dev/null

ttests: ttest1 ttest2 ttest3 ttest4 ttest5 ttest6

cd1.iso: cd1.iso.bz2
	bunzip2 -kf $<

cd2.iso: cd2.iso.bz2
	bunzip2 -kf $<

cd5.iso: cd5.iso.bz2
	bunzip2 -kf $<

cd6.iso: cd6.iso.bz2
	bunzip2 -kf $<

ttest1: cd1.iso cd6.iso
	@valgrind $(VALGRIND) ./test1 > /dev/null

ttest2:
	@valgrind $(VALGRIND) ./test2 > /dev/null

ttest3:
	@valgrind $(VALGRIND) ./test3 > /dev/null

ttest4: cd5.iso
	@valgrind $(VALGRIND) ./test4 > /dev/null

ttest5:
	@valgrind $(VALGRIND) ./test5 > /dev/null

ttest6:
	@valgrind $(VALGRIND) ./test6 > /dev/null

testbattery:
	@rm -Rf battery
	valgrind $(VALGRIND) ./kompakt -x -f battery.iso -o battery > /dev/null
	md5sum -c battery.md5
	@rm -Rf battery

testfdfullcd:
	@rm -Rf fdfullcd
	valgrind $(VALGRIND) ./kompakt -x -f fdfullcd.iso -o fdfullcd > /dev/null
	md5sum -c fdfullcd.md5
	@rm -Rf fdfullcd


clean:
	rm -Rvf *.o test? cd?.iso kompakt fdfullcd battery

