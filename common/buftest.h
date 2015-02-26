#ifndef _BUFTEST_H_
#define _BUFTEST_H_
#include "common.h"
#include "fector.h"


class buftest : public streambuf
{
    FILE *_fp;
    const uint32_t _put_back;
    char _buffer[264];
    uint32_t _pos = 0;
public:
    buftest(FILE *fp) : _fp(fp), _put_back(8) { }

    fpos<mbstate_t> seekoff(int64_t off, ios::seekdir way, ios::openmode m)
    {
        return (uint64_t)_M_in_cur - (uint64_t)_M_in_beg + ftell(_fp) - 264;
    }

    int underflow()
    {
        if (gptr() < egptr())
            return (int)(*gptr());

        char *base = _buffer;
        char *start = base;

        if (eback() == base)
        {
            memmove(base, egptr() - _put_back, _put_back);
            start += _put_back;
        }

        uint32_t n = fread(start, 1, 264 - (start - base), _fp);

        if (n == 0)
            return EOF;

        setg(base, start, start + n);
        return (int)(*gptr());
    }   
};

#endif


