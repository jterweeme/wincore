CXXFLAGS = -Wall -Wno-parentheses --std=c++11 -O3
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = #valgrind $(VALFLAGS)

%.o: %.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

.PHONY: all

all: mystl.o kompakt nl crc32 grep
	g++ $(CXXFLAGS) -o md5s md5s.cpp mystl.o hasher.cpp
	g++ $(CXXFLAGS) -o test1 test1.cpp mystl.o hasher.cpp
	g++ $(CXXFLAGS) -o jpg2tga jpg2tga.cpp -lm
	g++ $(CXXFLAGS) -o uuidgen uuidgen.cpp
	g++ $(CXXFLAGS) -o od od.cpp mystl.o odmain.cpp
	g++ $(CXXFLAGS) -o base64 base64.cpp
	g++ $(CXXFLAGS) -o yes yes.cpp
	g++ $(CXXFLAGS) -o tr tr.cpp
	g++ $(CXXFLAGS) -o cat cat.cpp

crc32: crc32.cpp
	g++ $(CXXFLAGS) -o $@ $<

kompakt: kompakt.o main.o mystl.o filesys.o
	g++ -o $@ $^

nl: nl.o
	g++ -o $@ $^

filesys.o: filesys.cpp filesys.h
kompakt.o: kompakt.cpp kompakt.h common.h mystl.h mystl.tcc
main.o: main.cpp
mystl.o: mystl.cpp
nl.o: nl.cpp

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
	rm -Rvf kompakt

rebuild: clean all


