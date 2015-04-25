#ifndef _GUNZIP_H_
#define _GUNZIP_H_
#include "inflate.h"

class GzipStream
{
    BitInput2 *_bi;
    Inflate _d;
    string _readString();
    bool _headerIsRead = false;
public:
    GzipStream(BitInput2 *bi) : _bi(bi), _d(bi) { }
    void readHeader();
    bool read(ostream &os);
    int read();
    int read(char *buf, int n);
    void extractTo(ostream &os);
};

class GzipStreamBuf : public streambuf
{
    GzipStream _gs;
    const uint32_t _put_back = 8;
    char _buffer[264];
    uint32_t _pos2 = 0;
    uint32_t _lastRead2 = 0;
public:
    GzipStreamBuf(BitInput2 *bi) : _gs(bi) { }
    streampos seekoff(ios::streamoff off, ios::seekdir way, ios::openmode m);
    int underflow();
};

#endif


