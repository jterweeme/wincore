#!/bin/bash

color()
{
if [ $? == 0 ]
then
    echo -e "\033[1;32mTest passed\033[0m"
else
    echo -e "\033[1;31mTest failed\033[0m"
fi
    
}

./grep alpha < test.txt | diff test1.out -
color
./grep bravo < test.txt | diff test2.out -
color
./grep alpha test.txt | diff test1.out -
color
./grep bravo test.txt | diff test2.out -
color


