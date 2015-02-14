#ifndef _BUFTEST_H_
#define _BUFTEST_H_
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include "fector.h"

class buftest : public streambuf
{
    FILE *_fp;
    const uint32_t _put_back;
    Fector<char> _buffer;
    uint32_t _pos = 0;
public:
    buftest(FILE *fp) : _fp(fp), _put_back(8), _buffer(264) { }

    fpos<mbstate_t> seekoff(int64_t off, ios::seekdir way, ios::openmode m)
    {
        return (uint64_t)_M_in_cur - (uint64_t)_M_in_beg + ftell(_fp) - _buffer.size();
    }

    int underflow()
    {
        if (gptr() < egptr())
            return (int)(*gptr());

        char *base = _buffer.begin();
        char *start = base;

        if (eback() == base)
        {
            memmove(base, egptr() - _put_back, _put_back);
            start += _put_back;
        }

        uint32_t n = fread(start, 1, _buffer.size() - (start - base), _fp);

        if (n == 0)
            return EOF;

        setg(base, start, start + n);
        return (int)(*gptr());
    }

#if 0
    int uflow()
    {
        int ret = traits_type::eof();
        bool testof = traits_type::eq_int_type(underflow(), ret);

        if (!testof)
        {
            ret = (int)(*gptr());
            gbump(1);
        }

        return ret;
    }
#endif
};

#endif


