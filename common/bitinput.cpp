#include "bitinput.h"

uint32_t BitInput::readBits(uint32_t count)
{
    for (; _bitCount < count; _bitCount += 8)
        _bitBuffer = _bitBuffer << 8 | getc();

    _bitCount -= count;
    return _bitBuffer >> _bitCount & ((1 << count) - 1);
}





