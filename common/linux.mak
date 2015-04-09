CXXFLAGS = -Wall -Wno-parentheses -O0 -g --std=c++11
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = #valgrind $(VALFLAGS)
UTIL2_H = util2.h mytypes.h
MYSTL_H = mystl.h mystl.tcc
COMMON_H = common.h $(MYSTL_H)
BITINPUT_H = bitinput.h $(COMMON_H)
BUFTEST_H = buftest.h $(COMMON_H)
BUNZIP2_H = bunzip2.h fector.h $(BITINPUT_H)
TAR_H = tar.h $(BUNZIP2_H)
INFLATE_H = inflate.h fector.h
GUNZIP_H = gunzip.h $(INFLATE_H)
OD_H = od.h $(COMMON_H)
HASHER_H = hasher.h $(COMMON_H)
MYSTDIO_O = mystdio.o util2.o linux64.o
MYSTL_O = mystl.o mytime.o util2.o $(MYSTDIO_O)

TARGETS = base64 bunzip2 bzcat bzinfo bzip2 cat cp crc32 date dd diff \
    dos2unix grep gunzip gzip \
    tcpcom01 tcpcom02 \
    jpg2tga kompakt ls md5sum nl od rm tar \
    tcpcom03 tcpcom04 tcpcom05 tcpcom06 tcpcom07 tcpcom08 tcpcom09 tcpcom10 \
    tcpref01 \
    tee test1 test2 test3 testbinp \
    teststl1 tgmtime1 tgunzip1 touch tr tstdio1 unix2dos uuidgen weekday wingroup yes zcat

%.o: %.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

%.o: %.asm
	yasm -f elf64 $<

%: %.o
	g++ -o $@ $^

.PHONY: all

