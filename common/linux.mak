CXXFLAGS = -Wall -Wno-parentheses -g --std=c++11
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = valgrind $(VALFLAGS)
MYSTL_H = mystl.h mystl.tcc
COMMON_H = common.h $(MYSTL_H)
BITINPUT_H = bitinput.h $(COMMON_H)
BUFTEST_H = buftest.h $(COMMON_H)
BUNZIP2_H = bunzip2.h fector.h $(BITINPUT_H)
TAR_H = tar.h $(BUNZIP2_H)
INFLATE_H = inflate.h
GUNZIP_H = gunzip.h $(INFLATE_H)
OD_H = od.h $(COMMON_H)

TARGETS = base64 bunzip2 bzcat bzinfo bzip2 cat cp crc32 date dd diff \
    dos2unix grep gunzip gzip \
    cppcom01 cppcom02 \
    jpg2tga kompakt ls md5sum nl od rm tar tee test1 test2 test3 testbinp \
    teststl1 tgmtime1 tgunzip1 touch tr unix2dos uuidgen weekday wingroup yes zcat

%.o: %.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

%: %.o
	g++ -o $@ $^

.PHONY: all

all: $(TARGETS)
base64: base64.o mystl.o
bunzip2: bunzip2m.o bunzip2.o bitinput.o fector.o mystl.o
bzcat: bzcat.o bitinput.o bunzip2.o fector.o mystl.o
bzinfo: bzinfo.o bitinput.o mystl.o
bzip2: bzip2.o
bzmd5: bzmd5.o
cat: cat.o mystl.o
cp: cp.o
cppcom01: cppcom01.o mystl.o
cppcom02: cppcom02.o mystl.o
crc32: crc32.o
date: date.o mystl.o
dd: dd.o
diff: diff.o
dos2unix: dos2unix.o mystl.o
grep: grep.o
gunzip: gunzip.o gunzipm.o
gzip: gzip.o
jpg2tga: jpg2tga.o
kompakt: kompakt.o main.o mystl.o filesys.o
ls: ls.o
md5sum: md5sum.o mystl.o hasher.o
nl: nl.o mystl.o
od: od.o mystl.o odmain.o
rm: rm.o mystl.o
tar: tarm.o tar.o bitinput.o bunzip2.o fector.o mystl.o
tee: tee.o mystl.o
test1: test1.o mystl.o hasher.o
test2: test2.o mystl.o
test3: test3.o mystl.o
testbinp: testbinp.o bitinput.o mystl.o
teststl1: teststl1.o mystl.o
tgmtime1: tgmtime1.o mystl.o
tgunzip1: tgunzip1.o gunzip.o
touch: touch.o
tr: tr.o mystl.o
unix2dos: unix2dos.o mystl.o
uuidgen: uuidgen.o
weekday: weekday.o mystl.o
wingroup: wingroup.o
yes: yes.o fector.o mystl.o
zcat: zcat.o gunzip.o
base64.o: base64.cpp $(COMMON_H)
bitinput.o: bitinput.cpp $(BITINPUT_H)
bunzip2.o: bunzip2.cpp $(BUNZIP2_H)
bzcat.o: bzcat.cpp $(BUNZIP2_H)
bzinfo.o: bzinfo.cpp fector.h $(BITINPUT_H)
bzip2.o: bzip2.cpp
bzmd5.o: bzmd5.cpp $(BUFTEST_H)
cat.o: cat.cpp $(COMMON_H)
cp.o: cp.cpp
cppcom01.o: cppcom01.cpp
cppcom02.o: cppcom02.cpp
crc32.o: crc32.cpp
date.o: date.cpp $(COMMON_H)
dd.o: dd.cpp
diff.o: diff.cpp
dos2unix.o: dos2unix.cpp
fector.o: fector.cpp
filesys.o: filesys.cpp filesys.h
grep.o: grep.cpp
gunzip.o: gunzip.cpp $(GUNZIP_H)
gunzipm.o: gunzipm.cpp $(GUNZIP_H)
gzip.o: gzip.cpp
hasher.o: hasher.cpp hasher.h
jpg2tga.o: jpg2tga.cpp
kompakt.o: kompakt.cpp kompakt.h $(COMMON_H)
ls.o: ls.cpp
main.o: main.cpp
md5sum.o: md5sum.cpp
mystl.o: mystl.cpp $(MYSTL_H)
nl.o: nl.cpp $(COMMON_H)
od.o: od.cpp od.h $(COMMON_H)
odmain.o: odmain.cpp $(OD_H)
rm.o: rm.cpp
tar.o: tar.cpp $(TAR_H)
tarm.o: tarm.cpp $(TAR_H)
tee.o: tee.cpp $(COMMON_H)
test1.o: test1.cpp
test2.o: test2.cpp
test3.o: test3.cpp
testbinp.o: testbinp.cpp
teststl1.o: teststl1.cpp $(MYSTL_H)
tgmtime1.o: tgmtime1.cpp $(COMMON_H)
tgunzip1.o: tgunzip1.cpp
touch.o: touch.cpp
tr.o: tr.cpp $(COMMON_H)
unix2dos.o: unix2dos.cpp
uuidgen.o: uuidgen.cpp
weekday.o: weekday.cpp
wingroup.o: wingroup.cpp
yes.o: yes.cpp fector.h $(COMMON_H)
zcat.o: zcat.cpp $(GUNZIP_H)

