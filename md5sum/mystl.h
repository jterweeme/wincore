#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <string>

namespace mytypes
{
    typedef unsigned int size_t;
    typedef unsigned char uint8_t;
    typedef unsigned uint32_t;
}

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
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    unsigned long stoul(const string &str, size_t *idx = 0, int base = 10);
}

#endif


