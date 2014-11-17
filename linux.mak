all:
	@make -C base64 -f linux.mak all
	@make -C cat -f linux.mak all
	@make -C dd -f linux.mak all
	@make -C dos2unix -f linux.mak all
	@make -C grep -f linux.mak all
	@make -C kompakt -f linux.mak all
	@make -C ls -f linux.mak all
	@make -C md5sum -f linux.mak all
	@make -C nl -f linux.mak all
	@make -C od -f linux.mak all
	@make -C rm -f linux.mak all
	@make -C touch -f linux.mak all
	@make -C uuidgen -f linux.mak all
	@make -C vi -f linux.mak all

test:
	@make -C base64 -f linux.mak test
	@make -C cat -f linux.mak test
	@make -C dd -f linux.mak test
	@make -C dos2unix -f linux.mak test
	@make -C grep -f linux.mak test
	@make -C kompakt -f linux.mak test
	@make -C ls -f linux.mak test
	@make -C md5sum -f linux.mak test
	@make -C nl -f linux.mak test
	@make -C od -f linux.mak test
	@make -C rm -f linux.mak test
	@make -C touch -f linux.mak test
	@make -C uuidgen -f linux.mak test
	@make -C vi -f linux.mak test

clean:
	@make -C base64 -f linux.mak clean
	@make -C cat -f linux.mak clean
	@make -C dd -f linux.mak clean
	@make -C dos2unix -f linux.mak clean
	@make -C doscore clean
	@make -C grep -f linux.mak clean
	@make -C kompakt -f linux.mak clean
	@make -C ls -f linux.mak clean
	@make -C md5sum -f linux.mak clean
	@make -C nl -f linux.mak clean
	@make -C od -f linux.mak clean
	@make -C rm -f linux.mak clean
	@make -C touch -f linux.mak clean
	@make -C uuidgen -f linux.mak clean
	@make -C vi -f linux.mak clean


