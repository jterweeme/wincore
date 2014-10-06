#ifndef _MYSTL_H_
#define _MYSTL_H_

class MyUtil
{
    typedef unsigned int size_t;
    typedef unsigned char uint8_t;
public:
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
};

namespace mystl
{
    typedef unsigned int size_t;
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
}

#endif


