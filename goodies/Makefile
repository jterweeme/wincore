all:
	javac *.java

clean:
	@rm -Rvf *.class *.mp3

test:
	rm -f battery boston.mp3 dinges aquarius.mp3
	java Bunzip2 battery.bz2
	java Bunzip2 boston.mp3.bz2
	java Bunzip2 dinges.bz2
	java Bunzip2 aquarius.mp3.bz2
	md5sum -c bunzip.md5

testgz:
	@rm -f battery boston.mp3 dinges aquarius.mp3 cdrtools.tar znew
	java Gunzip boston.mp3.gz boston.mp3
	java Gunzip cdrtools.tar.gz cdrtools.tar
	java Gunzip znew.gz znew
	md5sum -c data.md5

rebuild: clean all

testg: rebuild testgz


