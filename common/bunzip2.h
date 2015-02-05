#ifndef _BZIP2_H_
#define _BZIP2_H_
#include "bitinput.h"
#include "fector.h"
#include <vector>

class Block
{
    int32_t _minLengths[6] = {0}, _bwtByteCounts[256] = {0}, *_merged;
    int32_t _bases[6][25] = {}, _symbols[6][258] = {};
    int32_t _limits[6][24] = {};
    int32_t _curTbl, _grpIdx, _grpPos, _last, _acc, _rleRepeat = 0, _randomIndex, _randomCount;
    bool _blockRandomised = false;
    uint8_t _symbolMap[256] = {0}, _indexToFront(uint8_t *a, uint32_t i);
    int32_t _curp, _length = 0, _dec = 0;
    uint32_t _nextByte() { int r = _curp & 0xff; _curp = _merged[_curp >> 8]; _dec++; return r; }
    void _generate(uint8_t *a) { for (unsigned i = 0; i < 256; i++) a[i] = i; }
    uint32_t _nextSymbol(BitInput *bi, const Fugt &selectors);
    Fugt _bwtBlock2;
public:
    Block() : _bwtBlock2(900000) { }
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
    int _read();
public:
    DecStream(BitInput *bi) : _bi(bi) { _bi->ignore(32); }
    void extractTo(ostream &os)
    { for (int b = _read(); b != -1; b = _read()) os.put(b); os.flush(); }
};

class DecStreamBuf : public streambuf
{
    FILE *_fp;
public:
    DecStreamBuf(FILE *fp) : _fp(fp) { }

    fpos<mbstate_t> seekoff(int64_t off, ios::seekdir way, ios::openmode m)
    {
    }

    int underflow()
    {
        
    }
};

#endif