test1go: test1
	$(VALGRIND) ./test1

tgunzip1go: tgunzip1
	$(VALGRIND) ./tgunzip1 > /dev/null

testgunzip2:
	rm -f znew.txt
	$(VALGRIND) ./gunzip znew.gz znew.txt
	$(VALGRIND) ./md5sum -c znew.md5
	rm -f znew.txt

testbzcat:
	$(VALGRIND) ./bzcat battery.bz2 | ./md5sum -x efc57edfaf907b5707878f544b39d6d5
	#$(VALGRIND) ./bzcat 7z920.tar.bz2 | ./md5sum -x 30df35882f3b6e20007e2e9aef07554b

testzcat:
	$(VALGRIND) ./zcat znew.gz | ./md5sum -x 742b0b4d1543c6df46e35b77ec6baa53
	$(VALGRIND) ./zcat arduinousb_release_004.tar.gz | ./md5sum -x 7e830e169f38ad7f28cc5e8da51c515d
	$(VALGRIND) ./zcat cdrtools-3.01a24.tar.gz | ./md5sum -x aa1660fe345a247cf9d7fa116d3ab1d6

testkompakt:
	$(VALGRIND) ./bzcat battery.bz2 | ./kompakt -l -s | ./diff -s kompakt1.out -

testod:
	$(VALGRIND) ./od zero.dat | ./diff -s zero.od -

testbase64:
	$(VALGRIND) ./base64 zero.dat | ./diff -s zero.b64 -

teststl1go:
	$(VALGRIND) ./teststl1

testtar:
	$(VALGRIND) ./tar -tvf dinges.tar | ./diff -s dinges.out -
	$(VALGRIND) ./tar -tvjf dinges.tar.bz2 | ./diff -s dinges.out -

testmd5sum:
	$(VALGRIND) ./md5sum zero.dat whouse.jpg neucastl.jpg tr.vcxproj | ./diff -s md5s.od -
	$(VALGRIND) ./md5sum -c data.md5

testbunzip2:
	$(VALGRIND) ./bunzip2 battery.bz2 battery.iso

testnl:
	$(VALGRIND) ./cat tr.cpp | ./nl | diff nl.out -

testgmtime1:
	$(VALGRIND) ./tgmtime1

tests1: testgunzip2 testkompakt testbunzip2 testmd5sum testgmtime1 testnl testbzcat testzcat

test: tests1 test1go tgunzip1go testod testtar testbase64 teststl1go
	$(VALGRIND) ./jpg2tga whouse.jpg whouse.tga
	$(VALGRIND) ./grep include Makefile | ./diff -s grep1.out -
	$(VALGRIND) ./test2
	$(VALGRIND) ./cppcom01
	$(VALGRIND) ./cppcom02

clean:
	rm -Rf *.o jpg2tga *.tga $(TARGETS)
	rm -Rf battery.iso

rebuild: clean all


