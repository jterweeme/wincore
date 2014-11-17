CXXFLAGS = -g -Wall -Wno-parentheses
VALGRIND = -q --error-exitcode=1 --leak-check=full

.PHONY: all

all:
	g++ $(CXXFLAGS) -o md5s main.cpp mystl.cpp hasher.cpp
	g++ $(CXXFLAGS) -o test1 test1.cpp mystl.cpp hasher.cpp

test:
	valgrind $(VALGRIND) ./test1

clean:
	@rm -Rvf *.o test? md5s

