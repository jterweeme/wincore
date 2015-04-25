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

int GzipStream::read()
{
    if (!_headerIsRead)
        readHeader();

    return _d.read();
}

#if 0
int GzipStream::read()
{
    int ret = _buf.get();

    if (ret == -1)
    {
        read(_buf);
        int v = _buf.get();
        return v;
    }

    return ret;

}
#endif

streampos GzipStreamBuf::seekoff(ios::streamoff off, ios::seekdir way, ios::openmode m)
{
    streampos pos = _pos2 - ((uint64_t)egptr() - (uint64_t)gptr());
    return pos;
}

int GzipStream::read(char *buf, int n)
{
    int i = 0;

    for (i = 0; i < n; i++)
    {
        int c = read();

        if (c == -1)
            return i;

        buf[i] = c;
    }
    return i;

}

void GzipStream::extractTo(ostream &os)
{
    readHeader();
#if 0
    _d.extractTo(os);
#else
    for (int v = 0; (v = read()) != -1;)
        os.put((uint8_t)v);
#endif
}

string GzipStream::_readString()
{
    string s;
    for (int c = _bi->readByte(); c != 0; c = _bi->readByte()) s.push_back(c);
    return s;
}

int GzipStreamBuf::underflow()
{
    if (gptr() < egptr()) return (int)(*gptr());
    char *base = _buffer;
    char *start = base;

    if (eback() == base)
    {
        memmove(base, egptr() - _put_back, _put_back);
        start += _put_back;
    }

    _lastRead2 = _gs.read(start, 264 - (start - base));
    _pos2 += _lastRead2;
    if (_lastRead2 == 0) return EOF;
    setg(base, start, start + _lastRead2);
    return (uint8_t)(*gptr());
}



