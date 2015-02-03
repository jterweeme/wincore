#ifndef _BITINPUT_H_
#define _BITINPUT_H_
#include <iostream>
#include <stdint.h>
using namespace std;

class BitInput
{
    istream *_is;
    uint32_t _bitBuffer = 0, _bitCount = 0;
public:
    BitInput(istream *is) : _is(is) { }
    uint32_t readBits(uint32_t count);
    bool readBool() { return readBits(1); }
    uint32_t readUnary() { uint32_t u = 0; while (readBool()) u++; return u; }
    uint32_t readUInt32() { return readBits(16) << 16 | readBits(16); }
    uint32_t readInt() { return readUInt32(); }
    void ignore(uint32_t n) { while (n--) readBool(); }
};


#endif



