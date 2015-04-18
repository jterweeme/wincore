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

#endif


