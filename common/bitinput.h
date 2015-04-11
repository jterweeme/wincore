#ifndef _BITINPUT_H_
#define _BITINPUT_H_
#include "common.h"

class BitInput
{
protected:
    uint32_t _bitBuffer = 0, _bitCount = 0;
    virtual int _getc() = 0;
public:
    uint32_t readBits(uint32_t count);
    bool readBool() { return readBits(1); }
    uint32_t readUnary() { uint32_t u = 0; while (readBool()) u++; return u; }
    uint32_t readUInt32() { return readBits(16) << 16 | readBits(16); }
    uint32_t readInt() { return readUInt32(); }
    uint8_t readByte() { return readBits(8); }
    void ignoreBytes(uint32_t n) { while (n--) readByte(); }
    void ignore(uint32_t n) { while (n--) readBool(); }
    void read(uint8_t *s, int n) { for (int i = 0; i < n; i++) s[i] = readByte(); }
};

class BitInputStream : public BitInput
{
    istream *_is;
public:
    BitInputStream(istream *is) : _is(is) { }
protected:
    int _getc() { return _is->get(); }
};

class BitInputStreamRev : public BitInput
{
    istream *_is;

    uint8_t _reverse(uint8_t c)
    {
  int shift;
   unsigned char result = 0;
   for (shift = 0; shift < 8; shift++)
    {
      if (c & (0x01 << shift))
         result |= (0x80 >> shift);
   }
   return result;
    }
public:
    BitInputStreamRev(istream *is) : _is(is) { }
protected:
    int _getc() { return _reverse(_is->get()); }
};

class BitInputFile : public BitInput
{
    FILE *_fp;
public:
    BitInputFile() { }
    BitInputFile(FILE *fp) : _fp(fp) { }
    void open(FILE *fp) { _fp = fp; }
protected:
    int _getc() { return fgetc(_fp); }
};

/*
BitInput2 leest van elke byte least-significant bit first
*/
#if 1
class BitInput2
{
    istream *_is;
    int _nextBits, _bitPos = 8;
    int _getc() { return _is->get(); }
    uint32_t _readBit();
    uint32_t _bitBuffer = 0, _bitCount = 0;
public:
    BitInput2(istream *is) : _is(is) { }
    uint32_t readBits(uint32_t n);
    bool readBool() { return readBits(1) == 1; }
    void ignore(int n) { while (n--) readBool(); }
    uint8_t readByte() { return readBits(8); }
    void ignoreBytes(int n) { while (n--) readByte(); }
    void read(uint8_t *s, int n) { for (int i = 0; i < n; i++) s[i] = readByte(); }

};
#else
typedef BitInputStreamRev BitInput2;
#endif


#endif



