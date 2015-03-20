#ifndef _MYSTDIO_H_
#define _MYSTDIO_H_
#include "linux64.h"

unsigned int strlen(const char *s)
{
    const char *t;
    for (t = s; *t; ++t) { }
    return t - s;
}

int puts(const char *s)
{
    print(s, strlen(s), 1);
    return 0;
}
#endif


