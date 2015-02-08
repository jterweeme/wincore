#ifndef _BZIP2_H_
#define _BZIP2_H_
#include "bitinput.h"
#include "fector.h"
#include <vector>
#include <cstring>

class Table : public Fugt
{
public:
    Table() : Fugt(256) { for (uint32_t i = 0; i < 256; i++) set(i, i); }

    uint8_t indexToFront(uint32_t index)
    {
        uint8_t value = at(index);
        for (uint32_t i = index; i > 0; i--) set(i, at(i - 1));
        return set(0, value);
    }
};

class Block
{
    int32_t _minLengths[6] = {0}, _bwtByteCounts[256] = {0}, *_merged;
    int32_t _bases[6][25] = {}, _symbols[6][258] = {};
    int32_t _limits[6][24] = {};
    int32_t _curTbl, _grpIdx, _grpPos, _last, _acc, _rleRepeat = 0, _randomIndex, _randomCount;
    bool _blockRandomised = false;
    uint8_t _symbolMap[256] = {0};
    int32_t _curp, _length = 0, _dec = 0;
    uint32_t _nextByte() { int r = _curp & 0xff; _curp = _merged[_curp >> 8]; _dec++; return r; }
    void _generate(uint8_t *a) { for (unsigned i = 0; i < 256; i++) a[i] = i; }
    void _generate(Fugt &a) { for (uint32_t i = 0; i < 256; i++) a.set(i, i); }
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
public:
    int read();
    int read(char *buf, int n)
    {
        int i = 0;

        for (int c; (c = read()) != -1 && i < n; i++)
            buf[i] = c;

        return i;
    }
    DecStream(BitInput *bi) : _bi(bi) { _bi->ignore(32); }
    void extractTo(ostream &os)
    { for (int b = read(); b != -1; b = read()) os.put(b); os.flush(); }
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

    fpos<mbstate_t> seekoff(int64_t off, ios::seekdir way, ios::openmode m)
    {
        return (uint64_t)_M_in_cur - (uint64_t)_M_in_beg;
    }

    int underflow()
    {
        if (gptr() < egptr())
            return (int)(*gptr());

        char *base = &_buffer.front();
        char *start = base;

        if (eback() == base)
        {
            memmove(base, egptr() - _put_back, _put_back);
            start += _put_back;
        }

        uint32_t n = _ds.read(start, _buffer.size());

        if (n == 0)
            return traits_type::eof();

        setg(base, start, start + n);
        return (int)(*gptr());
    }
};

#endif



