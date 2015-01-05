CXXFLAGS = -O2 -Wall -Wno-parentheses

.PHONY: all

all:
	gpp $(CXXFLAGS) -o md5sum main.cpp mystl.cpp hasher.cpp
	gpp $(CXXFLAGS) -o test1 test1.cpp mystl.cpp hasher.cpp


clean:
	deltree /y *.obj *.o *.exe

