CXXFLAGS = -g -Wall -Wno-parentheses
VALFLAGS = -q --error-exitcode=1 --leak-check=full
VALGRIND = valgrind $(VALFLAGS)

.PHONY: all

all:
	g++ $(CXXFLAGS) -o md5s md5s.cpp mystl.cpp hasher.cpp
	g++ $(CXXFLAGS) -o test1 test1.cpp mystl.cpp hasher.cpp
	g++ $(CXXFLAGS) -o jpg2tga jpg2tga.cpp -lm
	g++ $(CXXFLAGS) -o uuidgen uuidgen.cpp

test:
	valgrind $(VALFLAGS) ./test1
	valgrind $(VALFLAGS) ./md5s -c data.md5
	$(VALGRIND) ./jpg2tga whouse.jpg whouse.tga

clean:
	rm -Rvf *.o jpg2tga *.tga md5s test1 uuidgen

rebuild: clean all


