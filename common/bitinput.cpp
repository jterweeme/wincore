#include "bitinput.h"

uint32_t BitInput::readBits(uint32_t count)
{
    if (count > 24)
        throw "Maximaal 24 bits";

    for (; _bitCount < count; _bitCount += 8)
        _bitBuffer = _bitBuffer << 8 | _getc();

    _bitCount -= count;
    return _bitBuffer >> _bitCount & ((1 << count) - 1);
}

uint32_t BitInput2::readBit()
{
    if (_bitPos == 8) _nextBits = _getc(), _bitPos = 0;
    return _nextBits >> _bitPos++ & 1;
}

uint32_t BitInput2::readBits(uint32_t n)
{
    uint32_t r = 0;
    for (uint32_t i = 0; i < n; i++) r |= readBit() << i;
    return r;
}



