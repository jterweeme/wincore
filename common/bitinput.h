#ifndef _BITINPUT_H_
#define _BITINPUT_H_
#include "common.h"

class BitInputBase
{
public:
    virtual uint32_t readBit() = 0;
    virtual uint32_t readBits(uint32_t n) = 0;
    bool readBool() { return readBit() == 1; }
};

/*
BitInput2 leest van elke byte least-significant bit first
*/
class BitInput2 : public BitInputBase
{
protected:
    int _nextBits, _bitPos = 8;
    virtual int _getc() = 0;
    uint32_t _readBit();
public:
    uint8_t revByte(uint8_t v)
    {
        uint8_t ret = 0;

        for (int i = 0; i < 8; i++)
            ret |= ((v >> i) & 1) << (7 - i);
        
        return ret;
    }
    uint32_t readBit() { return _readBit(); }
    uint32_t readBits(uint32_t n);
    void ignore(int n) { while (n--) readBool(); }
    uint8_t readByte() { return readBits(8); }
    void ignoreBytes(int n) { while (n--) readByte(); }
    void read(uint8_t *s, int n) { for (int i = 0; i < n; i++) s[i] = readByte(); }
    uint32_t readUnary() { uint32_t u = 0; while (readBool()) u++; return u; }
    uint32_t readUInt32() { return readBits(16) << 16 | readBits(16); }
    uint32_t readInt() { return readUInt32(); }
};

class BitInput2Stream : public BitInput2
{
protected:
    istream *_is;
    int _getc() { return _is->get(); }
public:
    BitInput2Stream(istream *is) : _is(is) { }
};

class BitInput2File : public BitInput2
{
    FILE *_fp;
public:
    BitInput2File() { }
    BitInput2File(FILE *fp) : _fp(fp) { }
    void open(FILE *fp) { _fp = fp; }
protected:
    int _getc() { return fgetc(_fp); }
};

class BitInput : public BitInputBase
{
protected:
    uint32_t _bitBuffer = 0, _bitCount = 0;
    virtual int _getc() = 0;
public:
    uint32_t readBits(uint32_t count);
    uint32_t readBit() { return readBits(1); }
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

#endif



