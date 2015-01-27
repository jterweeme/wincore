CXXFLAGS = -Wall -Wno-parentheses -g --std=c++11
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = #valgrind $(VALFLAGS)

TARGETS = base64 bunzip2 bzcat cat cp crc32 dd diff dos2unix grep gunzip gzip \
    jpg2tga kompakt md5s nl \
    od tar test1 testbinp tgunzip1 tr unix2dos uuidgen yes

%.o: %.cpp
	@g++ $(CXXFLAGS) -c -o $@ $<

%: %.o
	@g++ -o $@ $^

.PHONY: all

all: $(TARGETS)
base64: base64.o
bunzip2: bunzip2.o bitinput.o bzip2.o
bzcat: bzcat.o bitinput.o bzip2.o
cat: cat.o
cp: cp.o
crc32: crc32.o
dd: dd.o
diff: diff.o
dos2unix: dos2unix.o mystl.o
grep: grep.o
gunzip: gunzip.o gunzipm.o
gzip: gzip.o
jpg2tga: jpg2tga.o
kompakt: kompakt.o main.o mystl.o filesys.o
md5s: md5s.o mystl.o hasher.o
nl: nl.o
od: od.o mystl.o odmain.o
tar: tar.o
test1: test1.o mystl.o hasher.o
testbinp: testbinp.o bitinput.o
tgunzip1: tgunzip1.o gunzip.o
tr: tr.o
unix2dos: unix2dos.o mystl.o
uuidgen: uuidgen.o
yes: yes.o
base64.o: base64.cpp
bitinput.o: bitinput.cpp bitinput.h
bunzip2.o: bunzip2.cpp bzip2.h bitinput.h
bzcat.o: bzcat.cpp bzip2.h bitinput.h
bzip2.o: bzip2.cpp bzip2.h bitinput.h
cat.o: cat.cpp
cp.o: cp.cpp
crc32.o: crc32.cpp
dd.o: dd.cpp
diff.o: diff.cpp
dos2unix.o: dos2unix.cpp
filesys.o: filesys.cpp filesys.h
grep.o: grep.cpp
gunzip.o: gunzip.cpp gunzip.h
gunzipm.o: gunzipm.cpp gunzip.h
gzip.o: gzip.cpp
hasher.o: hasher.cpp hasher.h
jpg2tga.o: jpg2tga.cpp
kompakt.o: kompakt.cpp kompakt.h common.h mystl.h mystl.tcc
main.o: main.cpp
mystl.o: mystl.cpp
nl.o: nl.cpp
od.o: od.cpp
odmain.o: odmain.cpp
tar.o: tar.cpp
test1.o: test1.cpp
testbinp.o: testbinp.cpp
tgunzip1.o: tgunzip1.cpp
tr.o: tr.cpp
unix2dos.o: unix2dos.cpp
uuidgen.o: uuidgen.cpp
yes.o: yes.cpp

test1go: test1
	$(VALGRIND) ./test1

tgunzip1go: tgunzip1
	$(VALGRIND) ./tgunzip1

test: test1go tgunzip1go
	$(VALGRIND) ./md5s zero.dat whouse.jpg neucastl.jpg tr.vcxproj | ./diff -s md5s.od -
	$(VALGRIND) ./jpg2tga whouse.jpg whouse.tga
	$(VALGRIND) ./od zero.dat | ./diff -s zero.od -
	$(VALGRIND) ./base64 zero.dat | ./diff -s zero.b64 -
	$(VALGRIND) ./bunzip2 battery.bz2 battery.iso
	$(VALGRIND) ./md5s -c data.md5
	$(VALGRIND) ./bzcat battery.bz2 | ./kompakt -l -s | ./diff -s kompakt1.out -
	$(VALGRIND) ./grep include Makefile | ./diff -s grep1.out -
	$(VALGRIND) ./gzip boston.mp3
	gunzip boston.mp3.gz
	$(VALGRIND) ./md5s -c data.md5

clean:
	rm -Rvf *.o jpg2tga *.tga $(TARGETS)
	rm -Rvf battery.iso

rebuild: clean all


