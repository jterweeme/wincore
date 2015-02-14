#include "mystl.h"

void *Util2::memcpy(void *dest, const void *src, size_t n)
{
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;

    while (n--)
        *dst8++ = *src8++;

    return dest;
}

void *Util2::memmove(char *dst, const char *src, uint32_t n)
{
    if (dst <= src)
        return memcpy(dst, src, n);

    src += n;
    dst += n;

    while (n--)
        *--dst = *--src;

    return dst;
}

void Util2::reverse(char str[], int length)
{
    for (int start = 0, end = length - 1; start < end;)
        swap(*(str + start++), *(str + end--));
}

char *Util2::itoa(uint32_t num, char *str, int base)
{
    int i = 0;

    while (num != 0)
    {
        uint32_t rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    str[i] = '\0';
    reverse(str, i);
    return str;
}

char *Util2::itoa(int32_t num, char *str, int base)
{
    int i = 0;
    bool isNegative = false;

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';
    reverse(str, i);
    return str;
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

    for (uint32_t i = 0; i < _width.size() - util.strlen(s); i++)
        put(_fill.fill());

    print(s);
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
    char *strncpy(char *d, const char *src, size_t n) { Util2 u; return u.strncpy(d, src, n); }
    int strncmp(const char *s1, const char *s2, size_t n) { Util2 u; return u.strncmp(s1, s2, n); }
    int strcmp(const char *s1, const char *s2) { Util2 u; return u.strcmp(s1, s2); }
    size_t strlen(const char *s) { Util2 u; return u.strlen(s); }
    base2 hex(base2::HEX);
    base2 oct(base2::OCT);
    base2 dec(base2::DEC);
    istream cin(stdin);
    ostream cout(stdout);
    ostream cerr(stderr);
    align right;
    fill2 setfill(char c) { return fill2(c); }
    width2 setw(int length) { return width2(length); }
    int toupper(int c) { Util2 u; return u.toupper(c); }
    char *strtok(char *s, const char *delim) { Util2 u; return u.strtok(s, delim); }
}

