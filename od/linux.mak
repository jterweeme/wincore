VALGRIND = -q --error-exitcode=1 --leak-check=full

all:
	g++ -g -Wall -o od main.cpp
	g++ -g -Wall -o test1 test1.cpp

test: force
	valgrind $(VALGRIND) ./test1

force:

clean:
	rm -Rf *.o *.obj od test test1

