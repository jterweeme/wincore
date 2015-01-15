#include <fstream>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include "bitinput.h"

typedef vector<uint8_t> Vugt;

int rnums[] = {
            619, 720, 127, 481, 931, 816, 813, 233, 566, 247, 985, 724, 205, 454, 863, 491,
            741, 242, 949, 214, 733, 859, 335, 708, 621, 574, 73, 654, 730, 472, 419, 436,
            278, 496, 867, 210, 399, 680, 480, 51, 878, 465, 811, 169, 869, 675, 611, 697,
            867, 561, 862, 687, 507, 283, 482, 129, 807, 591, 733, 623, 150, 238, 59, 379,
            684, 877, 625, 169, 643, 105, 170, 607, 520, 932, 727, 476, 693, 425, 174, 647,
            73, 122, 335, 530, 442, 853, 695, 249, 445, 515, 909, 545, 703, 919, 874, 474,
            882, 500, 594, 612, 641, 801, 220, 162, 819, 984, 589, 513, 495, 799, 161, 604,
            958, 533, 221, 400, 386, 867, 600, 782, 382, 596, 414, 171, 516, 375, 682, 485,
            911, 276, 98, 553, 163, 354, 666, 933, 424, 341, 533, 870, 227, 730, 475, 186,
            263, 647, 537, 686, 600, 224, 469, 68, 770, 919, 190, 373, 294, 822, 808, 206,
            184, 943, 795, 384, 383, 461, 404, 758, 839, 887, 715, 67, 618, 276, 204, 918,
            873, 777, 604, 560, 951, 160, 578, 722, 79, 804, 96, 409, 713, 940, 652, 934,
            970, 447, 318, 353, 859, 672, 112, 785, 645, 863, 803, 350, 139, 93, 354, 99,
            820, 908, 609, 772, 154, 274, 580, 184, 79, 626, 630, 742, 653, 282, 762, 623,
            680, 81, 927, 626, 789, 125, 411, 521, 938, 300, 821, 78, 343, 175, 128, 250,
            170, 774, 972, 275, 999, 639, 495, 78, 352, 126, 857, 956, 358, 619, 580, 124,
            737, 594, 701, 612, 669, 112, 134, 694, 363, 992, 809, 743, 168, 974, 944, 375,
            748, 52, 600, 747, 642, 182, 862, 81, 344, 805, 988, 739, 511, 655, 814, 334,
            249, 515, 897, 955, 664, 981, 649, 113, 974, 459, 893, 228, 433, 837, 553, 268,
            926, 240, 102, 654, 459, 51, 686, 754, 806, 760, 493, 403, 415, 394, 687, 700,
            946, 670, 656, 610, 738, 392, 760, 799, 887, 653, 978, 321, 576, 617, 626, 502,
            894, 679, 243, 440, 680, 879, 194, 572, 640, 724, 926, 56, 204, 700, 707, 151,
            457, 449, 797, 195, 791, 558, 945, 679, 297, 59, 87, 824, 713, 663, 412, 693,
            342, 606, 134, 108, 571, 364, 631, 212, 174, 643, 304, 329, 343, 97, 430, 751,
            497, 314, 983, 374, 822, 928, 140, 206, 73, 263, 980, 736, 876, 478, 430, 305,
            170, 514, 364, 692, 829, 82, 855, 953, 676, 246, 369, 970, 294, 750, 807, 827,
            150, 790, 288, 923, 804, 378, 215, 828, 592, 281, 565, 555, 710, 82, 896, 831,
            547, 261, 524, 462, 293, 465, 502, 56, 661, 821, 976, 991, 658, 869, 905, 758,
            745, 193, 768, 550, 608, 933, 378, 286, 215, 979, 792, 961, 61, 688, 793, 644,
            986, 403, 106, 366, 905, 644, 372, 567, 466, 434, 645, 210, 389, 550, 919, 135,
            780, 773, 635, 389, 707, 100, 626, 958, 165, 504, 920, 176, 193, 713, 857, 265,
            203, 50, 668, 108, 645, 990, 626, 197, 510, 357, 358, 850, 858, 364, 936, 638 };

class Huffman
{
    BitInput *_bi;
    const Vugt _selectors;
    int _minLengths[6];
    uint32_t _codeBases[6][25], _codeLimits[6][24], _codeSymbols[6][258];
    uint32_t _curTbl = 0, _groupIdx = -1, _groupPos = -1;
public:
    int nextSymbol();
    Huffman(BitInput *bis, int nalphabet, uint8_t tblCodeLengths[6][258], Vugt selectors);
};

