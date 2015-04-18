#ifndef _MYSTL_H_
#define _MYSTL_H_

#if 1
#include <cstdio>
#include <cstdlib>
#else
#include "mystdio.h"
#endif

#include "mytime.h"

template <typename T, typename U = int> class ni
{
public:
    T _M_current;
public:
    typedef T iterator_type;
    typedef Util2::ptrdiff_t difference_type;
    ni() : _M_current(T()) { }
    explicit ni(const T &i) : _M_current(i) { }
    template <typename T2> struct itts;
    template <typename T2> struct itts<T2*> { typedef T2 &ref; };
    typename itts<T>::ref operator*() const { return *_M_current; }
    T operator->() const { return _M_current; }
    ni& operator++() { ++_M_current; return *this; }
    ni operator++(int) { return ni(_M_current++); }
    ni operator--() { --_M_current; return *this; }
    ni operator--(int) { return ni(_M_current--); }
    ni& operator+=(difference_type __n) { _M_current += __n; return *this; }
    ni operator+(difference_type n) const { return ni(_M_current + n); }
    ni& operator-=(difference_type __n) { _M_current -= __n; return *this; }
    ni operator-(difference_type n) const { return ni(_M_current - n); }
    const T& base() const { return _M_current; }
};

template <typename T = int, typename U = int> inline bool
operator==(const ni<T>& lhs, const ni<T>& rhs)
{ return lhs.base() == rhs.base(); }

template<typename L = int, typename R = int> bool
operator!=(const ni<L> &l, const ni<R> &r)
{ return l.base() != r.base(); }

template<typename T = int> inline bool operator<(const ni<T> &lhs, const ni<T> &rhs)
{ return lhs.base() < rhs.base(); }

template<typename T = int, typename C = int> inline typename ni<T, C>::difference_type
operator-(const ni<T>& lhs, const ni<T>& rhs)
{ return lhs.base() - rhs.base(); }

template <typename T, bool> struct Iter_base { static T _S_base(T it) { return it; } };
template <typename T> struct Iter_base<T, true> { static T _S_base(T it) { return it.base(); } };
template <typename> struct ini { enum { value = 0 }; };
template <typename T, typename U> struct ini<ni<T, U> > { enum { value = 1 }; };
template <typename> struct imi { enum { value = 0 }; };
template <typename T> struct Nb : Iter_base<T, ini<T>::value> { };
template <typename T> T nb(T it) { return Nb<T>::_S_base(it); }
template <typename T> struct Mb : Iter_base<T, imi<T>::value> { };
template <typename T> T mb(T it) { return Mb<T>::_S_base(it); }

#ifndef _NEW
#define _NEW
inline void* operator new(size_t, void* p) { return p; }
#endif

template<typename T> class allocator2
{
public:
    typedef T *pointer;
    typedef const T *const_pointer;
    allocator2() { }
    template<typename T1> allocator2(const allocator2<T1>&) { }
    ~allocator2() { }
    T *allocate(size_t n) const { return static_cast<T*>(::operator new(n * sizeof(T))); }
    void deallocate(T *p, size_t) const { ::operator delete(p); }
    size_t max_size() const { return size_t(-1) / sizeof(T); }
    void construct(T *p, const T &val) const { ::new((void *)p) T(val); }
    void destroy(T *p) const { p->~T(); }
};

template <typename T> struct alloc_traits
{
    typedef typename T::pointer pointer;
    typedef typename T::const_pointer const_pointer;
    static typename T::pointer allocate(T &a, size_t n) { return a.allocate(n); }
    static void deallocate(T &a, typename T::pointer p, size_t n) { a.deallocate(p, n); }
    template<typename U> static void construct(T &a, pointer p, const U &b) { a.construct(p, b); }
    static void destroy(T &a, pointer p) { a.destroy(p); }
    static size_t max_size(const T &a) { return a.max_size(); }
    static const T &_S_select_on_copy(const T &a) { return a; }
};

template <typename T> struct Vector_impl2 : public allocator2<T>
{
    T *start = 0;
    T *fin = 0;
    T *eos = 0;
    Vector_impl2() { }
    Vector_impl2(allocator2<T> const &a) : allocator2<T>(a) { }
};

