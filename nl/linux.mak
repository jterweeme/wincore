VALGRIND = -q --error-exitcode=1 --leak-check=full

all:
	g++ -g -Wall -o nl main.cpp

test: force
	valgrind $(VALGRIND) ./test1

force:

clean:
	@rm -Rf *.o *.obj nl main

