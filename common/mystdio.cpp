#include "mystdio.h"

FILE _stdin = 0;
FILE _stdout = 1;
FILE _stderr = 2;

FILE *stdin = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

MyStdio m;

int MyStdio::snprintf(char *s, int size, const char *fmt) const
{
    for (int i = 0; i < size; i++)
        s[i] = fmt[i];

    return 0;
}

FILE *MyStdio::fopen(const char *fn, const char *mode)
{
    int i = _getFileSlot();
    if (i < 0) throw "Too many open files";
    _files[i] = open(fn, 0, 0);
    return &_files[i];
}

#if 0
int strcmp2(const char *s1, const char *s2)
{
    while (*s1 == *s2++) if (*s1++ == 0) return 0;
    return (*(const uint8_t *)s1 - *(const uint8_t *)(s2 - 1));
}
#endif

int fputc(int c, FILE *stream) { return m.fputc(c, stream); }
int fputs(const char *s, FILE *stream) { return m.fputs(s, stream); }
int puts(const char *s) { return m.puts(s); }
FILE *fopen(const char *fn, const char *mode) { return m.fopen(fn, mode); }
unsigned fread(void *p, unsigned size, unsigned n, FILE *s) { return m.fread(p, size, n, s); }
int fgetc(FILE *stream) { return m.fgetc(stream); }
long ftell(FILE *stream) { return m.ftell(stream); }
int fclose(FILE *fp) { return m.fclose(fp); }
int fflush(FILE *stream) { return m.fflush(stream); }
int fseek(FILE *stream, long off, int whence) { return m.fseek(stream, off, whence); }




