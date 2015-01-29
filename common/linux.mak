CXXFLAGS = -Wall -Wno-parentheses -g --std=c++11
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = #valgrind $(VALFLAGS)

TARGETS = base64 bunzip2 bzcat bzip2 cat cp crc32 dd diff dos2unix grep gunzip gzip \
    jpg2tga kompakt ls md5s nl od tar tee test1 testbinp \
    tgunzip1 touch tr unix2dos uuidgen wingroup yes zcat

%.o: %.cpp
	@g++ $(CXXFLAGS) -c -o $@ $<

%: %.o
	@g++ -o $@ $^

.PHONY: all

all: $(TARGETS)
base64: base64.o
bunzip2: bunzip2m.o bunzip2.o bitinput.o
bzcat: bzcat.o bitinput.o bunzip2.o
bzip2: bzip2.o
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
ls: ls.o
md5s: md5s.o mystl.o hasher.o
nl: nl.o
od: od.o mystl.o odmain.o
tar: tar.o tarm.o
tee: tee.o
test1: test1.o mystl.o hasher.o
testbinp: testbinp.o bitinput.o
tgunzip1: tgunzip1.o gunzip.o
touch: touch.o
tr: tr.o
unix2dos: unix2dos.o mystl.o
uuidgen: uuidgen.o
wingroup: wingroup.o
yes: yes.o
zcat: zcat.o gunzip.o
base64.o: base64.cpp
bitinput.o: bitinput.cpp bitinput.h
bunzip2.o: bunzip2.cpp bunzip2.h bitinput.h
bzcat.o: bzcat.cpp bunzip2.h bitinput.h
bzip2.o: bzip2.cpp
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
ls.o: ls.cpp
main.o: main.cpp
mystl.o: mystl.cpp
nl.o: nl.cpp
od.o: od.cpp
odmain.o: odmain.cpp
tar.o: tar.cpp tar.h
tarm.o: tarm.cpp tar.h
tee.o: tee.cpp
test1.o: test1.cpp
testbinp.o: testbinp.cpp
tgunzip1.o: tgunzip1.cpp
touch.o: touch.cpp
tr.o: tr.cpp
unix2dos.o: unix2dos.cpp
uuidgen.o: uuidgen.cpp
wingroup.o: wingroup.cpp
yes.o: yes.cpp
zcat.o: zcat.cpp gunzip.h inflate.h

test1go: test1
	$(VALGRIND) ./test1

tgunzip1go: tgunzip1
	$(VALGRIND) ./tgunzip1 > /dev/null

testgunzip2:
	rm -f znew.txt
	$(VALGRIND) ./gunzip znew.gz znew.txt
	$(VALGRIND) ./md5s -c znew.md5
	rm -f znew.txt

test: testgunzip2 test1go tgunzip1go
	$(VALGRIND) ./md5s zero.dat whouse.jpg neucastl.jpg tr.vcxproj | ./diff -s md5s.od -
	$(VALGRIND) ./jpg2tga whouse.jpg whouse.tga
	$(VALGRIND) ./od zero.dat | ./diff -s zero.od -
	$(VALGRIND) ./base64 zero.dat | ./diff -s zero.b64 -
	$(VALGRIND) ./bunzip2 battery.bz2 battery.iso
	$(VALGRIND) ./md5s -c data.md5
	$(VALGRIND) ./bzcat battery.bz2 | ./kompakt -l -s | ./diff -s kompakt1.out -
	$(VALGRIND) ./grep include Makefile | ./diff -s grep1.out -
	$(VALGRIND) ./md5s -c data.md5

clean:
	rm -Rf *.o jpg2tga *.tga $(TARGETS)
	rm -Rf battery.iso

rebuild: clean all


