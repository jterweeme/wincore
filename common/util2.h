#ifndef _UTIL2_H_
#define _UTIL2_H_
#include "mytypes.h"

class Util2 : public MyTypes
{
public:
    void reverse(char *str, const int length);
    template<typename T> inline void reverse2(T first, T last);
    char *itoa(const uint32_t num, char *str, const int base);
    char *itoa(const int32_t num, char *str, const int base);
    int isdigit(int c) const { return c >= '0' && c <= '9'; }
    int isspace(int c) const { return c == ' '; }
    int isupper(int c) const { return c >= 'A' && c <= 'Z'; }
    int islower(int c) const { return c >= 'a' && c <= 'z'; }
    int isalpha(int c) const { return isupper(c) || islower(c); }
    int isxdigit(int c) { return isdigit(c) || c >=  'a' && c <= 'f' || c >= 'A' && c <= 'F'; }
    int toupper(int c) { return islower(c) ? c - 32 : c; }
    void toupper(const char *src, char *dest) { while (*src) *dest++ = toupper(*src++); }
    void toupper(char *s) { toupper(s, s); }
    char tocupper(int c) { return toupper(c); }
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    void *memmove(char *dst, const char *src, uint32_t n);
    char *strcpy(char *dest, const char *src) const;
    char *strncpy(char *dest, const char *src, size_t n) const;
    size_t strlen(const char *s) const;
    int strcmp(const char* s1, const char *s2) const;
    int strncmp(const char *s1, const char *s2, size_t n) const;
    char *strtok(char *str, const char *delimiters);
    char *strchr(const char *s, int c) const;
    char *strcat(char *dest, const char *src) const;
    char *strstr(const char *haystack, const char *needle) const;
    uint8_t ctoi(char c);
    uint32_t upow(uint32_t base, uint32_t exp);
    uint32_t strtol(const char *a, const char *b, int base);
    static bool smaller(uint32_t a, uint32_t b) { return a < b; }
    template <typename T> inline void swap(T &a, T &b) { T tmp = a; a = b; b = tmp; }
    template <typename T> void iter_swap(T a, T b) { return swap(*a, *b); }
    template <typename T> inline const T &min(const T &a, const T &b) { return b < a ? b : a; }
    template <typename T> inline const T &max(const T &a, const T &b) { return a < b ? b : a; }
    template <typename T, typename U> void sort(T first, T last, U comp);
    template <typename T> void sort(T first, T last) { sort(first, last, smaller); }
    template <typename T> inline T* addressof(T& r);
    template <typename T, typename T2> void construct(T *p, const T2 &value);
    template <typename BI1, typename BI2> BI2 copy_move_back_a(BI1 f, BI1 l, BI2 result);
    template <typename T, typename U> void destroy(T first, T last, U &alloc);
    template <typename T, typename T2> T2 uninitialized_copy(T first, T last, T2 result);
#ifndef _STDLIB_H
    uint32_t be32toh(uint32_t v) const;
#endif
};

namespace mystl
{
    char *strchr(const char *s, int c);
    char *strcat(char *dest, const char *src);
    char *strstr(const char *haystack, const char *needle);
    int isdigit(int c);
    int isspace(int c);
    int isupper(int c);
    int islower(int c);
    int isalpha(int c);
    int isxdigit(int c);
}

#endif