all: $(TARGETS)
base64: base64.o $(MYSTL_O)
bunzip2: bunzip2m.o bunzip2.o bitinput.o fector.o $(MYSTL_O)
bzcat: bzcat.o bitinput.o bunzip2.o fector.o $(MYSTL_O)
bzinfo: bzinfo.o bitinput.o $(MYSTL_O)
bzip2: bzip2.o
bzmd5: bzmd5.o
cat: cat.o $(MYSTL_O)
cp: cp.o $(MYSTL_O)
tcpcom01: tcpcom01.o $(MYSTL_O)
tcpcom02: tcpcom02.o $(MYSTL_O)
crc32: crc32.o $(MYSTL_O)
date: date.o $(MYSTL_O)
dd: dd.o $(MYSTL_O)
diff: diff.o $(MYSTL_O)
dos2unix: dos2unix.o $(MYSTL_O)
grep: grep.o $(MYSTL_O)
gunzip: gunzip.o gunzipm.o inflate.o $(MYSTL_O)
gzip: gzip.o
jpg2tga: jpg2tga.o $(MYSTL_O)
kompakt: kompakt.o main.o filesys.o $(MYSTL_O)
ls: ls.o
md5sum: md5sum.o hasher.o $(MYSTL_O)
nl: nl.o $(MYSTL_O)
od: od.o odmain.o $(MYSTL_O)
rm: rm.o $(MYSTL_O)
tar: tarm.o tar.o bitinput.o bunzip2.o fector.o $(MYSTL_O)
tcpcom03: tcpcom03.o $(MYSTL_O)
tcpcom04: tcpcom04.o $(MYSTL_O)
tcpcom05: tcpcom05.o $(MYSTL_O)
tcpcom06: tcpcom06.o $(MYSTL_O)
tcpcom07: tcpcom07.o $(MYSTL_O)
tcpcom08: tcpcom08.o $(MYSTL_O)
tcpcom09: tcpcom09.o $(MYSTL_O)
tcpcom10: tcpcom10.o $(MYSTL_O)
tcpref01: tcpref01.o util2.o $(MYSTL_O)
tee: tee.o $(MYSTL_O)
test1: test1.o hasher.o $(MYSTL_O)
test2: test2.o $(MYSTL_O)
test3: test3.o bitinput.o bunzip2.o fector.o $(MYSTL_O)
testbinp: testbinp.o bitinput.o $(MYSTL_O)
teststl1: teststl1.o $(MYSTL_O)
tgmtime1: tgmtime1.o $(MYSTL_O)
tgunzip1: tgunzip1.o gunzip.o inflate.o $(MYSTL_O)
touch: touch.o
tr: tr.o $(MYSTL_O)
tstdio1: tstdio1.o $(MYSTDIO_O)
unix2dos: unix2dos.o $(MYSTL_O)
uuidgen: uuidgen.o $(MYSTL_O)
weekday: weekday.o $(MYSTL_O)
wingroup: wingroup.o $(MYSTL_O)
yes: yes.o fector.o $(MYSTL_O)
zcat: zcat.o gunzip.o inflate.o $(MYSTL_O)
base64.o: base64.cpp $(COMMON_H)
bitinput.o: bitinput.cpp $(BITINPUT_H)
bunzip2.o: bunzip2.cpp $(BUNZIP2_H)
bzcat.o: bzcat.cpp $(BUNZIP2_H)
bzinfo.o: bzinfo.cpp fector.h $(BITINPUT_H)
bzip2.o: bzip2.cpp
bzmd5.o: bzmd5.cpp $(BUFTEST_H)
cat.o: cat.cpp $(COMMON_H)
cp.o: cp.cpp
tcpcom01.o: tcpcom01.cpp
tcpcom02.o: tcpcom02.cpp
crc32.o: crc32.cpp
date.o: date.cpp $(COMMON_H)
dd.o: dd.cpp $(COMMON_H)
diff.o: diff.cpp $(COMMON_H)
dos2unix.o: dos2unix.cpp
fector.o: fector.cpp
filesys.o: filesys.cpp filesys.h
grep.o: grep.cpp
gunzip.o: gunzip.cpp $(GUNZIP_H)
gunzipm.o: gunzipm.cpp $(GUNZIP_H)
gzip.o: gzip.cpp
hasher.o: hasher.cpp hasher.h
inflate.o: inflate.cpp $(INFLATE_H)
jpg2tga.o: jpg2tga.cpp $(COMMON_H)
kompakt.o: kompakt.cpp kompakt.h $(COMMON_H)
ls.o: ls.cpp
main.o: main.cpp
md5sum.o: md5sum.cpp $(HASHER_H)
mystdio.o: mystdio.cpp $(MYSTDIO_H)
mystl.o: mystl.cpp $(MYSTL_H)
mytime.o: mytime.cpp
nl.o: nl.cpp $(COMMON_H)
od.o: od.cpp od.h $(COMMON_H)
odmain.o: odmain.cpp $(OD_H)
rm.o: rm.cpp
tar.o: tar.cpp $(TAR_H)
tarm.o: tarm.cpp $(TAR_H)
tcpcom03.o: tcpcom03.cpp $(COMMON_H)
tcpcom04.o: tcpcom04.cpp $(COMMON_H)
tcpcom05.o: tcpcom05.cpp $(COMMON_H)
tcpcom06.o: tcpcom06.cpp $(COMMON_H)
tcpcom07.o: tcpcom07.cpp $(COMMON_H)
tcpcom08.o: tcpcom08.cpp $(COMMON_H)
tcpcom09.o: tcpcom09.cpp $(COMMON_H)
tcpcom10.o: tcpcom10.cpp $(COMMON_H)
tcpref01.o: tcpref01.cpp
tee.o: tee.cpp $(COMMON_H)
test1.o: test1.cpp
test2.o: test2.cpp
test3.o: test3.cpp
testbinp.o: testbinp.cpp $(BITINPUT_H)
teststl1.o: teststl1.cpp $(MYSTL_H)
tgmtime1.o: tgmtime1.cpp $(COMMON_H)
tgunzip1.o: tgunzip1.cpp $(GUNZIP_H)
touch.o: touch.cpp
tr.o: tr.cpp $(COMMON_H)
tstdio1.o: tstdio1.cpp $(COMMON_H)
unix2dos.o: unix2dos.cpp
util2.o: util2.cpp $(UTIL2_H)
uuidgen.o: uuidgen.cpp $(COMMON_H)
weekday.o: weekday.cpp $(COMMON_H)
wingroup.o: wingroup.cpp $(COMMON_H)
yes.o: yes.cpp fector.h $(COMMON_H)
zcat.o: zcat.cpp $(GUNZIP_H)

