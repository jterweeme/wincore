#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stdio.h>

class MyIstream
{
    FILE *_fp;
    size_t _lastRead;
    bool _eof;
public:
    MyIstream(FILE *fp) : _fp(fp), _lastRead(0), _eof(false) { }
    void getline(char *dest, int n)
    {
        for (int i = 0, c = fgetc(_fp); i < n && c != '\n' && c != -1; i++)
        {
            dest[i] = c;
            c = fgetc(_fp);
        }
    }
    operator void *() const { return (void *)!_eof; }
};

class MyOstream
{
    FILE *_fp;
public:
    MyOstream(FILE *fp) : _fp(fp) { }
    MyOstream& operator << (const char *s) { fprintf(_fp, s); return *this; }
};

class MyRegex
{
public:
    MyRegex(const char *s) { }
    
};

namespace mystl
{
    typedef MyIstream istream;
    typedef MyOstream ostream;
    typedef MyRegex regex;
    static istream cin(stdin);
    static ostream cout(stdout);
    static ostream cerr(stderr);
    static bool regex_search(const char *s, MyRegex &rx) { return true; }
};
#endif


