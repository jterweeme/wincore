set watcom=c:\tools\watcom
wpp -I\tools\watcom\h main.cpp
wpp -I\tools\watcom\h mystl.cpp
wpp -I\tools\watcom\h hasher.cpp
wlink @md5sum.lnk


