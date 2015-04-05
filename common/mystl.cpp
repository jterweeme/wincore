#include "common.h"

void *Util2::memcpy(void *dest, const void *src, size_t n)
{
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;
    while (n--) *dst8++ = *src8++;
    return dest;
}

Util2::streamsize streambuf2::in_avail()
{
    uint64_t a = (uint64_t)egptr() - (uint64_t)gptr(); return a == 0 ? showmanyc() : a;
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

char *Util2::strtok(char *str, const char *delimiters)
{
    static char *sp = NULL;
    int i = 0;
    int len = strlen(delimiters); 
    if(len == 0) printf("delimiters are empty\n");
    if(!str && !sp) return NULL;
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

Util2::size_t Util2::strlen(const char *s) const
{
    const char *t;
    for (t = s; *t; ++t) { }
    return (t - s);
}

void *Util2::memset(void *s, int c, size_t n)
{
    uint8_t *p = (uint8_t *)s;
    while (n--) *p++ = (uint8_t)c;
    return s;
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

Util2::streamsize streambuf2::xsgetn(char *s, streamsize length)
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

uint32_t Util2::be32toh(uint32_t v) const
{
    uint32_t r = 0;
    r |= (v & 0x000000ff) << 24;
    r |= (v & 0x0000ff00) << 8;
    r |= (v & 0x00ff0000) >> 8;
    r |= (v & 0xff000000) >> 24;
    return r;
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
    int isdigit(int c) { Util2 u; return u.isdigit(c); }
    void srand(unsigned seed) { }
    int rand() { return 5; }
    uint32_t be32toh(uint32_t value) { Util2 u; return u.be32toh(value); }

    bool regex_search(const char *s, const regex &rx)
    { regex_functions r; return r.regex_search(s, rx); }
}

