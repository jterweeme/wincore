#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stdio.h>
#include <iostream>
#include <iomanip>


namespace mystl
{

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

class iomanip
{
    int _type;
public:
    static const uint8_t HEX = 1;
    static const uint8_t DEC = 2;
    static const uint8_t OCT = 3;
    iomanip() { }
    iomanip(int type) : _type(type) { }
    int type() const { return _type; }
};

class ostream
{
    FILE *_fp;
    iomanip _manip;
public:
    ostream(FILE *fp) : _fp(fp) { }
    ostream& operator << (iomanip &i) { _manip = i; return *this; }
    ostream& operator << (const char *s) { fprintf(_fp, s); return *this; }
};

class istream
{
    FILE *_fp;
    size_t _lastRead;
    bool _eof;
public:
    istream(FILE *fp) : _fp(fp), _lastRead(0), _eof(false) { }
    ~istream() { fclose(_fp); }
    uint16_t tellg() { return ftell(_fp); }
    uint16_t gcount() { return _lastRead; }
    operator void * () const { return (void *)!_eof; }
    virtual void read(char *s, size_t length)
    {
        _lastRead = fread(s, 1, length, _fp);
        _eof = _lastRead < length;
    }
};

class ifstream : public istream
{
public:
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    ifstream(char *s, int m) : istream(fopen(s, "rb")) { }
};

istream cin(stdin);
ostream cout(stdout);
ostream cerr(stderr);
iomanip hex(iomanip::HEX);
iomanip oct(iomanip::OCT);

iomanip setw(int length) { return hex; }
iomanip setfill(char c) { return hex; }

}

#endif


