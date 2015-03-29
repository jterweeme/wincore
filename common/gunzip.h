#ifndef _GUNZIP_H_
#define _GUNZIP_H_
#include "inflate.h"

class GzipStream : public std::istream
{
    BitInput *_bi;
    std::string _readString();
public:
    GzipStream(BitInput *bi) : _bi(bi) { }
    void extractTo(std::ostream &os);
};

#endif


