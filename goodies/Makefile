all:
	javac Bunzip2.java

clean:
	rm -Rvf *.class

test:
	rm -f battery
	java Bunzip2 battery.bz2
	md5sum battery

rebuild: clean all


