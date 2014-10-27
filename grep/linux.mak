all:
	g++ -g -std=c++11 -o grep main.cpp
	g++ -g -std=c++11 -o test1 test1.cpp

test:

test1: force
	./grep alpha < test.txt > test1.tst
	diff test1.tst test1.out

force:

clean:
	rm -Rvf test1.tst grep test?