class BlockDec
{
    BitInput *_bi;
    int _rleRepeat = 0;
    int _rleAccumulator = 0;
    int _bwtCurrentMergedPointer = 0;
    uint8_t _bwtBlock[900000] = {0};
    uint32_t _bwtBlockLength = 0;
    uint32_t _bwtBytesDecoded = 0;
    int _decodeNextBWTByte();
    uint8_t _huffmanSymbolMap[256] = {0};
    int _bwtByteCounts[256], _bwtMergedPointers[9999], _blockRandomised = 0;
    int _randomCount = rnums[0] - 1, _randomIndex = 0, _rleLastDecodedByte = -1;
    void _generate(uint8_t *a) { for (unsigned i = 0; i < 256; i++) a[i] = i; }
    uint8_t _indexToFront(uint8_t *a, int i);
    int _decNextBWTByte();
public:
    int read();
    BlockDec(BitInput *bi) : _bi(bi) { }
    void reset();
};

class DecStream
{
    BitInput *_bi;
    BlockDec _bd;
    int _streamComplete = false;
    bool _initNextBlock();
public:
    DecStream(BitInput *bi);
    void extractTo(ostream &os);
    int read();
};

class App
{
public:
    int run(int argc, char **argv);
};

DecStream::DecStream(BitInput *bi) : _bi(bi), _bd(bi)
{
    _bi->readBits(16);
    _bi->readBits(16);
    _initNextBlock();
}

uint8_t BlockDec::_indexToFront(uint8_t *a, int index)
{
    uint8_t value = a[index];
    
    for (int i = 0; i < index; i++)
        a[i + 1] = a[i];

    return a[0] = value;
}

void BlockDec::reset()
{
    _bi->readInt();
    _blockRandomised = _bi->readBool();
    uint32_t bwtStartPointer = _bi->readBits(24), symbolCount = 0;
    uint8_t tableCodeLengths[6][258];

    for (uint16_t i = 0, ranges = _bi->readBits(16); i < 16; i++)
        if ((ranges & ((1 << 15) >> i)) != 0)
            for (int j = 0, k = i << 4; j < 16; j++, k++)
                if (_bi->readBool())
                    _huffmanSymbolMap[symbolCount++] = (uint8_t)k;

    uint32_t _eob = symbolCount + 1;
    uint32_t totalTables = _bi->readBits(3), totalSelectors = _bi->readBits(15);
    uint8_t tableMTF[256];
    _generate(tableMTF);
    Vugt selectors(totalSelectors);

    for (uint32_t selector = 0; selector < totalSelectors; selector++)
        selectors.push_back(_indexToFront(tableMTF, _bi->readUnary()));
        //selectors[selector] = _indexToFront(tableMTF, _bi->readUnary());

    for (uint32_t table = 0; table < totalTables; table++)
    {
        uint8_t currentLength = _bi->readBits(5);

        for (uint32_t i = 0; i <= _eob; i++)
        {
            while (_bi->readBool())
                currentLength += _bi->readBool() ? -1 : 1;

            tableCodeLengths[table][i] = currentLength;
        }
    }

    Huffman h(_bi, symbolCount + 2, tableCodeLengths, selectors);
    uint8_t symbolMTF[256];
    _generate(symbolMTF);
    _bwtBlockLength = 0;
   
    for (int n = 0, repeatIncrement = 1, mtfValue = 0; n < 2; n++)
    {
        uint32_t nextSymbol = h.nextSymbol();
        //int nextSymbol = _eob;

#if 0
        if (nextSymbol == 0)
        {
            n += repeatIncrement;
            repeatIncrement <<= 1;
        }
        else if (nextSymbol == 1)
        {
            n += repeatIncrement << 1;
            repeatIncrement <<= 1;
        }
        else
        {
            if (n > 0)
            {
                uint8_t nextByte = _huffmanSymbolMap[mtfValue];
                _bwtByteCounts[nextByte & 0xff] += n;
                
                while (--n >= 0)
                    _bwtBlock[_bwtBlockLength++] = nextByte;

                n = 0;
                repeatIncrement = 1;
            }

            if (nextSymbol == _eob)
                break;

            mtfValue = _indexToFront(symbolMTF, nextSymbol - 1) & 0xff;
            uint8_t nextByte = _huffmanSymbolMap[mtfValue];
            _bwtByteCounts[nextByte & 0xff]++;
            _bwtBlock[_bwtBlockLength++] = nextByte;
        }
#endif
    }

    cerr << "BWTBlockLength: " << _bwtBlockLength << "\n";

    int characterBase[256];
    
    for (int i = 0; i < 255; i++)
        characterBase[i + 1] = _bwtByteCounts[i];

    for (uint32_t i = 0; i < _bwtBlockLength; i++)
    {
        int value = _bwtBlock[i] & 0xff;
        _bwtMergedPointers[characterBase[value]++] = (i << 8) + value;
    }

    _bwtCurrentMergedPointer = _bwtMergedPointers[bwtStartPointer];
}

