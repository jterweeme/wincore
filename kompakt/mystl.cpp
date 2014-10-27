#include "mystl.h"

void *Util2::memcpy(void *dest, const void *src, size_t n)
{
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;

    while (n--)
        *dst8++ = *src8++;

    return dest;
}

char *Util2::strtok(char *str, const char *delimiters)
{
    static char *sp = NULL;
    int i = 0;
    int len = strlen(delimiters);
 
    if(len == 0)
        printf("delimiters are empty\n");
 
    if(!str && !sp)
        return NULL;
 
    if(str && !sp)
        sp = str;
 
    char* p_start = sp;

    while(true)
    {
        for(i = 0; i < len; i ++)
        {
            if(*p_start == delimiters[i])
            {
                p_start ++;
                break;
            }
        }
 
        if (i == len)
        {
            sp = p_start;
            break;
        }
    }
 
    if(*sp == '\0')
    {
        sp = NULL;
        return sp;
    }
 
    while(*sp != '\0')
    {
        for(i = 0; i < len; i ++)
        {
            if(*sp == delimiters[i])
            {
                *sp = '\0';
                break;
            }
        }
 
        sp ++;
        if (i < len)
            break;
    }
    return p_start;
}

Util2::size_t Util2::strlen(const char *s)
{
    const char *t;
    for (t = s; *t; ++t) { }
    return (t - s);
}

void *Util2::memset(void *s, int c, size_t n)
{
    uint8_t *p = (uint8_t *)s;

    while (n--)
        *p++ = (uint8_t)c;

    return s;
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

void istream2::read(char *s, size_t length)
{
    _lastRead = fread(s, 1, length, _fp);
    _pos += _lastRead;
    _eof = _lastRead < length;
}

void istream2::getline(char *dest, size_t size)
{
    for (int pos = 0, c = 1; c != '\n'; pos++)
    {
        c = get();

        if (c == EOF)
        {
            _eof = true;
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

void ofstream2::open(const char *fn, openmode om)
{
    switch (om)
    {
    case out:
        _fp = fopen(fn, "wb+");
        break;
    default:
        _fp = fopen(fn, "wb+");
        break;
    }
}

ostream2& ostream2::operator << (const uint32_t u)
{
    char a[] = "%08x";
    char d[] = "%8d";
    a[2] = _width.size() + '0';
    d[1] = _width.size() + '0';

    switch (_base.type())
    {
    case base2::HEX:
        fprintf(_fp, a, u);
        break;
    default:
        fprintf(_fp, d, u);
        break;
    }
    fflush(_fp);
    _width.size(1);
    return *this;
}

int Util2::strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2++)
        if (*s1++ == 0)
            return (0);
    return (*(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1));
}

int Util2::strncmp(const char *s1, const char *s2, size_t n)
{
    for ( ; n > 0; s1++, s2++, --n)
        if (*s1 != *s2)
            return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
        else if (*s1 == '\0')
            return 0;
    return 0;
}

char *Util2::strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';

    return dest;
}

char *Util2::strcpy(char *dest, const char *src)
{
    char *save = dest;
    while ((*dest++ = *src++));
    return save;
}

namespace mystl
{
    void *memcpy(void *dest, const void *src, size_t n) { Util2 u; return u.memcpy(dest, src, n); }
    void *memset(void *s, const int c, const size_t n) { Util2 u; return u.memset(s, c, n); }
    char *strcpy(char *dest, const char *src) { Util2 util; return util.strcpy(dest, src); }
    int strncmp(const char *s1, const char *s2, size_t n) { Util2 u; return u.strncmp(s1, s2, n); }
    int strcmp(const char *s1, const char *s2) { Util2 u; return u.strcmp(s1, s2); }
    size_t strlen(const char *s) { Util2 u; return u.strlen(s); }
    base2 hex(base2::HEX);
    base2 dec(base2::DEC);
    istream cin(stdin);
    ostream cout(stdout);
    ostream cerr(stderr);
    align right;
    width2 setw(int length) { return width2(length); }
    int toupper(int c) { Util2 u; return u.toupper(c); }
    char *strtok(char *s, const char *delim) { Util2 u; return u.strtok(s, delim); }
}

