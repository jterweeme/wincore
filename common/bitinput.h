#ifndef _BITINPUT_H_
#define _BITINPUT_H_
#include "mystl.h"
#include <stdint.h>
#if 0
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <algorithm>
#endif

class BitInput
{
protected:
    uint32_t _bitBuffer = 0, _bitCount = 0;
    virtual int getc() = 0;
public:
    uint32_t readBits(uint32_t count);
    bool readBool() { return readBits(1); }
    uint32_t readUnary() { uint32_t u = 0; while (readBool()) u++; return u; }
    uint32_t readUInt32() { return readBits(16) << 16 | readBits(16); }
    uint32_t readInt() { return readUInt32(); }
    void ignore(uint32_t n) { while (n--) readBool(); }
};

class BitInputStream : public BitInput
{
    mystl::istream *_is;
public:
    BitInputStream(mystl::istream *is) : _is(is) { }
    int getc() { return _is->get(); }
};

class BitInputFile : public BitInput
{
    FILE *_fp;
public:
    BitInputFile(FILE *fp) : _fp(fp) { }
    int getc() { return fgetc(_fp); }
};

#endif



