#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <string>
#include <stdio.h>
#include <cstdlib>

class MyUtil
{
public:
    typedef unsigned int size_t;
    typedef unsigned char uint8_t;
    typedef unsigned short int uint16_t;
    typedef unsigned uint32_t;   
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    int atoi(const char *str);
    long int strtol(const char *s, char **end, int base);
    int isdigit(int c) { return c >= '0' && c <= '9'; }
    int isspace(int c) { return c == ' '; }
    int isupper(int c) { return c >= 'A' && c <= 'Z'; }
    int islower(int c) { return c >= 'a' && c <= 'z'; }
    int isalpha(int c) { return isupper(c) || islower(c); }
    int isxdigit(int c) { return isdigit(c) || c >=  'a' && c <= 'f' || c >= 'A' && c <= 'F'; }
    int xdigit(int c);
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
};


namespace mystl
{
    using std::string;
    typedef MyUtil::size_t size_t;
    typedef MyUtil::uint8_t uint8_t;
    typedef MyUtil::uint16_t uint16_t;
    typedef MyUtil::uint32_t uint32_t;
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    unsigned long stoul(const string &str, size_t *idx = 0, int base = 10);
    template <class T> class vector : public vector2<T> { };
    typedef istream2 istream;
    typedef ifstream2 ifstream;
    static istream cin(stdin);
}

#endif


