#ifndef _GUNZIP2_H_
#define _GUNZIP2_H_
#include "bitinput.h"

class GzipStream2
{
    BitInputBase *_bi;
public:
    GzipStream2(BitInputBase *bi) { }
    int read();
};

class GzipStreambuf2 : public streambuf
{
    GzipStream2 _gs;
    const uint32_t _put_back = 8;
    char _buffer[264];
    uint32_t _pos = 0;
    uint32_t _lastRead = 0;
public:
    GzipStreambuf2(BitInputBase *bi) : _gs(bi) { }
    streampos seekoff(ios::streamoff off, ios::seekdir way, ios::openmode m) { return 0; }
    int underflow();
};

#endif



