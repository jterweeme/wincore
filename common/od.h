#ifndef _OD_H_
#define _OD_H_

#include "common.h"

class HexStream
{
public:
    void dump(istream &input);
};

class OptionsOd
{
};

class AppOd
{
public:
    int run(int argc, char **argv);
};

#endif


