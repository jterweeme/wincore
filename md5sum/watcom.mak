CXXFLAGS = -xs -I\tools\watcom\h

.PHONY: all

all:
	wpp $(CXXFLAGS) main.cpp
	wpp $(CXXFLAGS) mystl.cpp
	wpp $(CXXFLAGS) hasher.cpp
	wlink @md5sum.lnk

clean:
	deltree /y *.obj *.o *.exe

