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
    uint32_t _pos = 0;
public:
    buftest(FILE *fp) : _fp(fp), _put_back(8), _buffer(264) { }

    fpos<mbstate_t> seekoff(int64_t off, ios::seekdir way, ios::openmode m)
    {
        cerr << "seekoff " << off << " " << way << " " << way << "\n";

        return (uint64_t)_M_in_cur - (uint64_t)_M_in_beg;
    }

    streambuf *setbuf(char *s, long n)
    {
        cerr << "setbuf\n";
        return streambuf::setbuf(s, n);
    }

    fpos<mbstate_t> seekpos(streampos sp, ios::openmode m)
    {
        cerr << "seekpos\n";
        return streambuf::seekpos(sp, m);
    }

    int sync()
    {
        cerr << "sync\n";
        return sync();
    }

    long showmanyc()
    {
        cerr << "showmanyc\n";
        return streambuf::showmanyc();
    }

    int pbackfail(int c)
    {
        cerr << "pbackfail\n";
        return streambuf::pbackfail(c);
    }

    int underflow()
    {
        cerr << "underflow\n";
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
        return (int)(*gptr());
    }

    long xsgetn(char *s, long n)
    {
        cerr << "xsgetn\n";
        return 0;
    }

    long xsputn(const char *s, long n)
    {
        cerr << "xsputn\n";
        return 0;
    }

    int overflow(int c)
    {
        cerr << "overflow\n";
        return 0;
    }

    int uflow()
    {
        cerr << "uflow\n";
        int ret = traits_type::eof();
        bool testof = traits_type::eq_int_type(underflow(), ret);

        if (!testof)
        {
            ret = (int)(*gptr());
            gbump(1);
        }

        return ret;
    }

    void imbue(const locale &loc)
    {
        cerr << "imbue\n";
    }
};

#endif


