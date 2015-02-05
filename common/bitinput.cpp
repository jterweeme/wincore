#include "bitinput.h"

uint32_t BitInputStream::readBits(uint32_t count)
{
    for (; _bitCount < count; _bitCount += 8)
        _bitBuffer = _bitBuffer << 8 | _is->get();

    _bitCount -= count;
    return _bitBuffer >> _bitCount & ((1 << count) - 1);
}