template<typename T> struct Vector_base
{
    typedef allocator2<T> A;
    void _M_create_storage(size_t n);
    Vector_impl2<T> _M_impl;
    A& _M_get_Tp_allocator() { return *static_cast<A*>(&_M_impl); }
    const A& _M_get_Tp_allocator() const { return *static_cast<const A*>(&_M_impl); }
    allocator2<T> get_allocator() const { return allocator2<T>(_M_get_Tp_allocator()); }
    Vector_base() : _M_impl() { }
    Vector_base(const allocator2<T> &a) : _M_impl(a) { }
    Vector_base(size_t n) : _M_impl() { _M_create_storage(n); }
    Vector_base(size_t n, const allocator2<T> &a) : _M_impl(a) { _M_create_storage(n); }
    ~Vector_base(){_M_deallocate(_M_impl.start,_M_impl.eos - _M_impl.start); }
    T *_M_allocate(size_t n) { return n != 0 ? alloc_traits<A>::allocate(_M_impl, n) : 0; }
    void _M_deallocate(T *p, size_t n) { if (p) alloc_traits<A>::deallocate(_M_impl, p, n); }
};

template <typename T> class vector2 : public Vector_base<T>
{
    typedef Vector_base<T> Base;
    typedef alloc_traits<typename Vector_base<T>::A> atraits;
protected:
    using Base::_M_allocate;
    using Base::_M_deallocate;
    using Base::_M_impl;
    using Base::_M_get_Tp_allocator;
public:
    typedef ni<T*> iterator;
    typedef ni<const T*> const_iterator;
    void _M_fill_initialize(size_t n, const T &value) { _M_impl.fin = _M_impl.eos; }
    vector2() : Base() { }
    explicit vector2(size_t n) : Base(n, allocator2<T>()) { _M_fill_initialize(n, T()); }
    vector2(const vector2 &x);
    const_iterator begin() const { return const_iterator(_M_impl.start); }
    const_iterator cbegin() const { return const_iterator(_M_impl.start); }
    iterator begin() { return iterator(_M_impl.start); }
    //T &front() { return *begin(); }
    const T &front() { return at(0); }
    ~vector2();
    void assign(size_t n, const T &val) { _M_fill_assign(n, val); }
    iterator end() { return iterator(_M_impl.fin); }
    const_iterator end() const { return const_iterator(_M_impl.fin); }
    const_iterator cend() const { return const_iterator(_M_impl.fin); }
    //T &back() { return *end(); }
    size_t size() const { return size_t(_M_impl.fin - _M_impl.start); }
    T &back() { return at(size() - 1); }
    const T &back() const { return at(size() - 1); }
    size_t max_size() const { return atraits::max_size(_M_get_Tp_allocator()); }
    void reserve(size_t n);
    const T& operator[](size_t n) const { return *(_M_impl.start + n); }
    T& operator[](size_t n) { return *(_M_impl.start + n); }
    void push_back(const T &x);
    void pop_back() { --_M_impl.fin; atraits::destroy(_M_impl, _M_impl.fin); }
    void insert(iterator pos, size_t n, const T &x) { _M_fill_insert(pos, n, x); }
    T &at(size_t n) { return (*this)[n]; }
    const T &at(size_t n) const { return (*this)[n]; }
    void clear() { _M_erase_at_end(_M_impl.start); }
protected:
    void _M_fill_assign(size_t n, const T &val);
    void _M_fill_insert(iterator pos, size_t n, const T &x);
    void _M_insert_aux(iterator position, const T &x);

#if 1
    void _M_erase_at_end(T* pos)
    {
        Util2 u;
        u.destroy(pos, _M_impl.fin, _M_get_Tp_allocator());
        _M_impl.fin = pos;
    }
#endif

    size_t _M_check_len(size_t n, const char * s) const;
};

enum _Ios_Seekdir2
{
    _S_beg = 0,
    _S_cur = SEEK_CUR,
    _S_end = SEEK_END,
    _S_ios_seekdir_end = 1L << 16
};

struct mbstate_t2
{
    int fill[6];
};

