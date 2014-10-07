/*
Jasper ter Weeme
*/

#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stdio.h>

/* tailored only for the od app */


typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;

class iomanip2
{
    int _type;
public:
    static const uint8_t HEX = 1;
    static const uint8_t DEC = 2;
    static const uint8_t OCT = 3;
    iomanip2() { }
    iomanip2(int type) : _type(type) { }
    int type() const { return _type; }
};

class dummy
{
};

class ostream2
{
    FILE *_fp;
    iomanip2 _manip;
public:
    ostream2(FILE *fp) : _fp(fp) { }
    ostream2& operator << (iomanip2 &i) { _manip = i; return *this; }
    ostream2& operator << (const char *s) { fprintf(_fp, s); return *this; }
    ostream2& operator << (char c) { fputc(c, _fp);  return *this; }
    ostream2& operator << (dummy &i) { return *this; }
    ostream2& operator << (int s)
    {
        switch (_manip.type())
        {
        case iomanip2::DEC:
            fprintf(_fp, "%d", s);
            break;
        case iomanip2::HEX:
            fprintf(_fp, "%.2x", s);
            break;
        case iomanip2::OCT:
            fprintf(_fp, "%07o", s);
            break;
        }
        return *this;
    }

};

class istream2
{
    FILE *_fp;
    size_t _lastRead;
    bool _eof;
public:
    istream2(FILE *fp) : _fp(fp), _lastRead(0), _eof(false) { }
    ~istream2() { fclose(_fp); }
    uint16_t tellg() { return ftell(_fp); }
    uint16_t gcount() { return _lastRead; }
    operator void * () const { return (void *)!_eof; }
    virtual void read(char *s, size_t length)
    {
        _lastRead = fread(s, 1, length, _fp);
        _eof = _lastRead < length;
    }
};

class ifstream2 : public istream2
{
public:
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    ifstream2(char *s, int m) : istream2(fopen(s, "rb")) { }
};

namespace mystl
{



    using ::uint8_t;
    using ::uint16_t;
    typedef iomanip2 iomanip;
    typedef ostream2 ostream;
    typedef istream2 istream;
    typedef ifstream2 ifstream;
    istream cin(stdin);
    ostream cout(stdout);
    ostream cerr(stderr);
    iomanip hex(iomanip::HEX);
    iomanip oct(iomanip::OCT);
    dummy dummy1;
    dummy& setw(int length) { return dummy1; }
    dummy& setfill(char c) { return dummy1; }

}

#endif


