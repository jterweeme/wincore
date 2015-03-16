#include "common.h"

void *Util2::memcpy(void *dest, const void *src, size_t n)
{
    char *dst8 = (char *)dest;
    char *src8 = (char *)src;
    while (n--) *dst8++ = *src8++;
    return dest;
}

size_t Time2::strftime(char *p, size_t max, const char *fmt, const tm *tp) const
{
    if (strcmp(fmt, "%F %T") == 0)
        snprintf(p, max, "%u-%02u-%02u %02u:%02u:%02u",
            tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday, tp->tm_hour, tp->tm_min, tp->tm_sec);

    if (strcmp(fmt, "%F %R") == 0)
        snprintf(p, max, "%u-%02u-%02u %02u:%02u",
            tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday, tp->tm_hour, tp->tm_min);

    return 0;
}

Util2::streampos fpinbuf::seekoff(int64_t off, seekdir way, openmode m)
{
    uint32_t pb = ftell(_fp) > _lastRead ? 8 : 0;
    return (uint64_t)gptr() - (uint64_t)eback() + ftell(_fp) - _lastRead - pb;
}

Util2::streamsize streambuf2::in_avail()
{
    uint64_t a = (uint64_t)egptr() - (uint64_t)gptr(); return a == 0 ? showmanyc() : a;
}

#if 0
bool UnixTime::isDST() const
{
    if (_yday > 50 && _yday < 298)
        return true;

    return false;
}
#endif

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

bool Time2::isLeap(uint32_t year) const
{
    switch (year)
    {
    case 72:
    case 76:
    case 80:
    case 84:
    case 88:
    case 92:
    case 96:
    case 100:
    case 104:
    case 108:
    case 112:
    case 116:
        return true;
    }

    return false;
}

#if 0
bool UnixTime::isLeap(uint32_t year) const
{
    switch (year)
    {
    case 1972:
    case 1976:
    case 1980:
    case 1984:
    case 1988:
    case 1992:
    case 1996:
    case 2000:
    case 2004:
    case 2008:
    case 2012:
    case 2016:
        return true;
    }

    return false;
}
#endif

int Time2::daysInMonth(uint32_t month) const
{
    switch (month)
    {
    case 1:
        return isLeap(_tm.tm_year) ? 29 : 28;
    case 3:
    case 5:
    case 8:
    case 10:
        return 30;
    default:
        return 31;
    }
}

#if 0
uint32_t UnixTime::daysInMonth(uint32_t month) const
{
    switch (month)
    {
    case 1:
        return isLeap(_year) ? 29 : 28;
    case 3:
    case 5:
    case 8:
    case 10:
        return 30;
    default:
        return 31;
    }
}

void UnixTime::exportTM(tm &t)
{
    t.tm_year = year() - 1900;
    t.tm_yday = yday();
    t.tm_mon = mon();
    t.tm_mday = day();
    t.tm_hour = hour();
    t.tm_min = min();
    t.tm_sec = sec();
}
#endif

void Time2::set(int y, int yd, int mon, int d, int h, int min, int s)
{
    _tm.tm_year = y;
    _tm.tm_yday = yd;
    _tm.tm_mon = mon;
    _tm.tm_mday = d;
    _tm.tm_hour = h;
    _tm.tm_min = min;
    _tm.tm_sec = s;
}

#if 0
void UnixTime::incSec()
{
    if (++_sec < 60) return;
    _sec = 0;
    if (++_min < 60) return;
    _min = 0;
    incHour();
}

void UnixTime::incHour()
{
    if (++_hour < 24) return;
    _hour = 0;
    incDay();
}
#endif

void Time2::incSec()
{
    if (++_tm.tm_sec < 60) return;
    _tm.tm_sec = 0;
    if (++_tm.tm_min < 60) return;
    _tm.tm_min = 0;
    incHour();
}

void Time2::incHour()
{
    if (++_tm.tm_hour < 24) return;
    _tm.tm_hour = 0;
    incDay();
}

void Time2::incDay()
{
    _tm.tm_yday++;
    if (++_tm.tm_mday <= daysInMonth(_tm.tm_mon)) return;
    _tm.tm_mday = 1;
    if (++_tm.tm_mon < 12) return;
    _tm.tm_yday = 0;
    _tm.tm_mon = 0;
    _tm.tm_year++;
}

#if 0
void UnixTime::incDay()
{
    ++_yday;
    if (++_day <= daysInMonth(_mon)) return;
    _day = 1;
    if (++_mon < 12) return;
    _yday = 0;
    _mon = 0;
    _year++;   
}

void UnixTime::incSec(uint32_t s)
{
    for (; s >= 86400; s -= 86400)
        incDay();

    for (; s >= 3600; s -= 3600)
        incHour();

    for (uint32_t i = 0; i < s; i++)
        incSec();
}
#endif

void Time2::incSec(uint32_t s)
{
    for (; s>= 86400; s -= 86400)
        incDay();

    for (uint32_t i = 0; i < s; i++)
        incSec();
}

tm Time2::_tm;

tm *Time2::gmtime(const time_t *timer)
{
#if 1
    set(*timer);
    return &_tm;
#else
    UnixTime ut;
    ut.set(*timer);
    ut.exportTM(_tm);
    return &_tm;
#endif
}

tm *Time2::localtime(const time_t *timer)
{
#if 1
    set(*timer);
    if (isDST()) incHour();
    incHour();
    return &_tm;
    return gmtime(timer);
#else
    UnixTime ut;
    ut.set(*timer);

    if (ut.isDST())
        ut.incSec(3600);

    ut.incSec(3600);
    ut.exportTM(_tm);
    return &_tm;
#endif
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

Util2::size_t Util2::strlen(const char *s)
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

filebuf2 *filebuf2::open(const char *fn, ios::openmode m)
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

void ofstream2::open(const char *fn, openmode om)
{
#if 0
    switch (om)
    {
    case out:
        _fp = fopen(fn, "wb+");
        break;
    default:
        _fp = fopen(fn, "wb+");
        break;
    }
#endif
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

int Util2::strcmp(const char *s1, const char *s2) const
{
    while (*s1 == *s2++)
        if (*s1++ == 0)
            return (0);

    return (*(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1));
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
}

