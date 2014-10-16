#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stdio.h>
#include <vector>

class Util2
{
public:
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned uint32_t;
    typedef long unsigned size_t;
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    char *strcpy(char *dest, const char *src);
    char *strncpy(char *dest, const char *src, size_t n);
    size_t strlen(const char *s);
};

template <Util2::size_t T> class bitset
{
    typedef Util2::uint32_t uint32_t;
    typedef Util2::size_t size_t;
    uint32_t _set;
public:
    bitset(uint32_t val) : _set(val) { }
    size_t size() const { return T; }
    bool test(size_t pos) const { return _set & (1 << pos); }
    bitset& set() { _set = 0xffffffff; return *this; }
    bitset& set(size_t pos) { _set |= (1<<pos); return *this; }
    bitset& reset() { _set = 0; return *this; }
    bitset& reset(size_t pos) { _set &= ~(1<<pos); return *this; }
    bool any() const { return _set != 0; }
    bool none() const { return _set == 0; }
    size_t count() const { size_t x = 0; for (size_t i = 0; i < T; i++) x += test(i); return x; }
};

class string2
{
    char _s[255];
    typedef Util2::size_t size_t;
public:
    string2() { Util2 u; u.memset(_s, 0, sizeof(_s)); }
    string2(const char *s) { Util2 util; util.strcpy(_s, s); }
    string2(const char *s1, const char *s2) { Util2 util; util.strncpy(_s, s1, s2 - s1); }
    string2(size_t n, char c) { _s[n] = '\0'; while (n) _s[--n] = c; }
    const char *c_str() const { return _s; }
    size_t length() const { Util2 util; return util.strlen(_s); }
};

class istream2
{
    typedef Util2::uint16_t uint16_t;
protected:
    FILE *_fp;
    size_t _lastRead;
    bool _eof;
public:
    istream2() : _lastRead(0), _eof(false) { }
    istream2(FILE *fp) : _fp(fp), _lastRead(0), _eof(false) { }
    virtual ~istream2() { }
    istream2& ignore(int n = 1, int d = '\n') { fseek(_fp, n, SEEK_CUR); return *this; }
    uint16_t tellg() { return ftell(_fp); }
    uint16_t gcount() { return _lastRead; }
    operator void * () const { return (void *)!_eof; }
    void getline(char *dest, size_t size);
    virtual void read(char *s, size_t length);
};

class fstream
{
    typedef Util2::uint8_t uint8_t;
public:
    static const uint8_t in = 1;
};

class ifstream2 : public istream2
{
    typedef Util2::uint8_t uint8_t;
    bool _open;
public:
    bool is_open() const { return _open; }
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    ifstream2() : _open(false) { }
    ifstream2(char *s, int m) : istream2(fopen(s, "rb")), _open(true) { }
    void open(const char *fn, int mode = 1) { _fp = fopen(fn, "rb"); _open = true; }
    void close() { if (_open) { fclose(_fp); _open = false; } }
    virtual ~ifstream2() { close(); }
};

class align
{
};

class dummy
{
};

class base2
{
    typedef Util2::uint8_t uint8_t;
    uint8_t _type;
public:
    static const uint8_t DEC = 1;
    static const uint8_t HEX = 2;
    static const uint8_t OCT = 3;
    base2(uint8_t type) : _type(type) { }
    base2() : _type(DEC) { }
    uint8_t type() const { return _type; }
};

class ostream2
{
    typedef Util2::uint8_t uint8_t;
    typedef Util2::uint32_t uint32_t;
protected:
    uint8_t _mode;
    base2 _base;
    FILE *_fp;
public:
    static const uint8_t DEC = 0;
    static const uint8_t OCT = 1;
    static const uint8_t HEX = 2;
    ostream2() : _base(base2::DEC) { }
    ostream2(FILE *fp) : _base(base2::DEC), _fp(fp) { }
    ostream2& operator << (string2 s) { fprintf(_fp, s.c_str()); return *this; }
    ostream2& operator << (const char *s) { fprintf(_fp, s); fflush(_fp); return *this; }
    ostream2& operator << (align &a) { return *this; }
    ostream2& operator << (dummy &d) { return *this; }
    ostream2& operator << (base2 &base) { _base = base; return *this; }
    virtual ostream2& operator << (const uint32_t u);
    ostream2& write(const char *s, int n);
    virtual ~ostream2() { }
};

class ofstream2 : public ostream2
{
public:
    void open(const char *fn) { _fp = fopen(fn, "rb"); }
    void close() { }
};

class ostringstream2 : public ostream2
{
public:
    string2 str() const { return string2("hello"); }
};

namespace mystl
{
    using std::vector;
    typedef Util2::uint8_t uint8_t;
    typedef Util2::uint16_t uint16_t;
    typedef Util2::uint32_t uint32_t;
    void *memcpy(void *dest, const void *src, size_t n);
    char *strcpy(char *dest, const char *src);
    typedef string2 string;
    typedef istream2 istream;
    typedef ostream2 ostream;
    typedef ofstream2 ofstream;
    typedef ifstream2 ifstream;
    typedef ostringstream2 ostringstream;
    static istream cin(stdin);
    static ostream cout(stdout);
    static ostream cerr(stdout);
    static const char endl[] = "\n";
    static align right;
    static dummy dummy1;
    static dummy& setw(int length) { return dummy1; }
};

#endif


