CXXFLAGS = -Wall -Wno-parentheses --std=c++11 -O3
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = #valgrind $(VALFLAGS)

.PHONY: all

all:
	g++ $(CXXFLAGS) -o md5s md5s.cpp mystl.cpp hasher.cpp
	g++ $(CXXFLAGS) -o test1 test1.cpp mystl.cpp hasher.cpp
	g++ $(CXXFLAGS) -o jpg2tga jpg2tga.cpp -lm
	g++ $(CXXFLAGS) -o uuidgen uuidgen.cpp
	g++ $(CXXFLAGS) -o od od.cpp mystl.cpp odmain.cpp
	g++ $(CXXFLAGS) -o base64 base64.cpp


test:
	$(VALGRIND) ./test1
	$(VALGRIND) ./md5s -c data.md5
	$(VALGRIND) ./md5s zero.dat | diff md5s.od -
	$(VALGRIND) ./jpg2tga whouse.jpg whouse.tga
	$(VALGRIND) ./od zero.dat | diff zero.od -
	$(VALGRIND) ./base64 zero.dat | diff zero.b64 -

clean:
	rm -Rvf *.o jpg2tga *.tga od md5s test1 uuidgen

rebuild: clean all


