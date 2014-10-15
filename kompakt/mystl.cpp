#include "mystl.h"

void *Util2::memcpy(void *dest, const void *src, size_t n)
{
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;

    while (n--)
        *dst8++ = *src8++;

    return dest;
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

ostream2& ostream2::write(const char *s, int n)
{
    for (int i = 0; i < n; i++)
        fputc(s[i], _fp);
        
    return *this;
}

ostream2& ostream2::operator << (const uint32_t u)
{
    switch (_base.type())
    {
    case base2::HEX:
        fprintf(_fp, "%08x", u);
        break;
    default:
        fprintf(_fp, "%d", u);
        break;
    }
    fflush(_fp);
    return *this;
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
    char *strcpy(char *dest, const char *src) { Util2 util; return util.strcpy(dest, src); }
}

