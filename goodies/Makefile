all:
	javac Bunzip2.java

clean:
	rm -Rvf *.class

test:
	rm -f battery boston.mp3 dinges aquarius.mp3
	java Bunzip2 battery.bz2
	java Bunzip2 boston.mp3.bz2
	java Bunzip2 dinges.bz2
	java Bunzip2 aquarius.mp3.bz2
	md5sum -c bunzip.md5

rebuild: clean all