template <typename T> class fpos2
{
public:
    fpos2(uint64_t off) { }
};

class ios_base2 : public Util2
{
public:
    typedef int seekdir;
    typedef int openmode;
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    static const uint8_t out = 4;
    static const uint8_t ate = 8;
    static const uint8_t app = 16;
    static const uint8_t trunc = 32;
    static const uint8_t beg = 0;
    static const uint8_t end = 1;
};

namespace mystl
{
    typedef ios_base2 ios_base;
}

class streambuf2 : public ios_base2
{
    char *_inBeg = 0;
    char *_inCur = 0;
    char *_inEnd = 0;
    char *_outBeg = 0;
    char *_outCur = 0;
    char *_outEnd = 0;
protected:
    streampos _pos = 0;
    char *gptr() const { return _inCur; }
    char *egptr() const { return _inEnd; }
    char *eback() const { return _inBeg; }
    void setg(char *beg, char *next, char *end) { _inBeg = beg; _inCur = next; _inEnd = end; }
    void gbump(int n) { _inCur += n; _pos += n; }
    virtual int overflow(int c) { throw "Overflow not implemented"; return 0; }
    virtual int underflow() { throw "Underflow not implemented"; return 0; }
    virtual int uflow() { return underflow() == EOF ? EOF : _bumpy(); }
    virtual streampos seekoff(streamoff, seekdir, openmode) { return _pos; }
    virtual streampos seekpos(streampos sp, openmode which) { return 0; }
    virtual streamsize xsgetn(char *s, streamsize n);
    virtual streamsize showmanyc() { throw "Showmanyc not implemented"; return 0; }
    virtual int sync() { return 0; }
    uint8_t _bumpy() { int ret = sgetc(); gbump(1); return (uint8_t)ret; }
public:
    streamsize in_avail();
    int sputc(char c) { return overflow(c); }
    int sbumpc() { return ((!gptr()) || (gptr() == egptr())) ? uflow() : _bumpy(); }
    int sgetc() { return ((!gptr()) || gptr() == egptr()) ? underflow() : *gptr(); }
    int snextc() { return sbumpc() == EOF ? EOF : sgetc(); }
    int sgetn(char *s, int n) { return xsgetn(s, n); }
    int pubsync() { return sync(); }
    streampos pubseekoff(int64_t off, seekdir way, openmode m) { return seekoff(off, way, m); }
    streampos pubseekpos(streampos pos, openmode which) { return seekpos(pos, which); }
    virtual ~streambuf2() { }
};

namespace mystl
{
    typedef streambuf2 streambuf;
}

class ios2 : public ios_base2
{
protected:
    streambuf2 *_sb;
public:
    streambuf2 *rdbuf() const { return _sb; }
    streambuf2 *rdbuf(streambuf2 *sb) { return _sb = sb; }
    ios2(streambuf2 *sb) : _sb(sb) { }
    ios2() { }
    bool eof() const { return _sb->in_avail() == -1; }
};

namespace mystl
{
    typedef ios2 ios;
}

class fpinbuf : public streambuf2
{
protected:
    FILE *_fp;
    const uint32_t _put_back = 8;
    char _buffer[264];
    streamsize _lastRead = 0;
    bool _eof = false;
public:
    fpinbuf *open(FILE *fp) { _fp = fp; return this; }
    virtual int underflow();
    virtual streamsize showmanyc() { return _eof ? -1 : 1; }

    virtual streampos seekpos(streampos sp, openmode which)
    {
        switch (which)
        {
        case ios_base2::end:
            fseek(_fp, sp, SEEK_END);
            _pos = ftell(_fp);
            setg(0,0,0);
            break;
        default:
            fseek(_fp, sp, SEEK_SET);
            _pos = ftell(_fp);
            setg(0,0,0);
        }
        return 0;
    }

    virtual int overflow(int c) { return fputc(c, _fp); }
};

class fpoutbuf : public streambuf2
{
    FILE *_fp;
public:
    fpoutbuf *open(FILE *fp) { _fp = fp; return this; }
    int overflow(int c) { return fputc(c, _fp); }
};

class filebuf2 : public fpinbuf
{
public:
    filebuf2 *open(const char *fn, ios2::openmode m);
};

