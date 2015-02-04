#ifndef _BUFTEST_H_
#define _BUFTEST_H_
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

class buftest : public streambuf
{
    FILE *_fp;
    const uint32_t _put_back;
    vector<char> _buffer;
public:
    buftest(FILE *fp) : _fp(fp), _put_back(8), _buffer(264) { }

    int underflow()
    {
        if (gptr() < egptr())
            return traits_type::to_int_type(*gptr());

        char *base = &_buffer.front();
        char *start = base;

        if (eback() == base)
        {
            memmove(base, egptr() - _put_back, _put_back);
            start += _put_back;
        }

        uint32_t n = fread(start, 1, _buffer.size() - (start - base), _fp);

        if (n == 0)
            return traits_type::eof();

        setg(base, start, start + n);
        return traits_type::to_int_type(*gptr());
    }

};

#endif


