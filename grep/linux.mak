all:
	g++ -g -std=c++11 -o grep main.cpp
	g++ -g -std=c++11 -o test1 test1.cpp

test: test1

test1: force
	./tester1.sh


force:

clean:
	rm -Rvf test1.tst grep test?

