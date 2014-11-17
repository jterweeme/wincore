all:
	g++ -o dos2unix dos2unix.cpp mystl.cpp

clean:
	@rm -Rvf dos2unix *.o

