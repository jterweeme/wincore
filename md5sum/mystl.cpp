#include "mystl.h"

void *MyUtil::memcpy(void *dest, const void *src, size_t n)
{
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;

    while (n--)
        *dst8++ = *src8++;

    return dest;
}

void *MyUtil::memset(void *s, const int c, size_t n)
{
    uint8_t *p = (uint8_t *)s;

    while (n--)
        *p++ = (uint8_t)c;

    return s;
}

int MyUtil::atoi(const char *str)
{
    int res = 0;

    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';

    return res;
}

int MyUtil::xdigit(int c)
{
    if (!isxdigit(c))
        return -1;

    if (isdigit(c))
        return c - '0';

    if (isupper(c))
        return c - 0x37;

    if (islower(c))
        return c - 0x57;

    return -1;
}


long int MyUtil::strtol(const char *nptr, char **endptr, int base)
{
    MyUtil util;
    long int result = xdigit(nptr[0]);
    
    for (int i = 1; util.isxdigit(nptr[i]); i++)
    {
        result <<= 4;
        result += xdigit(nptr[i]);
    }

    return result;
}

void istream2::read(char *s, size_t length)
{
    _lastRead = fread(s, 1, length, _fp);
    _eof = _lastRead < length;
}

void istream2::getline(char *dest, size_t size)
{
    for (int pos = 0, c = 1; c != '\n'; pos++)
    {
        c = fgetc(_fp);

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

MyUtil::size_t MyUtil::strlen(const char *s)
{
    const char *t;
    for (t = s; *t; ++t) { }
    return (t - s);
}

char *MyUtil::strcpy(char *dest, const char *src)
{
    char *save = dest;
    while ((*dest++ = *src++));
    return save;
}

char *MyUtil::strncpy(char *dest, const char *src, const size_t n)
{
    size_t i;

    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';

    return dest;
}

namespace mystl
{
    void *memcpy(void *dest, const void *src, size_t n)
    {
        MyUtil util;
        return util.memcpy(dest, src, n);
    }

    void *memset(void *s, const int c, const size_t n)
    {
        MyUtil util;
        return util.memset(s, c, n);
    }

    unsigned long stoul(const string &str, size_t *idx, int base)
    {
        MyUtil util;
        return util.strtol(str.c_str(), 0, 16);
    }

    dummy dummy1;
    dummy hex;
    dummy& setw(int length) { return dummy1; }
    dummy& setfill(int length) { return dummy1; }
}


