all: base64

base64: main.cpp
	g++ -o $@ main.cpp

test: base64
	./base64 test1.in | diff - test1.out
	./base64 test2.in | diff - test2.out
	./base64 test3.in | diff - test3.out
	./base64 test4.in | diff - test4.out
	./base64 test5.in | diff - test5.out

clean:
	rm -Rvf base64