namespace mystl
{
    typedef filebuf2 filebuf;
};

class string2 : public Util2
{
    char *_s = new char[255];
    uint32_t _pos = 0;
    void _append(const string2 &s, size_t subpos, size_t n) { append(s.substr(subpos, n)); }
    void _append(const char *f, const char *l) { for (;f != l; f++) push_back(*f); }
public:
    typedef size_t size_type;
    typedef char * iterator;
    typedef const char * const_iterator;
    static const size_t npos = -1;
    string2() { memset(_s, 0, 255); }
    string2(const char *s) { memset(_s, 0, 255); strcpy(_s, s); }
    string2(const char *s1, const char *s2);
    string2(const char *s1, size_t n);
    string2(size_t n, char c) { _s[n] = '\0'; while (n) _s[--n] = c; }
    string2(const string2 &s) : _pos(s._pos) { strcpy(_s, s._s); }
    string2& operator= (const string2 &s) { strcpy(_s, s._s); _pos = s._pos; return *this; }
    ~string2() { delete[] _s; }
    const char *c_str() const { return _s; }
    size_t length() const { return strlen(_s); }
    size_t size() const { return length(); }
    void push_back(char c) { _s[_pos++] = c; }
    int compare(const char *s) const { return strcmp(_s, s); }
    int compare(const string2 &s) const { return compare(s.c_str()); }
    int compare(size_t p, size_t n, const string2 &s) const { return 0; }
    int compare(size_t p, size_t n, const string2 &s, size_t sp, size_t sn) const { return 0; }
    iterator begin() { return _s; }
    iterator end() { return _s + length(); }
    const_iterator cbegin() const { return _s; }
    const_iterator cend() const { return _s + length(); }
    char at(size_t pos) const { return _s[pos]; }
    string2 substr(size_t pos) const;
    string2 substr(size_t pos, size_t len) const;
    void clear() { memset(_s, 0, 255); _pos = 0; }
    string2& append(const string2 &s);
    string2& append(const string2 &s, size_t p, size_t n) { _append(s, p, n); return *this; }
    string2& append(const char *s) { append(string2(s)); return *this; }
    string2& append(const char *s, size_t n) { append(string2(s, n)); return *this; }
    string2& append(size_t n, char c) { while (n-- > 0) push_back(c); return *this; }

    template <class T> string2& append(size_t n, T v)
    { while (n-- > 0) push_back(v); return *this; }

    string2& append(const char *f, const char *l) { _append(f, l); return *this; }
    size_t find(const string2 &s, size_t pos = 0) const;
    size_t find(const char *s, size_t pos = 0) const { string2 str(s); return find(str, pos); }
    //size_t find(const char *s, size_t pos = 0) const { return strstr(_s + pos, s) - s + pos; }
    size_t find(const char *s, size_t pos, size_t n) const { return 44; }
    size_t find(char c, size_t pos = 0) { return 51; }
    string2& replace(size_t pos, size_t len, const string2 &str);
};

class stringbuf2 : public streambuf2
{
    string2 _buf;
public:
    int overflow(int c) { _buf.push_back(c); return c; }
    string2 str() const { return _buf; }
};

template <size_t T> class bitset2
{
    uint32_t _set;
public:
    bitset2(uint32_t val) : _set(val) { }
    size_t size() const { return T; }
    bool test(size_t pos) const { return _set & (1 << pos); }
    bitset2& set() { _set = 0xffffffff; return *this; }
    bitset2& set(size_t pos) { _set |= (1<<pos); return *this; }
    bitset2& reset() { _set = 0; return *this; }
    bitset2& reset(size_t pos) { _set &= ~(1<<pos); return *this; }
    bool any() const { return _set != 0; }
    bool none() const { return _set == 0; }
    size_t count() const { size_t x = 0; for (size_t i = 0; i < T; i++) x += test(i); return x; }
};

