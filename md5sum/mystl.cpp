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
}


