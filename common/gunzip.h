#ifndef _GUNZIP_H_
#define _GUNZIP_H_
#include "inflate.h"

class GzipStream
{
    BitInput *_bi;
    string _readString();
public:
    GzipStream(BitInput *bi) : _bi(bi) { }
    void extractTo(ostream &os);
};

#endif


