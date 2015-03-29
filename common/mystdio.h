#ifndef _MYSTDIO_H_
#define _MYSTDIO_H_
#include "linux64.h"
#include "mytypes.h"

const unsigned SEEK_SET = 0;
const unsigned SEEK_CUR = 1;
const unsigned SEEK_END = 2;
const int EOF = -1;
typedef int FILE;
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
#define NULL 0
unsigned int strlen2(const char *s);
int puts(const char *s);
int fputc(int c, FILE *stream);
int fgetc(FILE *stream);
int snprintf(char *str, int size, const char *fmt, ...);
int sprintf(char*str, const char *fmt, ...);
int printf(const char *fmt, ...);
FILE *fopen(const char *fn, const char *mode);
unsigned fread(void *ptr, unsigned size, unsigned nmemb, FILE *stream);
int fclose(FILE *fp);
long ftell(FILE *stream);
int fprintf(FILE *stream, const char *fmt, ...);
int fflush(FILE *stream);
int scanf(const char *fmt, ...);
#endif


