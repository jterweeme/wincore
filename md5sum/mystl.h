#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stdio.h>

class MyUtil
{
public:
    typedef unsigned int size_t;
    typedef unsigned char uint8_t;
    typedef unsigned short int uint16_t;
#ifdef __WATCOMC__
    typedef unsigned long uint32_t;
#else
    typedef unsigned uint32_t;
#endif
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    char *strcpy(char *dest, const char *src);
    char *strncpy(char *dest, const char *src, size_t n);
    size_t strlen(const char *s);
    int atoi(const char *str);
    long int strtol(const char *s, char **end, int base);
    int isdigit(int c) { return c >= '0' && c <= '9'; }
    int isspace(int c) { return c == ' '; }
    int isupper(int c) { return c >= 'A' && c <= 'Z'; }
    int islower(int c) { return c >= 'a' && c <= 'z'; }
    int isalpha(int c) { return isupper(c) || islower(c); }
    int isxdigit(int c) { return isdigit(c) || c >=  'a' && c <= 'f' || c >= 'A' && c <= 'F'; }
    int xdigit(int c);
    template <typename T> T swap_endian(T u);
#ifdef __WATCOMC__
    uint32_t be32toh(uint32_t v)
    { return (v >> 24) | ((v << 8) & 0x00ff0000) | ((v >> 8) & 0x0000ff00) | (v << 24); }
#else
    uint32_t be32toh(uint32_t v) { return swap_endian<uint32_t>(v); }
#endif
};

#ifndef __WATCOMC__
template <typename T> T MyUtil::swap_endian(T u)
{
    union foo
    {
        T u;
        uint8_t u8[sizeof(T)];
    };

    foo source, dest;
    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}
#endif

class ios2
{
public:
    static const MyUtil::uint8_t binary = 1;
};

class istream2
{
    typedef MyUtil::uint16_t uint16_t;
protected:
    FILE *_fp;
    size_t _lastRead;
    bool _eof;
public:
    istream2() : _lastRead(0), _eof(false) { }
    istream2(FILE *fp) : _fp(fp), _lastRead(0), _eof(false) { }
    virtual ~istream2() { }
    uint16_t tellg() { return ftell(_fp); }
    uint16_t gcount() { return _lastRead; }
    operator void * () const { return (void *)!_eof; }
    void getline(char *dest, size_t size);
    virtual void read(char *s, size_t length);
};

class ifstream2 : public istream2
{
    typedef MyUtil::uint8_t uint8_t;
public:
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    ifstream2() { }
    ifstream2(char *s, int m) : istream2(fopen(s, "rb")) { }
    virtual ~ifstream2() { }
    void open(const char *fn, int mode) { _fp = fopen(fn, "rb"); }
    void close() { fclose(_fp); }
};

class string2
{
    char _s[255];
public:
    string2() { }
    string2(const char *s) { MyUtil util; util.strcpy(_s, s); }
    string2(const char *s1, const char *s2) { MyUtil util; util.strncpy(_s, s1, s2 - s1); }
    const char *c_str() const { return _s; }
};

class dummy
{
};

class Hex
{
};

class ostream2
{
    typedef MyUtil::uint8_t uint8_t;
    typedef MyUtil::uint32_t uint32_t;
protected:
    uint8_t _mode;
    FILE *_fp;
public:
    static const uint8_t DEC = 0;
    static const uint8_t OCT = 1;
    static const uint8_t HEX = 2;
    ostream2() : _mode(DEC) { }
    ostream2(FILE *fp) : _mode(DEC), _fp(fp) { }
    ostream2& operator << (string2 s) { fprintf(_fp, s.c_str()); return *this; }
    ostream2& operator << (const char *s) { fprintf(_fp, s); fflush(_fp); return *this; }
    virtual ostream2& operator << (dummy &d) { return *this; }
    virtual ostream2& operator << (Hex &hex) { return *this; }
    virtual ostream2& operator << (const uint32_t u);
    virtual ~ostream2() { }
};

class fstream2
{
public:
    static const MyUtil::uint8_t in = 1;
    static const MyUtil::uint8_t binary = 2;
};

#ifdef __USE_XOPEN2K8
class ostringstream2 : public ostream2
{
    char *_buf;
    size_t _size;
public:
    ostringstream2() : ostream2(open_memstream(&_buf, &_size)) { }
    string2 str() { return string2(_buf); }
    ~ostringstream2() { }
};
#endif

class ostringstream3 : public ostream2
{
public:
    string2 str() { return string2("onzin"); }
};

template <class T> class vector2
{
    T _buf[100];        // uiteraard moet dit anders
    size_t _capacity;
    size_t _size;
public:
    typedef T * iterator;
    vector2() : _capacity(100), _size(0) {  }
    void push_back(T a) { _buf[_size++] = a; }
    iterator begin() { return _buf; }
    iterator end() { return _buf + _size; }
    virtual ~vector2() { }
};


namespace mystl
{
    typedef MyUtil::size_t size_t;
    typedef MyUtil::uint8_t uint8_t;
    typedef MyUtil::uint16_t uint16_t;
    typedef MyUtil::uint32_t uint32_t;
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    typedef string2 string;
    unsigned long stoul(const string &str, size_t *idx = 0, int base = 10);
    template <class T> class vector : public vector2<T> { };
    typedef ios2 ios;
    typedef istream2 istream;
    typedef ifstream2 ifstream;
    typedef ostream2 ostream;
#ifdef __USE_XOPEN2K8
    typedef ostringstream2 ostringstream;
#else
    typedef ostringstream3 ostringstream;
#endif
    typedef fstream2 fstream;
    static istream cin(stdin);
    static ostream cout(stdout);
    static ostream cerr(stderr);
    extern dummy dummy1;
    extern Hex hex;
    extern dummy& setw(int length);
    extern dummy& setfill(int length);
}

#endif


