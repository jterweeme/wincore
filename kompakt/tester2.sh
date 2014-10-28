#!/bin/bash

VALGRIND='valgrind -q --error-exitcode=1 --leak-check=full'

color()
{
if [ $? != 0 ]
then
echo -e "\033[1;33mSkipping $1 \033[0m"
exit 0
fi

}

# check if iso image exists
stat $1 &> /dev/null
color

# first remove possibly previous remnants
rm -Rf $2

# check list output
bzcat $1 | $VALGRIND ./kompakt -l -s | diff $4 -
if [ $? != 0 ]
then
echo -e "\033[1;31mTest $1 list failed \033[0m"
rm -Rf $2
exit 0
fi

# check path table
bzcat $1 | $VALGRIND ./kompakt -p -s > /dev/null

# extract file from iso image
bzcat $1 | $VALGRIND ./kompakt -x -s -o $2 > /dev/null

# check the md5 sums
md5sum -c $3 > /dev/null
if [ $? != 0 ]
then
echo -e "\033[1;31mTest $1 failed \033[0m"
rm -Rf $2
exit 0
fi

# remove temp files
rm -Rf $2

# if we made it here the test passed
echo -e "\033[1;32mTest $1 passed \033[0m"

