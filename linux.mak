all:
	make -C grep -f linux.mak all
	make -C kompakt -f linux.mak all
	make -C od -f linux.mak all

test:
	make -C grep -f linux.mak test
	make -C kompakt -f linux.mak test
	make -C od -f linux.mak test

clean:
	make -C grep -f linux.mak clean
	make -C kompakt -f linux.mak clean
	make -C od -f linux.mak clean


