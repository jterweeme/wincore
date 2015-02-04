#include "bitinput.h"
#include <vector>
#include <stdint.h>
#include <cstring>
#include <fstream>
#include "fector.h"

class Block
{
    uint32_t _minLengths[6] = {0}, _bwtByteCounts[256] = {0}, *_merged;
    uint32_t _bases[6][25] = {}, _symbols[6][258] = {};
    int32_t _limits[6][24] = {};
    int32_t _curTbl, _grpIdx, _grpPos, _last, _acc, _rleRepeat = 0, _randomIndex, _randomCount;
    bool _blockRandomised = false;
    uint8_t _bwtBlock[900000] = {0}, _symbolMap[256] = {0}, _indexToFront(uint8_t *a, uint32_t i);
    uint32_t _curp = 0, _length = 0, _dec = 0;
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
    DecStream(BitInput *bi) : _bi(bi)
    {
        uint32_t magic = bi->readUInt32();
        cerr << "Magic: " << hex << magic << "\n";
    }
    void extractTo(ostream &os) { for (int b = _read(); b != -1; b = _read()); }
};

uint8_t Block::_indexToFront(uint8_t *a, uint32_t index)
{
    uint8_t value = a[index];
    for (uint32_t i = index; i > 0; i--) a[i] = a[i - 1];
    return a[0] = value;
}

uint32_t Block::_nextSymbol(BitInput *bi, const Fugt &selectors)
{
    if (++_grpPos % 50 == 0)
        _curTbl = selectors.at(++_grpIdx);

    uint32_t n = _minLengths[_curTbl];
    int32_t codeBits = bi->readBits(n);

    for (; n <= 23; n++)
    {
        if (codeBits <= _limits[_curTbl][n])
            return _symbols[_curTbl][codeBits - _bases[_curTbl][n]];

        codeBits = codeBits << 1 | bi->readBits(1);
    }

    return 0;
}

void Block::init(BitInput *bi)
{
    _grpIdx = _grpPos = _last = -1;
    uint32_t crc = bi->readUInt32();
    cerr << "Block CRC: " << hex << crc << "\n";
    _blockRandomised = bi->readBool();
    _acc = _rleRepeat = _length = _curp = _dec = _randomIndex = _curTbl = 0;
    uint32_t bwtStartPointer = bi->readBits(24), symbolCount = 0;
    cerr << "BWT Start: " << dec << bwtStartPointer << "\n";
    uint8_t tableCodeLengths[6][258];
    uint16_t ranges = bi->readBits(16);
    cerr << "Huffman Used Map: " << ranges << "\n";

    for (uint16_t i = 0; i < 16; i++)
        if ((ranges & ((1 << 15) >> i)) != 0)
            for (int j = 0, k = i << 4; j < 16; j++, k++)
                if (bi->readBool())
                    _symbolMap[symbolCount++] = (uint8_t)k;

    cerr << "SymbolCount: " << dec << symbolCount << "\n";
    uint32_t eob = symbolCount + 1, tables = bi->readBits(3), selectors_n = bi->readBits(15);
    cerr << "Huffman Groups: " << tables << "\n";
    cerr << "Selectors: " << selectors_n << "\n";
    uint8_t tableMTF[256];
    _generate(tableMTF);


    Fugt selectors2(selectors_n);

    for (uint32_t i = 0; i < selectors_n; i++)
    {
        uint32_t x = bi->readUnary();
        uint8_t y = _indexToFront(tableMTF, x);
        selectors2.set(i, y);
    }

    //cerr << selectors2.toString() << "\n";

    //cerr << "Selectors: " << selectors.size() << "\n";

    for (uint32_t t = 0; t < tables; t++)
    {
        for (uint32_t i = 0, c = bi->readBits(5); i <= eob; i++)
        {
            while (bi->readBool()) c += bi->readBool() ? -1 : 1;
            tableCodeLengths[t][i] = c;
        }
    }

    for (uint32_t table = 0, minLength = 23, maxLength = 0; table < tables; table++)
    {
        for (uint32_t i = 0; i < symbolCount + 2; i++)
        {
            maxLength = max((uint32_t)tableCodeLengths[table][i], maxLength);
            minLength = min((uint32_t)tableCodeLengths[table][i], minLength);
        }

        _minLengths[table] = minLength;

        for (uint32_t i = 0; i < symbolCount + 2; i++)
            _bases[table][tableCodeLengths[table][i] + 1]++;

        for (uint32_t i = 1; i < 25; i++)
            _bases[table][i] += _bases[table][i - 1];

        for (uint32_t i = minLength, code = 0; i <= maxLength; i++)
        {
            int base = code;
            code += _bases[table][i + 1] - _bases[table][i];
            _bases[table][i] = base - _bases[table][i];
            _limits[table][i] = code - 1;
            code <<= 1;
        }

        for (uint32_t n = minLength, i = 0; n <= maxLength; n++)
            for (uint32_t symbol = 0; symbol < symbolCount + 2; symbol++)
                if (tableCodeLengths[table][symbol] == n)
                    _symbols[table][i++] = symbol;
    }

    _curTbl = selectors2.at(0);
    uint8_t symbolMTF[256];
    _generate(symbolMTF);
    _length = 0;

    for (int n = 0, inc = 1, mtfValue = 0;;)
    {
        uint32_t nextSymbol = _nextSymbol(bi, selectors2);

        if (nextSymbol == 0)
        {
            n += inc;
            inc <<= 1;
        }
        else if (nextSymbol == 1)
        {
            n += inc << 1;
            inc <<= 1;
        }
        else
        {
            if (n > 0)
            {
                uint8_t nextByte = _symbolMap[mtfValue];
                _bwtByteCounts[nextByte & 0xff] += n;

                while (--n >= 0)
                {
                    _bwtBlock2.set(_length, nextByte);
                    _bwtBlock[_length++] = nextByte;
                }

                n = 0;
                inc = 1;
            }

            if (nextSymbol == eob)
                break;

            mtfValue = _indexToFront(symbolMTF, nextSymbol - 1) & 0xff;
            uint8_t nextByte = _symbolMap[mtfValue];
            _bwtByteCounts[nextByte & 0xff]++;
            _bwtBlock2.set(_length, nextByte);
            _bwtBlock[_length++] = nextByte;
        }
    }

    cerr << "Length: " << dec << _length << "\n";
    _merged = new uint32_t[_length]();
    int characterBase[256] = {0};
    for (int i = 0; i < 255; i++) characterBase[i + 1] = _bwtByteCounts[i];
    for (int i = 2; i <= 255; i++) characterBase[i] += characterBase[i - 1];

    for (uint32_t i = 0; i < _length; i++)
    {
        int value = _bwtBlock2.at(i) & 0xff;
        value = _bwtBlock[i] & 0xff;
        _merged[characterBase[value]++] = (i << 8) + value;
    }

    _curp = _merged[bwtStartPointer];
}

