namespace mystl { }
using namespace mystl;
#include "mystl.h"

//#include "common.h"

streamsize streambuf2::in_avail()
{
    uint64_t a = (uint64_t)egptr() - (uint64_t)gptr(); return a == 0 ? showmanyc() : a;
}

filebuf2 *filebuf2::open(const char *fn, ios2::openmode m)
{
    switch (m)
    {
    case ios::in:
        _fp = fopen(fn, "r");
        break;
    case ios::out:
        _fp = fopen(fn, "w");
        break;
    default:
        throw "Unknown mode";
    }
    return this;
}

string2::string2(const char *s1, size_t n)
{
    Util2 util;
    util.memset(_s, 0, sizeof(_s));
    util.strncpy(_s, s1, n);
}

string2::string2(const char *s1, const char *s2)
{
    Util2 util;
    util.memset(_s, 0, sizeof(_s));
    util.strncpy(_s, s1, s2 - s1);
}

streamsize streambuf2::xsgetn(char *s, streamsize length)
{
    for (streamsize i = 0; i < length; i++)
    {
        int c = sbumpc();
        if (c == EOF) return i;
        s[i] = c;
    }

    return length;
}

void istream2::getline(char *dest, size_t size)
{
    for (int pos = 0, c = 1; c != '\n'; pos++)
    {
        c = get();

        if (c == EOF)
        {
            dest[pos] = '\0';
            return;
        }

        if (c == '\n')
        {
            dest[pos] = '\0';
            return;
        }

        dest[pos] = c;
    }
}

int fpinbuf::underflow()
{
    if (gptr() < egptr()) return (int)(*gptr());
    char *base = _buffer;
    char *start = base;

    if (eback() == base)
    {
        memmove(base, egptr() - _put_back, _put_back);
        start += _put_back;
    }

    _lastRead = fread(start, 1, 264 - (start - base), _fp);

    if (_lastRead == 0)
    {
        _eof = true;
        return EOF;
    }

    setg(base, start, start + _lastRead);
    return (uint8_t)(*gptr());
}

string2& string2::append(const string2 &s)
{
    for (string2::const_iterator it = s.cbegin(); it != s.cend(); it++)
        push_back(*it);

    return *this;
}

ostream2& ostream2::printInt(const uint32_t u)
{
    Util2 util;
    char s[10] = {0};

    switch (_base.type())
    {
    case base2::OCT:
        util.itoa(u, s, 8);
        break;
    case base2::HEX:
        util.itoa(u, s, 16);
        break;
    default:
        util.itoa(u, s, 10);
    }

    uint32_t fill = (uint32_t)_width.size() > util.strlen(s) ? _width.size() - util.strlen(s) : 0;
    for (uint32_t i = 0; i < fill; i++) put(_fill.fill());
    print(s);
    return *this;
}

ostream2& ostream2::printInt2(int32_t u)
{
    Util2 util;
    char s[10] = {0};

    switch (_base.type())
    {
    case base2::OCT:
        util.itoa(u, s, 8);
        break;
    case base2::HEX:
        util.itoa(u, s, 16);
        break;
    default:
        util.itoa(u, s, 10);
    }

    uint32_t fill = (uint32_t)_width.size() > util.strlen(s) ? _width.size() - util.strlen(s) : 0;
    for (uint32_t i = 0; i < fill; i++) put(_fill.fill());
    print(s);
    return *this;
}

string2 string2::substr(size_t pos) const
{
    string2 s;
    
    for (const_iterator it = cbegin() + pos; it != cend(); it++)
        s.push_back(*it);

    return s;
}

string2 string2::substr(size_t pos, size_t len) const
{
    string2 s;

    for (const_iterator it = cbegin() + pos; it != cend() && it < cbegin() + len + pos; it++)
        s.push_back(*it);

    return s;
}

size_t string2::find(const string2 &s, size_t pos) const
{
    for (; pos + s.length() < length(); pos++)
        if (s.compare(substr(pos, s.length())) == 0)
            return pos;

    return npos;
}

string2& string2::replace(size_t pos, size_t len, const string2 &str)
{
    string s(_s);
    clear();

    for (string2::const_iterator it = s.cbegin(); it != s.cbegin() + pos; it++)
        push_back(*it);

    for (string2::const_iterator it = str.cbegin(); it != str.cend(); it++)
        push_back(*it);

    for (string2::const_iterator it = s.cbegin() + pos + len; it != s.cend(); it++)
        push_back(*it);

    return *this;
}

namespace mystl
{
    void *memcpy(void *d, const void *src, size_t n) { Util2 u; return u.memcpy(d, src, n); }
    void *memset(void *s, const int c, const size_t n) { Util2 u; return u.memset(s, c, n); }
    void *memmove(char *d, const char *s, uint32_t n) { Util2 u; return u.memmove(d, s, n); }
    char *strcpy(char *dest, const char *src) { Util2 util; return util.strcpy(dest, src); }
    char *strncpy(char *d, const char *src, size_t n) { Util2 u; return u.strncpy(d, src, n); }
    int strncmp(const char *s1, const char *s2, size_t n) { Util2 u; return u.strncmp(s1, s2, n); }
    int strcmp(const char *s1, const char *s2) { Util2 u; return u.strcmp(s1, s2); }
    size_t strlen(const char *s) { Util2 u; return u.strlen(s); }
    uint32_t strtol(const char *a, const char *b, int c) { Util2 u; return u.strtol(a, b, c); }
    tm *gmtime(time_t *t) { Time2 u; return u.gmtime(t); }
    tm *localtime(time_t *t) { Time2 u; return u.localtime(t); }

    size_t strftime(char *p, size_t max, const char *fmt, const tm *tp)
    { Time2 u; return u.strftime(p, max, fmt, tp); }

    time_t time(time_t *timer) { Time2 u; return u.time(timer); }
    time_t mktime(tm *timeptr) { Time2 u; return u.mktime(timeptr); }
    base2 hex(base2::HEX);
    base2 oct(base2::OCT);
    base2 dec(base2::DEC);
    ifpstream2 cin(stdin);
    ofpstream2 cout(stdout);
    ofpstream2 cerr(stderr);
    align right;
    fill2 setfill(char c) { return fill2(c); }
    width2 setw(int length) { return width2(length); }
    int toupper(int c) { Util2 u; return u.toupper(c); }
    char *strtok(char *s, const char *delim) { Util2 u; return u.strtok(s, delim); }
#ifndef _STDLIB_H
    void srand(unsigned seed) { }
    int rand() { return 5; }
    uint32_t be32toh(uint32_t value) { Util2 u; return u.be32toh(value); }
#endif

    bool regex_search(const char *s, const regex &rx)
    { regex_functions r; return r.regex_search(s, rx); }
}