Huffman::Huffman(BitInput *bi, int nalphabet, uint8_t tblCodeLengths[6][258], Vugt selectors)
  :
    _bi(bi),
    _selectors(selectors),
    _curTbl(_selectors.front())
{
    for (uint8_t table = 0, minLength = 23, maxLength = 0; table < 6; table++)
    {
        for (int i = 0; i < nalphabet; i++)
        {
            maxLength = max(tblCodeLengths[table][i], maxLength);
            minLength = min(tblCodeLengths[table][i], minLength);
        }

        _minLengths[table] = minLength;

        for (int i = 0; i < nalphabet; i++)
            _codeBases[table][tblCodeLengths[table][i] + 1]++;

        for (int i = 1; i < 25; i++)
            _codeBases[table][i] += _codeBases[table][i - 1];

        for (int i = minLength, code = 0; i <= maxLength; i++)
        {
            int base = code;
            code += _codeBases[table][i + 1] - _codeBases[table][i];
            _codeBases[table][i] = base - _codeBases[table][i];
            _codeLimits[table][i] = code - 1;
            code <<= 1;
        }

        for (int bitLength = minLength, i = 0; bitLength <= maxLength; bitLength++)
            for (int symbol = 0; symbol < nalphabet; symbol++)
                if (tblCodeLengths[table][symbol] == bitLength)
                    _codeSymbols[table][i++] = symbol;
    }
    cerr << "Huffman constructor\n";
}

int Huffman::nextSymbol()
{
    if (++_groupPos % 50 == 0)
    {
        if (++_groupIdx == _selectors.size())
            throw "Error decoding BZip2 block";

        _curTbl = _selectors[_groupIdx] & 0xff;
    }

    for (uint32_t n = _minLengths[_curTbl], codeBits = _bi->readBits(n); n <= 23; n++)
    {
        cerr << n << " " << _curTbl << " " << codeBits << "\n";
        return 0;
        if (codeBits <= _codeLimits[_curTbl][n])
            return 0;
            return _codeSymbols[0][0];
#if 0
            return _codeSymbols[_curTbl][codeBits - _codeBases[_curTbl][n]];
#endif
        codeBits = codeBits << 1 | _bi->readBits(1);
    }
    
    throw "Error decoding BZip2 block";
    return 0;
}

void DecStream::extractTo(ostream &os)
{
    for (int b = read(); b != -1; b = read())
        os.put(b);
}

int DecStream::read()
{
#if 1
    int nextByte = _bd.read();

    if (nextByte == -1 && _initNextBlock())
        nextByte = _bd.read();

    return nextByte;
#else
    return -1;
#endif
}

bool DecStream::_initNextBlock()
{
    if (_streamComplete)
        return false;

    uint32_t marker1 = _bi->readBits(24), marker2 = _bi->readBits(24);

    if (marker1 == 0x314159 && marker2 == 0x265359)
    {
        _bd.reset();
        return true;
    }
    else if (marker1 == 0x177245 && marker2 == 0x385090)
    {
        _streamComplete = true;
        _bi->readInt();
        return false;
    }

    _streamComplete = true;
    throw "BZip2 stream format error";
}

int BlockDec::_decodeNextBWTByte()
{
    int mergedPointer = _bwtCurrentMergedPointer, nextDecodedByte = mergedPointer & 0xff;
    _bwtCurrentMergedPointer = _bwtMergedPointers[mergedPointer >> 8];

    if (_blockRandomised && --_randomCount == 0)
    {
        nextDecodedByte ^= 1;
        _randomIndex = (_randomIndex + 1) % 512;
        _randomCount = rnums[_randomIndex];
    }

    _bwtBytesDecoded++;
    return nextDecodedByte;
}

int BlockDec::read()
{
    while (_rleRepeat < 1)
    {
        if (_bwtBytesDecoded == _bwtBlockLength)
            return -1;

        int nextByte = _decodeNextBWTByte();

        if (nextByte != _rleLastDecodedByte)
        {
            _rleLastDecodedByte = nextByte;
            _rleRepeat = 1;
            _rleAccumulator = 1;
        }
        else if (++_rleAccumulator == 4)
        {
            _rleRepeat = _decodeNextBWTByte() + 1;
            _rleAccumulator = 0;
        }
        else
        {
            _rleRepeat = 1;
        }
    }

    _rleRepeat--;
    return _rleLastDecodedByte;
}

int App::run(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    BitInput bi(&ifs);
    DecStream ds(&bi);
    ofstream ofs(argv[2]);
    ds.extractTo(ofs);
    ofs.close();
    ifs.close();
    return 0;
}

int main(int argc, char **argv)
{
    App app;

    try
    {
        return app.run(argc, argv);
    }
    catch (const char *e)
    {
        cout << e << "\n";
    }
    catch (...)
    {
        cout << "Unknown exception\n";
    }

    return -1;
}


