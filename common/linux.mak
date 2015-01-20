CXXFLAGS = -Wall -Wno-parentheses -g --std=c++11
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = #valgrind $(VALFLAGS)

%.o: %.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

%: %.o
	g++ -o $@ $^

.PHONY: all

all: base64 bunzip2 bzcat cat crc32 grep jpg2tga kompakt md5s nl od test1 testbinp tr uuidgen yes
base64: base64.o
bunzip2: bunzip2.o bitinput.o
bzcat: bzcat.o bitinput.o
cat: cat.o
crc32: crc32.o
grep: grep.o
jpg2tga: jpg2tga.o
kompakt: kompakt.o main.o mystl.o filesys.o
md5s: md5s.o mystl.o hasher.o
nl: nl.o
od: od.o mystl.o odmain.o
test1: test1.o mystl.o hasher.o
testbinp: testbinp.o bitinput.o
tr: tr.o
uuidgen: uuidgen.o
yes: yes.o
base64.o: base64.cpp
bitinput.o: bitinput.cpp bitinput.h
bunzip2.o: bunzip2.cpp bitinput.h
bzcat.o: bzcat.cpp bitinput.h
cat.o: cat.cpp
crc32.o: crc32.cpp
filesys.o: filesys.cpp filesys.h
grep.o: grep.cpp
hasher.o: hasher.cpp hasher.h
jpg2tga.o: jpg2tga.cpp
kompakt.o: kompakt.cpp kompakt.h common.h mystl.h mystl.tcc
main.o: main.cpp
mystl.o: mystl.cpp
nl.o: nl.cpp
od.o: od.cpp
odmain.o: odmain.cpp
test1.o: test1.cpp
testbinp.o: testbinp.cpp
tr.o: tr.cpp
uuidgen.o: uuidgen.cpp
yes.o: yes.cpp

test:
	$(VALGRIND) ./test1
	$(VALGRIND) ./md5s zero.dat whouse.jpg neucastl.jpg tr.vcxproj | diff md5s.od -
	$(VALGRIND) ./jpg2tga whouse.jpg whouse.tga
	$(VALGRIND) ./od zero.dat | diff zero.od -
	$(VALGRIND) ./base64 zero.dat | diff zero.b64 -
	$(VALGRIND) ./bunzip2 battery.bz2 battery.iso
	$(VALGRIND) ./md5s -c data.md5
	$(VALGRIND) ./bzcat battery.bz2 | ./kompakt -l -s

clean:
	rm -Rvf *.o jpg2tga *.tga od md5s test1 uuidgen base64 grep yes cat tr nl crc32
	rm -Rvf kompakt bunzip2 testbinp battery.iso bzcat

rebuild: clean all