download:
	$(VALGRIND) wget -i download.get

test1go: test1
	$(VALGRIND) ./test1

tgunzip1go: tgunzip1
	$(VALGRIND) ./tgunzip1 > /dev/null

testgunzip2:
	rm -f znew.txt
	$(VALGRIND) ./gunzip znew.gz znew.txt
	$(VALGRIND) ./md5sum -c znew.md5
	rm -f znew.txt

# moet een andere file worden, want dinges.tar zit niet in git!
testcp:
	rm -f dinges2.tar
	$(VALGRIND) ./cp dinges.tar dinges2.tar
	$(VALGRIND) ./cat dinges2.tar | ./md5sum -x cffd664a74cbbd3d9f6877668c42fa03

testbzcat:
	$(VALGRIND) ./bzcat battery.bz2 | ./md5sum -x efc57edfaf907b5707878f544b39d6d5
	$(VALGRIND) ./bzcat dinges.tar.bz2 | ./md5sum -x cffd664a74cbbd3d9f6877668c42fa03

testzcat:
	$(VALGRIND) ./zcat znew.gz | ./md5sum -x 742b0b4d1543c6df46e35b77ec6baa53
	$(VALGRIND) ./zcat octave-3.8.2.tar.gz | ./md5sum -x 1da9f884aefbf5191243ed64286695e9
	$(VALGRIND) ./zcat mc-4.6.1.tar.gz | ./md5sum -x b11aac7c755bc5b4ad2cec64d6274d6d

testkompakt:
	$(VALGRIND) bzcat battery.bz2 | ./kompakt -l -s | ./diff -s kompakt1.out -

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
	$(VALGRIND) ./bunzip2 dinges.tar.bz2 dinges.tar

testnl:
	$(VALGRIND) ./cat tr.cpp | ./nl | ./diff nl.out -

testcrc32:
	$(VALGRIND) ./crc32 diff.cpp

testjpg2tga:
	$(VALGRIND) ./jpg2tga whouse.jpg whouse.tga

testgmtime1:
	$(VALGRIND) ./tgmtime1

testgrep:
	$(VALGRIND) ./grep include Makefile | ./diff -s grep1.out -

testcppcom01:
	$(VALGRIND) ./tcpcom01 | ./diff -s tcpcom01.txt -

testcppcom02:
	$(VALGRIND) ./tcpcom02

testcppcom03:
	$(VALGRIND) ./tcpcom03 | ./diff -s tcpcom03.txt -

testcppcom04:
	$(VALGRIND) ./tcpcom04 | ./diff -s tcpcom04.txt -

testcppcom05:
	$(VALGRIND) ./tcpcom05 | ./diff -s tcpcom05.txt -

testcppcom06:
	$(VALGRIND) ./tcpcom06 | ./diff -s tcpcom06.txt -

testcppcom07:
	$(VALGRIND) ./tcpcom07 | ./diff -s tcpcom07.txt -

testcppcom10:
	$(VALGRIND) ./tcpcom10 | ./diff -s tcpcom10.txt -

testcppref01:
	$(VALGRIND) ./tcpref01 | ./diff -s tcpref01.txt -

test2go:
	$(VALGRIND) ./test2

tests1: testkompakt
tests2: testcp testjpg2tga testtar
tests3: testgmtime1 tgunzip1go testbunzip2 testmd5sum testgunzip2
tests4: testcppcom01 testcppcom03 testcppcom04 testcppcom05 testcppcom06 testcppcom07
tests5: testcppcom10
tests6: testcppref01
tests7: testod test1go testgrep testnl testzcat testbzcat testcrc32 testbase64 teststl1go
test: tests1 tests2 tests3 tests4 tests5 tests6 tests7

clean:
	rm -Rf *.o jpg2tga *.tga $(TARGETS)
	rm -Rf battery.iso

rebuild: clean all


