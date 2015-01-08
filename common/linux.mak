CXXFLAGS = -Wall -Wno-parentheses --std=c++11 -O3
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = #valgrind $(VALFLAGS)

.PHONY: all

all: mystl.o crc32 grep
	g++ $(CXXFLAGS) -o md5s md5s.cpp mystl.o hasher.cpp
	g++ $(CXXFLAGS) -o test1 test1.cpp mystl.o hasher.cpp
	g++ $(CXXFLAGS) -o jpg2tga jpg2tga.cpp -lm
	g++ $(CXXFLAGS) -o uuidgen uuidgen.cpp
	g++ $(CXXFLAGS) -o od od.cpp mystl.o odmain.cpp
	g++ $(CXXFLAGS) -o base64 base64.cpp
	g++ $(CXXFLAGS) -o yes yes.cpp
	g++ $(CXXFLAGS) -o tr tr.cpp
	g++ $(CXXFLAGS) -o cat cat.cpp
	g++ $(CXXFLAGS) -o nl nl.cpp

crc32: crc32.cpp
	g++ $(CXXFLAGS) -o $@ $<

mystl.o: mystl.cpp
	g++ $(CXXFLAGS) -c $<

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

rebuild: clean all


