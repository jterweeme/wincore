CXXFLAGS = -g -Wall -Wno-parentheses --std=c++11
MYSTL_H = mystl.h mystl.tcc
COMMON_H = common.h $(MYSTL_H)
BITINPUT_H = bitinput.h $(COMMON_H)
BUNZIP2_H = bunzip2.h fector.h $(BITINPUT_H)
MYSTL_O = mystl.o mytime.o util2.o
GUNZIP_O = gunzip.o bitinput.o inflate.o $(MYSTL_O)

TARGETS = base64 bunzip2 bzcat bzinfo cat cp crc32 date dd diff \
    dos2unix grep gunzip \
    jpg2tga kompakt md5sum nl od rm tar \
    tcpcom01 tcpcom02 \
    tcpcom03 tcpcom04 tcpcom05 tcpcom06 tcpcom07 tcpcom08 tcpcom09 tcpcom10 tcpcom11 \
    tcpref01 \
    tee test1 test2 test3 test4 testbinp \
    teststl1 tgmtime1 tgunzip1 touch tr unix2dos uuidgen weekday wingroup yes zcat

%.o: %.cpp
	@gpp $(CXXFLAGS) -c -o $@ $<

%: %.o
	@gpp -o $@ $^

.PHONY: all

all: $(TARGETS)
base64: base64.o $(MYSTL_O)
bunzip2: bunzip2m.o bunzip2.o bitinput.o fector.o $(MYSTL_O)
bzcat: bzcat.o bitinput.o bunzip2.o fector.o $(MYSTL_O)
bzinfo: bzinfo.o bitinput.o $(MYSTL_O)
bzip2: bzip2.o $(MYSTL_O)
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
gunzip: gunzipm.o $(GUNZIP_O)
gzip: gzip.o
jpg2tga: jpg2tga.o $(MYSTL_O)
kompakt: kompakt.o main.o filesys.o $(MYSTL_O)
ls: ls.o
md5sum: md5sum.o hasher.o $(MYSTL_O)
nl: nl.o $(MYSTL_O)
od: od.o odmain.o $(MYSTL_O)
rm: rm.o $(MYSTL_O)
tar: tarm.o tar.o bitinput.o bunzip2.o gunzip.o inflate.o fector.o $(MYSTL_O)
tcpcom03: tcpcom03.o $(MYSTL_O)
tcpcom04: tcpcom04.o $(MYSTL_O)
tcpcom05: tcpcom05.o $(MYSTL_O)
tcpcom06: tcpcom06.o $(MYSTL_O)
tcpcom07: tcpcom07.o $(MYSTL_O)
tcpcom08: tcpcom08.o $(MYSTL_O)
tcpcom09: tcpcom09.o $(MYSTL_O)
tcpcom10: tcpcom10.o $(MYSTL_O)
tcpcom11: tcpcom11.o $(MYSTL_O)
tcpref01: tcpref01.o util2.o $(MYSTL_O)
tee: tee.o $(MYSTL_O)
test1: test1.o hasher.o $(MYSTL_O)
test2: test2.o $(GUNZIP_O)
test3: test3.o bunzip2.o fector.o bitinput.o $(MYSTL_O)
test4: test4.o $(MYSTL_O)
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
zcat: zcat.o $(GUNZIP_O)
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
inflate.o: inflate.cpp inflate.h
jpg2tga.o: jpg2tga.cpp
kompakt.o: kompakt.cpp kompakt.h $(COMMON_H)
ls.o: ls.cpp
main.o: main.cpp
md5sum.o: md5sum.cpp
mystdio.o: mystdio.cpp mystdio.h
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
tcpcom11.o: tcpcom11.cpp $(COMMON_H)
tcpref01.o: tcpref01.cpp
tee.o: tee.cpp $(COMMON_H)
test1.o: test1.cpp
test2.o: test2.cpp
test3.o: test3.cpp
test4.o: test4.cpp
testbinp.o: testbinp.cpp
teststl1.o: teststl1.cpp $(MYSTL_H)
tgmtime1.o: tgmtime1.cpp $(COMMON_H)
tgunzip1.o: tgunzip1.cpp
touch.o: touch.cpp
tr.o: tr.cpp $(COMMON_H)
tstdio1.o: tstdio1.cpp
unix2dos.o: unix2dos.cpp
util2.o: util2.cpp
uuidgen.o: uuidgen.cpp
weekday.o: weekday.cpp
wingroup.o: wingroup.cpp $(COMMON_H)
yes.o: yes.cpp fector.h $(COMMON_H)
zcat.o: zcat.cpp $(GUNZIP_H)

test1go: test1
	test1

tgunzip1go:
	tgunzip1

testgunzip2:
	rm -f znew.txt
	gunzip znew.gz znew.txt
	md5sum -c znew.md5
	rm -f znew.txt

testcp:
	rm -f whouse2.jpg
	cp whouse.jpg whouse2.jpg
	cat whouse2.jpg | md5sum -x a264902120c099dfb0c6539b977de2f5

testbzcat:
	bzcat battery.bz2 | md5sum -x efc57edfaf907b5707878f544b39d6d5

testzcat:
	zcat znew.gz | md5sum -x 742b0b4d1543c6df46e35b77ec6baa53

testkompakt:
	bzcat battery.bz2 | kompakt -l -s | diff -s kompakt1.out -

testod:
	od -t x1z zero.dat | ./diff -s zero.od -
	od -t x1z circles.bmp | md5sum -x 40d8c800dd99de6e80e166c37f683e46
	od -t x1z minimal.gif | md5sum -x 5c4171a8b5cbf3076be875e3af7ce610

testbase64:
	base64 zero.dat | diff -s zero.b64 -

tststl1go:
	teststl1

testtar:
	tar -tvf dinges.tar | diff -s dinges.out -

testmd5sum:
	md5sum -c data.md5

testnl:
	cat tr.cpp | nl | diff -s nl.out -

testcrc32:
	crc32 diff.cpp

testjpg2tga:

testgmtime1:
	tgmtime1

tstbinpgo:
	testbinp

testgrep:
	grep include Makefile | diff -s grep2.out -

testcppcom01:
	tcpcom01 | diff -s tcpcom01.txt

testcppcom02:
	tcpcom02

testcppcom03:
	tcpcom03 | diff -s tcpcom03.txt

testcppcom04:
	tcpcom04 | diff -s tcpcom04.txt

testcppcom05:
	tcpcom05 | diff -s tcpcom05.txt

testcppcom06:
	tcpcom06 | diff -s tcpcom06.txt

testcppcom07:
	tcpcom07 | diff -s tcpcom07.txt

testcppcom10:
	tcpcom10 | diff -s tcpcom10.txt

testcppref01:
	tcpref01 | diff -s tcpref01.txt

test2go:
	test2

testgzip:
	rm -vf bzip2.gz

testbzip2:
	rm -vf gzip.bz2 whouse.bz2

tests1: testnl
tests2: tstbinpgo
tests3: testmd5sum
tests4: testcppcom01 testcppcom03 testcppcom04 testcppcom05 testcppcom06 testcppcom07
tests5: testcppref01
tests6: testod test1go testgrep testcrc32 testbase64
tests7: tststl1go testgmtime1 testcppcom10
test: tests1 tests2 tests3 tests4 tests5 tests6 tests7

clean:
	deltree /y *.obj *.o *.exe

rebuild: clean all

