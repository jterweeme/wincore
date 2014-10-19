set watcom=c:\tools\watcom
wpp -xs -xr -I\tools\watcom\h kompakt.cpp
wpp -xs -xr -I\tools\watcom\h main.cpp
wpp -xs -xr -I\tools\watcom\h mystl.cpp
nasm routine.asm -f obj
wpp -xs -xr -I\tools\watcom\h test6.cpp
wlink @kompakt.lnk
wlink @test6.lnk

