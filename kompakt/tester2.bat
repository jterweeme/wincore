bzcat %1.bz2 > %1.iso
kompakt -x -f %1.iso -o %1 >NUL
md5sum -c %1.md5
deltree /y %1