int DecStream::_read()
{
    int nextByte = _bd.read();
    return nextByte = nextByte == -1 && _initNextBlock() ? _bd.read() : nextByte;
}

bool DecStream::_initNextBlock()
{
    if (_streamComplete) return false;
    uint32_t marker1 = _bi->readBits(24), marker2 = _bi->readBits(24);

    if (marker1 == 0x314159 && marker2 == 0x265359)
    {
        _bd.init(_bi);
        return true;
    }
    else if (marker1 == 0x177245 && marker2 == 0x385090)
    {
        _streamComplete = true;
        uint32_t crc = _bi->readInt();
        cerr << "Stream CRC: " << hex << crc << "\n";
        return false;
    }

    _streamComplete = true;
    throw string("BZip2 stream format error");
}

int Block::read()
{
    for (int nextByte; _rleRepeat < 1;)
    {
        if (_dec == _length)
            return -1;

        if ((nextByte = _nextByte()) != _last)
            _last = nextByte, _rleRepeat = 1, _acc = 1;
        else if (++_acc == 4)
            _rleRepeat = _nextByte() + 1, _acc = 0;
        else
            _rleRepeat = 1;
    }

    _rleRepeat--;
    return _last;
}

class App
{
    void test(BitInput *bi)
    {
        uint32_t magic = bi->readUInt32();
        cerr << "Magic: " << hex << magic << "\n";
    }
public:
    int run(int argc, char **argv);
};

int App::run(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    BitInput bi(&ifs);
    DecStream ds(&bi);
    ds.extractTo(cerr);
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
    catch (string s)
    {
        cout << s << "\n";
    }
    catch (const char *e)
    {
        cout << e << "\n";
    }
    catch (bad_alloc)
    {
        cout << "bad alloc\n";
    }
    catch (bad_exception)
    {
        cout << "bad exception\n";
    }
    catch (exception &e)
    {
        cout << "Onbekend exception " << e.what() << "\n";
    }
    catch (...)
    {
        cout << "Unknown exception\n";
    }
    return 0;
}


