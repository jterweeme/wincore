CXXFLAGS = -Wall -Wno-parentheses --std=c++11 -O3
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = #valgrind $(VALFLAGS)

%.o: %.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

%: %.o
	g++ -o $@ $^

.PHONY: all

all: mystl.o bunzip2 cat crc32 grep kompakt nl test1 testbinp tr yes
	g++ $(CXXFLAGS) -o md5s md5s.cpp mystl.o hasher.cpp
	g++ $(CXXFLAGS) -o jpg2tga jpg2tga.cpp -lm
	g++ $(CXXFLAGS) -o uuidgen uuidgen.cpp
	g++ $(CXXFLAGS) -o od od.cpp mystl.o odmain.cpp
	g++ $(CXXFLAGS) -o base64 base64.cpp

bunzip2: bunzip2.o bitinput.o
cat: cat.o
crc32: crc32.o
kompakt: kompakt.o main.o mystl.o filesys.o
md5s: md5s.o mystl.o hasher.o
nl: nl.o
test1: test1.o mystl.o hasher.o
testbinp: testbinp.o bitinput.o
tr: tr.o
yes: yes.o

bitinput.o: bitinput.cpp bitinput.h
bunzip2.o: bunzip2.cpp bitinput.h
cat.o: cat.cpp
crc32.o: crc32.cpp
filesys.o: filesys.cpp filesys.h
hasher.o: hasher.cpp hasher.h
kompakt.o: kompakt.cpp kompakt.h common.h mystl.h mystl.tcc
main.o: main.cpp
mystl.o: mystl.cpp
nl.o: nl.cpp
test1.o: test1.cpp
testbinp.o: testbinp.cpp
tr.o: tr.cpp
yes.o: yes.cpp

grep: grep.cpp
	g++ $(CXXFLAGS) -o $@ $<

test:
	$(VALGRIND) ./test1
	$(VALGRIND) ./md5s -c data.md5
	$(VALGRIND) ./md5s zero.dat whouse.jpg neucastl.jpg tr.vcxproj | diff md5s.od -
	$(VALGRIND) ./jpg2tga whouse.jpg whouse.tga
	$(VALGRIND) ./od zero.dat | diff zero.od -
	$(VALGRIND) ./base64 zero.dat | diff zero.b64 -

clean:
	rm -Rvf *.o jpg2tga *.tga od md5s test1 uuidgen base64 grep yes cat tr nl crc32
	rm -Rvf kompakt bunzip2 testbinp

rebuild: clean all


