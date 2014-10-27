#!/bin/bash

stat $1 &> /dev/null

if [ $? != 0 ]
then
echo -e "\033[1;33mSkipping $1 \033[0m"
exit 0
fi

rm -Rf $2
valgrind -q --error-exitcode=1 --leak-check=full ./kompakt -x -f $1 -o $2 > /dev/null
md5sum -c $3 > /dev/null

if [ $? != 0 ]
then
echo -e "\033[1;31mTest $1 failed \033[0m"
rm -Rf $2
exit 0
fi

rm -Rf $2
echo -e "\033[1;32mTest $1 passed \033[0m"

