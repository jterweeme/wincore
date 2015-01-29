#include "bunzip2.h"

uint8_t Block::_indexToFront(uint8_t *a, uint32_t index)
{
    uint8_t value = a[index];
    for (uint32_t i = index; i > 0; i--) a[i] = a[i - 1];
    return a[0] = value;
}

uint32_t Block::_nextSymbol(BitInput *bi, Vugt selectors)
{

    if (++_grpPos % 50 == 0)
        _curTbl = selectors[++_grpIdx];

    for (uint32_t n = _minLengths[_curTbl], codeBits = bi->readBits(n); n <= 23; n++)
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
    _randomCount = rnums[0] - 1;
    bi->readInt();
    _blockRandomised = bi->readBool();
    _acc = _rleRepeat = _length = _curp = _dec = _randomIndex = _curTbl = 0;
    uint32_t bwtStartPointer = bi->readBits(24), symbolCount = 0;
    uint8_t tableCodeLengths[6][258];

    for (uint16_t i = 0, ranges = bi->readBits(16); i < 16; i++)
        if ((ranges & ((1 << 15) >> i)) != 0)
            for (int j = 0, k = i << 4; j < 16; j++, k++)
                if (bi->readBool())
                    _symbolMap[symbolCount++] = (uint8_t)k;

    uint32_t eob = symbolCount + 1, tables = bi->readBits(3), selectors_n = bi->readBits(15);
    uint8_t tableMTF[256];
    _generate(tableMTF);
    Vugt selectors;

    for (uint32_t i = 0; i < selectors_n; i++)
        selectors.push_back(_indexToFront(tableMTF, bi->readUnary()));

    for (uint32_t t = 0; t < tables; t++)
    {
        for (uint32_t i = 0, c = bi->readBits(5); i <= eob; i++)
        {
            while (bi->readBool()) c += bi->readBool() ? -1 : 1;
            tableCodeLengths[t][i] = c;
        }
    }

    for (uint32_t table = 0, minLength = 23, maxLength = 0; table < 6; table++)
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

    _curTbl = selectors[0];
    uint8_t symbolMTF[256];
    _generate(symbolMTF);
    _length = 0;

    for (int n = 0, inc = 1, mtfValue = 0;;)
    {
        uint32_t nextSymbol = _nextSymbol(bi, selectors);

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
                while (--n >= 0) _bwtBlock[_length++] = nextByte;
                n = 0;
                inc = 1;
            }

            if (nextSymbol == eob)
                break;

            mtfValue = _indexToFront(symbolMTF, nextSymbol - 1) & 0xff;
            uint8_t nextByte = _symbolMap[mtfValue];
            _bwtByteCounts[nextByte & 0xff]++;
            _bwtBlock[_length++] = nextByte;
        }
    }

    int characterBase[256] = {0};

    for (int i = 0; i < 255; i++)
        characterBase[i + 1] = _bwtByteCounts[i];

    for (int i = 2; i <= 255; i++)
        characterBase[i] += characterBase[i - 1];

    for (uint32_t i = 0; i < _length; i++)
    {
        int value = _bwtBlock[i] & 0xff;
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
    if (_streamComplete)
        return false;

    uint32_t marker1 = _bi->readBits(24), marker2 = _bi->readBits(24);

    if (marker1 == 0x314159 && marker2 == 0x265359)
    {
        _bd.init(_bi);
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

uint32_t Block::_nextByte()
{
    int next = _curp & 0xff;
    _curp = _merged[_curp >> 8];

    if (_blockRandomised && --_randomCount == 0)
        next ^= 1, _randomIndex = (_randomIndex + 1) % 512, _randomCount = rnums[_randomIndex];

    _dec++;
    return next;
}

int Block::read()
{
    while (_rleRepeat < 1)
    {
        if (_dec == _length)
            return -1;

        int nextByte = _nextByte();

        if (nextByte != _last)
        {
            _last = nextByte;
            _rleRepeat = 1;
            _acc = 1;
        }
        else if (++_acc == 4)
        {
            _rleRepeat = _nextByte() + 1;
            _acc = 0;
        }
        else
        {
            _rleRepeat = 1;
        }
    }

    _rleRepeat--;
    return _last;
}