class istream2 : public ios2
{
protected:
    size_t _lastRead = 0;
public:
    istream2() { }
    istream2(streambuf2 *sb) : ios2(sb) { }
    virtual ~istream2() { }
    virtual int get() { return _sb->sbumpc(); }
    virtual istream2 &ignore(int n = 1, int d = '\n') { while (n-- > 0) get(); return *this; }
    virtual int tellg() { return _sb->pubseekoff(0, 0, 0); }
    int gcount() { return _lastRead; }
    operator void * () const { return _sb->in_avail() == -1 ? (void *)false : (void *)true; }
    virtual void getline(char *dest, size_t size);
    virtual void read(char *s, size_t length) { _lastRead = _sb->sgetn(s, length); }
    void seekg(streampos pos) { }
    void seekg(streamoff off, ios_base2::seekdir way) { _sb->pubseekpos(off, way); }
};

class iostream2 : public istream2
{
};

class fstream2 : public iostream2
{
};

namespace mystl
{
    typedef istream2 istream;
    typedef fstream2 fstream;
}

class ifstream2 : public istream2
{
    filebuf2 _fb;
public:
    ifstream2() : istream2(&_fb) { }
    ifstream2(const char *s, int m = 1) : istream2(&_fb) { _fb.open(s, ios::in); }
    bool is_open() { return true; }
    void open(const char *fn, int mode = 1) { _fb.open(fn, ios::in); }
    void close() { }
};

class align
{
};

class width2
{
    int _size;
public:
    width2() : _size(1) { }
    width2(int n) : _size(n) { }
    int size() const { return _size; }
    void size(int n) { _size = n; }
};

class fill2
{
    char _fill;
public:
    fill2() : _fill(' ') { }
    fill2(char fill) : _fill(fill) { }
    char fill() const { return _fill; }
};

class base2
{
    uint8_t _type;
public:
    static const uint8_t DEC = 1;
    static const uint8_t HEX = 2;
    static const uint8_t OCT = 3;
    base2(uint8_t type) : _type(type) { }
    base2() : _type(DEC) { }
    uint8_t type() const { return _type; }
};

class ostream2 : public ios2
{
protected:
    base2 _base;
    fill2 _fill;
    width2 _width;
    filebuf2 _fb;
public:
    ostream2(streambuf2 *sb) : ios2(sb) { }
    void put(int c) { _sb->sputc(c); }
private:
    ostream2 &printChar(char c) { put(c); return *this; }
    ostream2 &print(const char *s) { while (*s) put(*s++); return *this; }
    ostream2 &printInt2(int32_t u);
    ostream2 &printInt(uint32_t u);
    void printStreambuf(streambuf2 *sb) { for (int x; (x = sb->sbumpc()) != EOF;) put(x); }
public:
    ostream2& operator << (char c) { return printChar(c); }
    ostream2& operator << (uint8_t c) { return printChar(c); }
    ostream2& operator << (const string2 &s) { return print(s.c_str()); }
    ostream2& operator << (const char *s) { return print(s); }
    ostream2& operator << (const align &a) { return *this; }
    ostream2& operator << (const width2 &w) { _width = w; return *this; }
    ostream2& operator << (const fill2 &f) { _fill = f; return *this; }
    ostream2& operator << (const base2 &base) { _base = base; return *this; }
    ostream2& operator << (uint16_t u) { return printInt(u); }
    ostream2& operator << (int32_t u) { return printInt2(u); }
#ifndef WIN32
    ostream2& operator << (size_t u) { return printInt(u); }
#endif
    ostream2& operator << (uint32_t u) { return printInt(u); }
    ostream2& operator << (streambuf2 *sb) { printStreambuf(sb); return *this; }
    ostream2 &write(const char *s, int n) { for (int i = 0; i < n; i++) put(s[i]); return *this; }
    virtual ~ostream2() { }
};

class ifpstream2 : public istream2
{
    fpinbuf _fb;
public:
    ifpstream2(FILE *fp) : istream2(&_fb) { _fb.open(fp); }
};

class ofpstream2 : public ostream2
{
    fpoutbuf _fb;
public:
    ofpstream2(FILE *fp) : ostream2(&_fb) { _fb.open(fp); }
};

