VALGRIND = -q --error-exitcode=1 --leak-check=full

all:
	g++ -g -Wall -o od main.cpp od.cpp mystl.cpp

test: force
	valgrind $(VALGRIND) ./od watcom.bat | diff watcom.od -

force:

clean:
	@rm -Rf *.o *.obj od test test1

rebuild: clean all


