#ifndef _BZIP2_H_
#define _BZIP2_H_
#include "bitinput.h"
#include "fector.h"
#include <vector>

class MoveToFront : public Fugt
{
public:
    MoveToFront() : Fugt(256) { for (uint32_t i = 0; i < 256; i++) set(i, i); }
    uint8_t indexToFront(uint32_t index);
};

class Table
{
    Fugt _codeLengths;
    uint16_t _pos = 0;
    const uint32_t _symbolCount;
    uint32_t _bases[25] = {0};
    int32_t _limits[24] = {0};
    uint32_t _symbols[258] = {0};
    uint8_t _minLength(uint32_t n) { return _codeLengths.min(n); }
    uint8_t _maxLength(uint32_t n) { return _codeLengths.max(n); }
public:
    Table(uint32_t symbolCount) : _codeLengths(258), _symbolCount(symbolCount) { }
    void calc();
    uint8_t maxLength() { return _maxLength(_symbolCount + 2); }
    uint8_t minLength() { return _minLength(_symbolCount + 2); }
    void add(uint8_t v) { _codeLengths.set(_pos++, v); }
    int32_t limit(uint8_t i) const { return _limits[i]; }
    uint32_t symbol(uint16_t i) const { return _symbols[i]; }
    uint32_t base(uint8_t i) const { return _bases[i]; }
    void dump(ostream &os) const;
    string toString() const { ostringstream o; dump(o); return o.str(); }
};

class Tables : public vector<Table>
{
public:
    Tables() : vector<Table>() { }
    void dump(ostream &os) const;
    string toString() const { ostringstream o; dump(o); return o.str(); }
};

class Block
{
    int32_t *_merged;
    int32_t _curTbl, _grpIdx, _grpPos, _last, _acc, _repeat, _curp, _length, _dec;
    uint32_t _nextByte() { int r = _curp & 0xff; _curp = _merged[_curp >> 8]; _dec++; return r; }
    uint32_t _nextSymbol(BitInput *bi, const Tables &t, const Fugt &selectors);
public:
    void reset();
    Block() { reset(); }
    ~Block() { delete[] _merged; }
    int read();
    void init(BitInput *bi);
};

class DecStream
{
    BitInput *_bi;
    Block _bd;
    uint32_t _streamComplete = false;
    bool _initNextBlock();
public:
    int read();
    int read(char *buf, int n);
    DecStream(BitInput *bi) : _bi(bi) { _bi->ignore(32); }
    void extractTo(mystl::ostream &os) { for (int b = read(); b != -1; b = read()) os.put(b); }
};

class DecStreamBuf : public streambuf
{
    DecStream _ds;
    const uint32_t _put_back = 8;
    vector<char> _buffer;
    uint32_t _pos = 0;
public:
    DecStreamBuf(BitInput *bi) : _ds(bi), _buffer(264) { }
    DecStreamBuf(FILE *fp) : _ds(new BitInputFile(fp)) { }
    //fpos2<mbstate_t> seekoff(int64_t off, ios::seekdir way, ios::openmode m);
    int underflow();
};

#endif



