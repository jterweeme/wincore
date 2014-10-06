#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <string>
#include <stdio.h>

class MyUtil
{
    typedef unsigned int size_t;
    typedef unsigned char uint8_t;
    typedef unsigned long int uint32_t;   
public:
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    int atoi(const char *str);
    long int strtol(const char *s, char **end, int base);
};

namespace mystl
{
    using std::string;

    typedef unsigned int size_t;
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned uint32_t;

    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    unsigned long stoul(const string &str, size_t *idx = 0, int base = 10);

    class istream
    {
    protected:
        FILE *_fp;
        size_t _lastRead;
        bool _eof;
    public:
        istream() : _lastRead(0), _eof(false) { }
        istream(FILE *fp) : _fp(fp), _lastRead(0), _eof(false) { }
        virtual ~istream() {  }
        uint16_t tellg() { return ftell(_fp); }
        uint16_t gcount() { return _lastRead; }
        operator void * () const { return (void *)!_eof; }


        void getline(char *dest, size_t size)
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

        virtual void read(char *s, size_t length)
        {
            _lastRead = fread(s, 1, length, _fp);
            _eof = _lastRead < length;
        }
    
    };

    class ifstream : public istream
    {
    public:
        static const uint8_t in = 1;
        static const uint8_t binary = 2;
        ifstream() { }
        ifstream(char *s, int m) : istream(fopen(s, "rb")) { }
        ~ifstream() { }
        void open(const char *fn, int mode) { _fp = fopen(fn, "rb"); }
        void close() { fclose(_fp); }
    };

    static istream cin(stdin);
}

#endif