class ofstream2 : public ostream2
{
    filebuf2 _fb;
public:
    void open(const char *fn, openmode om = out) { _fb.open(fn, om); }
    void close() { }
    ofstream2() : ostream2(&_fb) { }
    ofstream2(const char *f) : ostream2(&_fb) { _fb.open(f, ios::out); }
};

class ostringstream2 : public ostream2
{
    stringbuf2 _sb;
public:
    ostringstream2() : ostream2(&_sb) { }
    string2 str() const { return _sb.str(); }
};

template <typename T, size_t N> struct array2
{
    typedef T *iterator;
    T elems[N];
    constexpr size_t size() const { return N; }
    constexpr size_t max_size() const { return N; }
    constexpr bool empty() const { return size() == 0; }
    T& operator[](size_t n) { return const_cast<T&>(elems[n]); }
    constexpr const T& operator[](size_t n) const { return const_cast<T&>(elems[n]); }
    T &at(size_t n) { return const_cast<T&>(elems[n]); }
    constexpr const T &at(size_t n) const { return const_cast<T&>(elems[n]); }
    void fill(const T &u) { for (size_t i = 0; i < size(); i++) at(i) = u; }
    constexpr const T &front() const { return const_cast<T&>(elems[0]); }
    iterator begin() { return elems; }
    iterator end() { return elems + N; }
};

class regex2 : public Util2
{
    char _r[255];
    const char *_s() const { return _r; }
public:
    regex2(const char *s) { strcpy(_r, s); }
    friend class regex_functions;
};

namespace mystl
{
    typedef regex2 regex;
}

class regex_functions : public Util2
{
public:
    bool regex_search(const char *s, const regex2 &rx)
    {
        string2 str(s);
        return str.find(rx._s()) < 999999;
    }
};

namespace mystl
{
    template <class T> using vector = vector2<T>;
    template <typename T, size_t N> using array = array2<T, N>;
    template <size_t T> using bitset = bitset2<T>;
#ifndef _STRING_H
    void *memcpy(void *dest, const void *src, size_t n);
    char *strcpy(char *dest, const char *src);
    void *memset(void *s, const int c, const size_t n);
    char *strtok(char *s, const char *delim);
    void *memmove(char *dst, const char *src, uint32_t n);
    char *strncpy(char *dest, const char *src, size_t n);
    size_t strlen(const char *s);
#endif
#ifndef _STDLIB_H
    uint32_t strtol(const char *a, const char *b, int base);
#endif
    int strcmp(const char* s1, const char *s2);
    int strncmp(const char *s1, const char *s2, size_t n);
    template <typename T> using fpos = fpos2<T>;
    typedef string2 string;
    typedef ostream2 ostream;
    typedef ofstream2 ofstream;
    typedef ifstream2 ifstream;
    typedef ostringstream2 ostringstream;
    extern ifpstream2 cin;
    extern ofpstream2 cout;
    extern ofpstream2 cerr;
    static const char endl[] = "\n";
    extern align right;
    extern base2 hex;
    extern base2 oct;
    extern base2 dec;
    width2 setw(int length);
    fill2 setfill(char c);
    int toupper(int c);
    void toupper(const char *src, char *dest);
    void toupper(char *s);
    bool regex_search(const char *s, const regex &rx);
#ifndef _STDLIB_H
    void srand(unsigned seed);
    int rand();
#endif
    template <typename T, typename U> void sort(T f, T l, U c) { Util2 u; u.sort(f, l, c); }
    template <typename T> void sort(T f, T l) { Util2 u; u.sort(f, l); }
    template <typename T> void reverse(T first, T last) { Util2 u; u.reverse2(first, last); }
    template <typename T> const T &min(const T &a, const T &b) { Util2 u; return u.min(a, b); }
    template <typename T> inline const T &max(const T &a, const T &b) { return a < b ? b : a; }
#ifndef _WCHAR_H
    typedef mbstate_t2 mbstate_t;
#endif
    template<typename T> inline T* addressof(T& r) { Util2 u; return u.addressof(r); }
    template <class T> T max_element(T first, T last);
    template <class I, class N, class T> I fill_n(I first, N n, const T &v);
#ifndef _STDLIB_H
    uint32_t be32toh(uint32_t value);
#endif
};

#include "mystl.tcc"

#endif


