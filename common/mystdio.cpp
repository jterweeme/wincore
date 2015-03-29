#include "mystdio.h"

FILE *stdin;
FILE *stdout;
FILE *stderr;

unsigned int strlen2(const char *s)
{
    const char *t;
    for (t = s; *t; ++t) { }
    return t - s;
}

#if 0
int strcmp2(const char *s1, const char *s2)
{
    while (*s1 == *s2++) if (*s1++ == 0) return 0;
    return (*(const uint8_t *)s1 - *(const uint8_t *)(s2 - 1));
}
#endif

int fputc(int c, FILE *stream)
{
    char c1 = c;
    print2(&c1, 1, *stream);
    return c;
}

int puts(const char *s)
{
    print2(s, strlen2(s), 1);
    return 0;
}

int sprintf(char *str, const char *fmt, ...)
{
    return 0;
}

int snprintf(char *str, int size, const char *fmt, ...)
{
    return 0;
}

int printf(const char *fmt, ...)
{
    return 0;
}

FILE *fopen(const char *fn, const char *mode)
{
#if 0
    if (strcmp(mode, "r"))
        return 1;
#endif

    return 0;
}

unsigned fread(void *ptr, unsigned size, unsigned nmemb, FILE *stream)
{
    return 0;
}

int fgetc(FILE *stream)
{
    return 0;
}

long ftell(FILE *stream)
{
    return 0;
}

int fclose(FILE *fp)
{
    return 0;
}

int fprintf(FILE *stream, const char *fmt, ...)
{
    return 0;
}

int fflush(FILE *stream)
{
    return 0;
}

int scanf(const char *fmt, ...)
{
    return 0;
}




