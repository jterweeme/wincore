VALGRIND = -q --error-exitcode=1 --leak-check=full

%.o: %.cpp
	@g++ -g -Wall -Wno-unused -Wno-sign-compare -Wno-parentheses -c -o $@ $<

.PHONY: all

all: kompakt test1 test2 test3 test4 test5 test6 tester

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

tester: tester.o
	@g++ -o $@ $(CXXFLAGS) $^

kompakt.o: kompakt.cpp kompakt.h common.h mystl.h mystl.tcc
filesys.o: filesys.cpp filesys.h common.h mystl.h mystl.tcc
main.o: main.cpp kompakt.h common.h mystl.h mystl.tcc
mystl.o: mystl.cpp mystl.h mystl.tcc
test1.o: test1.cpp kompakt.h common.h mystl.h mystl.tcc
test2.o: test2.cpp kompakt.h common.h mystl.h mystl.tcc
test3.o: test3.cpp kompakt.h common.h mystl.h mystl.tcc
test4.o: test4.cpp kompakt.h common.h mystl.h mystl.tcc
test5.o: test5.cpp kompakt.h common.h mystl.h mystl.tcc
test6.o: test6.cpp common.h
tester.o: tester.cpp
test: ttests testisos

testisos: tester cd1.iso
	valgrind $(VALGRIND) ./kompakt -l -v -f cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -l -v -s < cd1.iso > /dev/null
	valgrind $(VALGRIND) ./kompakt -d -f cd1.iso > /dev/null
	@./tester2.sh battery.iso.bz2 battery battery.md5
	@./tester2.sh wxphome.iso.bz2 wxphome wxphome.md5
	@./tester2.sh winnt4.iso.bz2 winnt4 winnt4.md5

ttests: ttest1 ttest2 ttest3 ttest5 ttest6

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

clean:
	rm -Rvf *.o test? cd?.iso kompakt fdfullcd battery tester

