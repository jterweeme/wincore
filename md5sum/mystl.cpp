#include "mystl.h"
#include <stdlib.h>

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

long int MyUtil::strtol(const char *s, char **end, int base)
{
    return 10;
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
        return strtol(str.c_str(), 0, 16);
    }
}


