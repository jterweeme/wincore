#include "util2.h"

void *Util2::memcpy(void *dest, const void *src, size_t n)
{
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;
    while (n--) *dst8++ = *src8++;
    return dest;
}

char *Util2::strcat(char *dest, const char *src) const
{
    size_t len = strlen(dest);
    strcpy(dest + len, src);
    return dest;
}

char *Util2::strstr(const char *haystack, const char *needle) const
{
    if (!*needle) return (char *)haystack;
    char *p1 = (char*)haystack;

    while (*p1)
    {
        char *p1Begin = p1, *p2 = (char*)needle;

        while (*p1 && *p2 && *p1 == *p2)
        {
            p1++;
            p2++;
        }

        if (!*p2)
            return p1Begin;

        p1 = p1Begin + 1;
  }
  return 0;
}

char *Util2::strchr(const char *s, int c) const
{
    while (*s != (char)c)
        if (!*s++)
            return 0;

    return (char *)s;
}

Util2::uint8_t Util2::ctoi(char c)
{
    return isdigit(c) == 1 ? c - '0' : c - 87;
}

Util2::uint32_t Util2::upow(uint32_t base, uint32_t exp)
{
    uint32_t result = base;
    for (uint32_t i = 1; i < exp; i++) result *= base;
    return exp > 0 ? result : 1;
}

Util2::uint32_t Util2::strtol(const char *a, const char *b, int base)
{
    uint32_t result = 0;
    for (uint32_t i = strlen(a), j = 0; i > 0; i--, j++) result += ctoi(a[i - 1]) * upow(base, j);
    return result;
}

void *Util2::memmove(char *dst, const char *src, uint32_t n)
{
    if (dst <= src) return memcpy(dst, src, n);
    src += n;
    dst += n;
    while (n--) *--dst = *--src;
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

    if (num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

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
    
    if (isNegative) str[i++] = '-';
    str[i] = '\0';
    reverse(str, i);
    return str;
}

Util2::size_t Util2::strlen(const char *s) const
{
    const char *t;
    for (t = s; *t; ++t) { }
    return (t - s);
}

char *Util2::strtok(char *str, const char *delimiters)
{
    static char *sp = 0;
    int i = 0;
    int len = strlen(delimiters);
    if(len == 0) throw "delimiters are empty";
    if(!str && !sp) return 0;
    if(str && !sp) sp = str;
    char* p_start = sp;

    while (true)
    {
        for (i = 0; i < len; i ++)
        {
            if (*p_start == delimiters[i])
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
        sp = 0;
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

int Util2::strcmp(const char *s1, const char *s2) const
{
    while (*s1 == *s2++) if (*s1++ == 0) return 0;
    return (*(const uint8_t *)s1 - *(const uint8_t *)(s2 - 1));
}

int Util2::strncmp(const char *s1, const char *s2, size_t n) const
{
    for ( ; n > 0; s1++, s2++, --n)
        if (*s1 != *s2)
            return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
        else if (*s1 == '\0')
            return 0;
    return 0;
}

char *Util2::strncpy(char *dest, const char *src, size_t n) const
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) dest[i] = src[i];
    for (; i < n; i++) dest[i] = '\0';
    return dest;
}

char *Util2::strcpy(char *dest, const char *src) const
{
    char *save = dest;
    while ((*dest++ = *src++));
    return save;
}

#ifndef _STDLIB_H
Util2::uint32_t Util2::be32toh(uint32_t v) const
{
    uint32_t r = 0;
    r |= (v & 0x000000ff) << 24;
    r |= (v & 0x0000ff00) << 8;
    r |= (v & 0x00ff0000) >> 8;
    r |= (v & 0xff000000) >> 24;
    return r;
}
#endif

void *Util2::memset(void *s, int c, size_t n)
{
    uint8_t *p = (uint8_t *)s;
    while (n--) *p++ = (uint8_t)c;
    return s;
}

namespace mystl
{
    char *strchr(const char *s, int c) { Util2 u; return u.strchr(s, c); }
    char *strcat(char *dest, const char *src) { Util2 u; return u.strcat(dest, src); }
    char *strstr(const char *hs, const char *needle) { Util2 u; return u.strstr(hs, needle); }
    int isdigit(int c) { Util2 u; return u.isdigit(c); }
    int isspace(int c) { Util2 u; return u.isspace(c); }
    int isprint(int c) { Util2 u; return u.isprint(c); }
}




