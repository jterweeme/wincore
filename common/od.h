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
    bool _cin = false;
    string _fn;
public:
    void parse(int argc, char **argv);
    bool cin() const { return _cin; }
    string fn() const { return _fn; }
};

class AppOd
{
public:
    int run(int argc, char **argv);
};

#endif


