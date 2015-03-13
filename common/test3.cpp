#if 0
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#else
#include "common.h"
#endif

class buftest : public streambuf
{
    FILE *_fp;
    const uint32_t _put_back;
    char _buffer[264];
public:
    buftest(FILE *fp) : _fp(fp), _put_back(8) { }

    streampos seekoff(int64_t off, ios::seekdir way, ios::openmode m)
    {
        return (uint64_t)gptr() == 0 ? 0 : (uint64_t)gptr() - (uint64_t)eback() + ftell(_fp) - 264;
        //return (uint64_t)gptr() - (uint64_t)eback() + ftell(_fp) - 264;
    }

    int underflow();
};

class App
{
public:
    int run(int argc, char **argv);
};

int buftest::underflow()
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

int App::run(int argc, char **argv)
{
    //buftest bt(stdin);
    filebuf bt;
    bt.open("grep.o", ios::in);
    fprintf(stderr, "%u\n", bt.pubseekoff(0, ios::beg, ios::in));
   
    for (int i = 0; i < 765771; i++)
    {
        int c = bt.sbumpc();
        cout.put(c);
    }

    fprintf(stderr, "\n%d\n", bt.in_avail());

#if 0
    for (int i = 0; i < 10; i++)
        cout.put(bt.sbumpc());
#endif

    cout << '\n';
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    int ret = -1;

    try
    {
        ret = app.run(argc, argv);
    }
    catch (const char *e)
    {
        cerr << e << "\n";
    }

    return ret;
}


