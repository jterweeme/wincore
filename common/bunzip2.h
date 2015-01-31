#ifndef _BZIP2_H_
#define _BZIP2_H_
#include "bitinput.h"
#include <vector>
#include <stdint.h>

typedef vector<uint8_t> Vugt;

class Block
{
    uint32_t _minLengths[6] = {0}, _bwtByteCounts[256] = {0}, _merged[999999] = {0};
    uint32_t _bases[6][25], _limits[6][24], _symbols[6][258];
    int _curTbl, _grpIdx, _grpPos, _last, _acc, _rleRepeat, _randomIndex, _randomCount;
    bool _blockRandomised = false;
    uint8_t _bwtBlock[900000] = {0}, _symbolMap[256] = {0}, _indexToFront(uint8_t *a, uint32_t i);
    uint32_t _curp, _length = 0, _dec = 0, _nextByte();

    void _generate(uint8_t *a) { for (unsigned i = 0; i < 256; i++) a[i] = i; }
    uint32_t _nextSymbol(BitInput *bi, Vugt selectors);
public:
    int read();
    void init(BitInput *bi);
};

class DecStream : public ostream
{
    BitInput *_bi;
    Block _bd;
    uint32_t _streamComplete = false;
    bool _initNextBlock();
    int _read();
public:
    DecStream(BitInput *bi) : _bi(bi) { _bi->ignore(32); }
    void extractTo(ostream &os) { for (int b = _read(); b != -1; b = _read()) os.put(b); }
};


#endif



