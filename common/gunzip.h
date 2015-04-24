#ifndef _GUNZIP_H_
#define _GUNZIP_H_
#include "inflate.h"

class GzipStream
{
    BitInput2 *_bi;
    Inflate _d;
    string _readString();
    bool _headerIsRead = false;
    stringstream _buf;
public:
    GzipStream(BitInput2 *bi) : _bi(bi), _d(bi) { }
    void readHeader();
    bool read(ostream &os);
    int read();
    void extractTo(ostream &os);
};

class GzipStreamBuf : public streambuf
{
    GzipStream _gs;
public:
    GzipStreamBuf(BitInput2 *bi) : _gs(bi) { }
    streampos seekoff(ios::streamoff off, ios::seekdir way, ios::openmode m) { return 0; }
    int underflow();
};

#endif


