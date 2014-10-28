/*
Jasper ter Weeme
*/

#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stdio.h>
#include <stdlib.h>

/* tailored only for the od app */

class Util
{
public:
    typedef unsigned char uint8_t;
    typedef unsigned short int uint16_t;   
};

class base2
{
    int _type;
public:
    static const Util::uint8_t HEX = 1;
    static const Util::uint8_t DEC = 2;
    static const Util::uint8_t OCT = 3;
    base2() { }
    base2(int type) : _type(type) { }
    int type() const { return _type; }
};

class dummy
{
};

class ostream2
{
    FILE *_fp;
    base2 _manip;
public:
    ostream2(FILE *fp) : _fp(fp), _manip(base2::DEC) { }
    ostream2& operator << (base2 &i) { _manip = i; return *this; }
    ostream2& operator << (const char *s) { fprintf(_fp, s); return *this; }
    ostream2& operator << (char c) { fputc(c, _fp);  return *this; }
    ostream2& operator << (dummy &i) { return *this; }
    ostream2& operator << (int s)
    {
        switch (_manip.type())
        {
        case base2::HEX:
            fprintf(_fp, "%.2x", s);
            break;
        case base2::OCT:
            fprintf(_fp, "%07o", s);
            break;
        default:
            fprintf(_fp, "%d", s);
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
    Util::uint16_t tellg() { return ftell(_fp); }
    Util::uint16_t gcount() { return _lastRead; }
    operator void * () const { return (void *)!_eof; }
    virtual void read(char *s, size_t length)
    {
        _lastRead = fread(s, 1, length, _fp);
        _eof = _lastRead < length;
    }
};

class ifstream2 : public istream2
{
    typedef Util::uint8_t uint8_t;
public:
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    ifstream2(char *s, int m) : istream2(fopen(s, "rb")) { }
};

template <class T> class vector2
{
    T *_buf;
    size_t _capacity;
    size_t _size;
    int _busy;
public:
    typedef T *iterator;

    vector2() : _capacity(100), _size(0)
    {
        _buf = (T *)calloc(sizeof(T), 100);
        _busy = 45;
    }

    void push_back(T &a) { _buf[_size++] = a; }
    iterator begin()
    {
        return _buf;
    }

    iterator end()
    {
        return _buf + _size;
    }

    virtual ~vector2()
    {
        if (_busy == 45)
        {
            if (_buf)
                free(_buf);
            _busy = 90;
        }
    }
};

namespace mystl
{
    typedef Util::uint8_t uint8_t;
    typedef Util::uint16_t uint16_t;
    typedef ostream2 ostream;
    typedef istream2 istream;
    typedef ifstream2 ifstream;
    template <class T> class vector : public vector2<T> { };
    istream cin(stdin);
    ostream cout(stdout);
    ostream cerr(stderr);
    base2 hex(base2::HEX);
    base2 oct(base2::OCT);
    dummy dummy1;
    dummy& setw(int length) { return dummy1; }
    dummy& setfill(char c) { return dummy1; }

}

#endif


