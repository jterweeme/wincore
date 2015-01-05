set watcom=c:\tools\watcom
wpp -xs -I\tools\watcom\h main.cpp
wpp -xs -I\tools\watcom\h mystl.cpp
wpp -xs -I\tools\watcom\h hasher.cpp
wlink @md5sum.lnk


