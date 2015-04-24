#include "gunzip.h"

void GzipStream::readHeader()
{
    uint8_t x[10];
    _bi->read(x, 10);
    uint8_t flags = x[3];
    
    if (flags & 4)
    {
        uint8_t b[2];
        _bi->read(b, 2);
        int len = b[0] | b[1] << 8;
        _bi->ignoreBytes(len);
    }

    if (flags & 8)
        _readString();

    if (flags & 2)
        _bi->ignoreBytes(2);

    if (flags & 0x10)
        _readString();

    _headerIsRead = true;
}

bool GzipStream::read(ostream &os)
{
    if (!_headerIsRead)
        readHeader();

    return _d.read(os);
}

void GzipStream::extractTo(ostream &os)
{
    readHeader();
    _d.extractTo(os);
}

string GzipStream::_readString()
{
    string s;
    for (int c = _bi->readByte(); c != 0; c = _bi->readByte()) s.push_back(c);
    return s;
}


