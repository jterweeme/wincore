set watcom=c:\tools\watcom
wpp -I\tools\watcom\h kompakt.cpp
wpp -I\tools\watcom\h main.cpp
wpp -I\tools\watcom\h mystl.cpp
wlink @kompakt.lnk


