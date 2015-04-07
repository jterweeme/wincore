#ifndef _MYSTDIO_H_
#define _MYSTDIO_H_
#include "linux64.h"
#include "util2.h"



#ifndef _UNISTD_H
const unsigned SEEK_SET = 0;
const unsigned SEEK_CUR = 1;
const unsigned SEEK_END = 2;
#endif
const int EOF = -1;
typedef int FILE;
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

const unsigned O_RDONLY = 1;
const unsigned O_RWONLY = 2;
const unsigned O_RDWR = 4;

class MyStdio : public Util2
{
    unsigned int strlen2(const char *s) const;
    int _tiep3b(int rdi, int rax) const { return ::tiep3(rdi, 0, 0, rax); }
    FILE _files[10] = {0};
    int _getFileSlot() { for (int i = 0; i < 10; i++) if (_files[i] == 0) return i; return -1; }
public:
    int open(const char *fn, int flags, int mode) { return ::tiep2(fn, flags, mode, 2); }
    int close(int fd) const { return _tiep3b(fd, 3); }
    int fstat(int fd, stat2 *buf) const { return ::tiep5(fd, (void *)buf, 0, 5); }
    size_t read(int fd, void *buf, size_t n) const { return ::tiep5(fd, buf, n, 0); }
    size_t write(int fd, const void *buf, size_t n) const { return ::tiep6(fd, buf, n, 1); }
    int fputc(int c, FILE *st) { char c1 = c; return write(*st, (const char *)&c1, 1); }
    int fputs(const char *s, FILE *str) const { return write(*str, (const void *)s, strlen(s)); }
    int puts(const char *s) const { return fputs(s, stdout); }
    int snprintf(char *s, int size, const char *fmt) const;
    int fprintf(FILE *stream, const char *s) const { return fputs(s, stream); }
    int printf(const char *s) const { return puts(s); }
    FILE *fopen(const char *fn, const char *mode);
    int fgetc(FILE *st) const { char buf[10] = {0}; read(*st, (char *)buf, 1); return buf[0]; }
    size_t fread(void *ptr, size_t size, size_t n, FILE *st) { return read(*st, ptr, n * size); }
    long ftell(FILE *stream) const { return 0; }
    int fclose(FILE *fp) const { close(*fp); *fp = 0; return 0; }
    int fflush(FILE *stream) const { return 0; }
    int fseek(FILE *stream, long off, int whence) const { return tiep1(*stream, off, whence, 8); }
    
    template <typename... Args> int printf(const char *s, Args... args)
    {
        return puts(s);
    }
};

extern MyStdio m;

int fputs(const char *s, FILE *stream);
int puts(const char *s);
int fputc(int c, FILE *stream);
int fgetc(FILE *stream);
FILE *fopen(const char *fn, const char *mode);
unsigned fread(void *ptr, unsigned size, unsigned nmemb, FILE *stream);
int fclose(FILE *fp);
long ftell(FILE *stream);
int fflush(FILE *stream);
int fseek(FILE *stream, long offset, int whence);
template <typename... A> int printf(const char *fmt, A... args) { return m.printf(fmt, args...); }

template <typename... Args> int sprintf(char *str, const char *fmt, Args... args)
{
    return 0;
}

template <typename... Args> int scanf(const char *fmt, Args... args)
{
    return 0;
}

#endif




