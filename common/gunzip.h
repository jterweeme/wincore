#ifndef _GUNZIP_H_
#define _GUNZIP_H_
#include "inflate.h"

class GzipStream
{
    BitInput2 *_bi;
    string _readString();
public:
    GzipStream(BitInput2 *bi) : _bi(bi) { }
    void readHeader();
    int read();
    void extractTo(ostream &os);
};

class GzipStreambuf : public streambuf
{
    GzipStream _gs;
    const uint32_t _put_back = 8;
    char _buffer[264];
    uint32_t _pos = 0;
    uint32_t _lastRead = 0;
public:
    GzipStreambuf(BitInput2 *bi) : _gs(bi) { }
};

#endif


