#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <cstdio>

namespace mystl
{
#ifndef _TIME_H
struct tm
{
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_day;
    int tm_mon;
    int tm_year;
    int tm_wday;
};
#endif
}

class Util2
{
public:
    typedef unsigned char uint8_t;
    typedef short int16_t;
    typedef unsigned short uint16_t;
    typedef int int32_t;
    typedef unsigned uint32_t;
    typedef long long int64_t;
    typedef unsigned long long uint64_t;
    typedef long unsigned size_t;
    typedef long int ptrdiff_t;
    typedef uint32_t streampos;
    typedef int32_t streamsize;
#ifndef _TIME_H
    typedef uint32_t time_t;
#endif
    struct __true_type { };
    struct __false_type { };
    struct true_type { };
    struct false_type { };
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
    char *strcpy(char *dest, const char *src);
    char *strncpy(char *dest, const char *src, size_t n);
    size_t strlen(const char *s);
    int strcmp(const char* s1, const char *s2);
    int strncmp(const char *s1, const char *s2, size_t n);
    char *strtok(char *str, const char *delimiters);
    uint8_t ctoi(char c);
    uint32_t upow(uint32_t base, uint32_t exp);
    uint32_t strtol(const char *a, const char *b, int base);
    size_t strftime(char *p, size_t max, const char *fmt, const tm *tp) const;
    static bool smaller(uint32_t a, uint32_t b) { return a < b; }
    tm *localtime(time_t *t) { return new tm; }
    time_t time(time_t *timer) { return 0; }
    time_t mktime(tm *timeptr) { return 0; }
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
};

template <typename T> struct iterator_traits2
{
    typedef typename T::pointer pointer;
    typedef typename T::reference reference;
};

template <typename T> struct iterator_traits2<T*>
{
    typedef T *pointer;
    typedef T &reference;
};

template <typename T> struct iterator_traits2<const T*>
{
    typedef const T* pointer;
    typedef const T& reference;
};

template <typename T, typename U> class ni
{
protected:
    T _M_current;
public:
    typedef T iterator_type;
    typedef Util2::ptrdiff_t difference_type;
    typedef typename iterator_traits2<T>::reference reference;
    typedef typename iterator_traits2<T>::pointer pointer;
    ni() : _M_current(T()) { }
    explicit ni(const T &i) : _M_current(i) { }
    reference operator*() const { return *_M_current; }
    pointer operator->() const { return _M_current; }
    ni& operator++() { ++_M_current; return *this; }
    ni operator++(int) { return ni(_M_current++); }
    ni operator--() { --_M_current; return *this; }
    ni operator--(int) { return ni(_M_current--); }
    reference operator[](difference_type __n) const { return _M_current[__n]; }
    ni& operator+=(difference_type __n) { _M_current += __n; return *this; }
    ni operator+(difference_type n) const { return ni(_M_current + n); }
    ni& operator-=(difference_type __n) { _M_current -= __n; return *this; }
    ni operator-(difference_type n) const { return ni(_M_current - n); }
    const T& base() const { return _M_current; }
};

template <typename T, typename U> inline bool operator==(const ni<T, U>& lhs, const ni<T, U>& rhs)
{ return lhs.base() == rhs.base(); }

template<typename L, typename R, typename C> bool operator!=(const ni<L, C> &l, const ni<R, C> &r)
{ return l.base() != r.base(); }

template<typename I, typename C> inline bool operator<(const ni<I, C> &lhs, const ni<I, C> &rhs)
{ return lhs.base() < rhs.base(); }

template<typename T, typename C> inline typename ni<T, C>::difference_type
operator-(const ni<T, C>& lhs, const ni<T, C>& rhs)
{ return lhs.base() - rhs.base(); }

template <typename T, bool> struct Iter_base { static T _S_base(T it) { return it; } };
template <typename T> struct Iter_base<T, true> { static T _S_base(T it) { return it.base(); } };
template <typename> struct ini { enum { value = 0 }; };
template <typename T, typename U> struct ini<ni<T, U> > { enum { value = 1 }; };
template <typename> struct imi { enum { value = 0 }; typedef Util2::false_type __type; };
template <typename T> struct Nb : Iter_base<T, ini<T>::value> { };
template <typename T> T nb(T it) { return Nb<T>::_S_base(it); }
template <typename T> struct Mb : Iter_base<T, imi<T>::value> { };
template <typename T> T mb(T it) { return Mb<T>::_S_base(it); }

#ifndef _NEW
#define _NEW
inline void* operator new(Util2::size_t, void* p) noexcept { return p; }
#endif

template<typename T> class allocator2
{
public:
    typedef Util2::size_t size_type;
    typedef Util2::ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T& const_reference;
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

template <typename T, typename U> struct Vector_impl2 : public allocator2<T>
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
    Vector_impl2<T, allocator2<T> > _M_impl;
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

template <typename V> class vector2 : public Vector_base<V>
{
    typedef Vector_base<V> Base;
    typedef alloc_traits<typename Vector_base<V>::A> atraits;
protected:
    using Base::_M_allocate;
    using Base::_M_deallocate;
    using Base::_M_impl;
    using Base::_M_get_Tp_allocator;
public:
    typedef ni<V *, vector2> iterator;
    typedef ni<const V *, vector2> const_iterator;
    void _M_fill_initialize(size_t n, const V &value) { _M_impl.fin = _M_impl.eos; }
    vector2() : Base() { }
    explicit vector2(size_t n) : Base(n, allocator2<V>()) { _M_fill_initialize(n, V()); }
    vector2(const vector2 &x);
    V &front() { return *begin(); }
    ~vector2();
    void assign(size_t n, const V &val) { _M_fill_assign(n, val); }
    iterator begin() { return iterator(_M_impl.start); }
    const_iterator begin() const { return const_iterator(_M_impl.start); }
    iterator end() { return iterator(_M_impl.fin); }
    const_iterator end() const { return const_iterator(_M_impl.fin); }
    size_t size() const { return size_t(_M_impl.fin - _M_impl.start); }
    size_t max_size() const { return atraits::max_size(_M_get_Tp_allocator()); }
    void reserve(size_t n);
    const V& operator[](size_t n) const { return *(_M_impl.start + n); }
    void push_back(const V &x);
    void pop_back() { --_M_impl.fin; atraits::destroy(_M_impl, _M_impl.fin); }
    void insert(iterator pos, size_t n, const V &x) { _M_fill_insert(pos, n, x); }
    const V &at(size_t n) const { return (*this)[n]; }
    void clear() { _M_erase_at_end(this->_M_impl.start); }
protected:
    void _M_fill_assign(size_t n, const V &val);
    void _M_fill_insert(iterator pos, size_t n, const V &x);
    void _M_insert_aux(iterator position, const V &x);
    size_t _M_check_len(size_t n, const char * s) const;
    template<typename Ptr> Ptr _M_data_ptr(Ptr ptr) const { return ptr; }
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
    typedef Util2::uint64_t uint64_t;
public:
    fpos2(uint64_t off) { }
};

class ios_base2 : public Util2
{
public:
    //typedef _Ios_Seekdir2 seekdir;
    typedef int seekdir;
    typedef int openmode;
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    static const uint8_t out = 4;
    static const uint8_t ate = 8;
    static const uint8_t app = 16;
    static const uint8_t trunc = 32;
    static const uint8_t beg = 0;
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
    char *gptr() const { return _inCur; }
    char *egptr() const { return _inEnd; }
    char *eback() const { return _inBeg; }
    void setg(char *beg, char *next, char *end) { _inBeg = beg; _inCur = next; _inEnd = end; }
    void gbump(int n) { _inCur += n; }
    virtual int overflow(int c) { throw "Overflow not implemented"; return 0; }
    virtual int underflow() { throw "Underflow not implemented"; return 0; }
    virtual int uflow() { return underflow() == EOF ? EOF : *_inCur++; }
    virtual streampos seekoff(int64_t, seekdir, openmode) { throw "Seekoff"; return 0; }
    virtual streamsize xsgetn(char *s, streamsize n);
    //virtual uint32_t xsputn(const char *s, uint32_t n) { return 0; }
    //virtual streambuf2 *setbuf(char *s, uint32_t n) { return this; }
    //virtual streampos seekpos(streampos sp, uint8_t which) { return 0; }
    virtual streamsize showmanyc() { throw "Showmanyc not implemented"; return 0; }
    //virtual int pbackfail(int c) { return 0; }
    virtual int sync() { return 0; }
public:
    streamsize in_avail()
    { uint64_t a = (uint64_t)egptr() - (uint64_t)gptr(); return a == 0 ? showmanyc() : a; }

    int sputc(char c) { return overflow(c); }
    int sbumpc() { return ((!gptr()) || (gptr() == egptr())) ? uflow() : *_inCur++; }
    int sgetc() { return ((!gptr()) || gptr() == egptr()) ? underflow() : *gptr(); }
    int snextc() { return sbumpc() == EOF ? EOF : sgetc(); }
    int sgetn(char *s, int n) { return xsgetn(s, n); }
    int pubsync() { return sync(); }
    //streampos pubseekpos(streampos pos, uint8_t which) { return seekpos(pos, which); }
    streampos pubseekoff(int64_t off, seekdir way, openmode m) { return seekoff(off, way, m); }
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
};

namespace mystl
{
    typedef ios2 ios;
}

class fpinbuf : public streambuf2
{
    FILE *_fp;
    typedef Util2::uint32_t uint32_t;
    const uint32_t _put_back = 8;
    char _buffer[264];
    uint32_t _pos = 0;
public:
    fpinbuf *open(FILE *fp) { _fp = fp; return this; }
    int underflow();
    streamsize xsgetn(char *s, streamsize n) { return fread(s, n, 1, _fp); }
};

class fpoutbuf : public streambuf2
{
    FILE *_fp;
public:
    fpoutbuf *open(FILE *fp) { _fp = fp; return this; }
    int overflow(int c) { return fputc(c, _fp); }
};

class filebuf2 : public streambuf2
{
    FILE *_fp;
    const uint32_t _put_back = 8;
    char _buffer[264];
    uint32_t _pos = 0;
    streamsize _lastRead = 0;
    bool multiFlag = false;
    bool _eof = false;
public:
    streampos seekoff(int64_t off, seekdir way, openmode m)
    {
        if (multiFlag)
        {
            multiFlag = false;
            return ftell(_fp);
        }

        uint32_t pb = ftell(_fp) > _lastRead ? 8 : 0;
        return (uint64_t)gptr() - (uint64_t)eback() + ftell(_fp) - _lastRead - pb;
    }

    int overflow(int c) { return fputc(c, _fp); }
    streamsize showmanyc() { return _eof ? -1 : 1; }
    filebuf2 *open(const char *fn, ios::openmode m);
    int underflow();

    streamsize xsgetn(char *s, streamsize n)
    {
        multiFlag = true;
        _lastRead = fread(s, 1, n, _fp);
        _eof = _lastRead != n;
        return _lastRead;
    }
};

namespace mystl
{
    typedef filebuf2 filebuf;
};

class string2
{
    typedef Util2::size_t size_t;
    typedef Util2::uint32_t uint32_t;
    char _s[255];
    uint32_t _pos = 0;
public:
    string2() { Util2 u; u.memset(_s, 0, sizeof(_s)); }
    string2(const char *s) { Util2 util; util.strcpy(_s, s); }
    string2(const char *s1, const char *s2);
    string2(const char *s1, size_t n);
    string2(size_t n, char c) { _s[n] = '\0'; while (n) _s[--n] = c; }
    const char *c_str() const { return _s; }
    size_t length() const { Util2 util; return util.strlen(_s); }
    void push_back(char c) { _s[_pos++] = c; }
};

class stringbuf2 : public streambuf2
{
    string2 _buf;
public:
    int overflow(int c) { _buf.push_back(c); return c; }
    string2 str() const { return _buf; }
};

template <Util2::size_t T> class bitset2
{
    typedef Util2::uint32_t uint32_t;
    typedef Util2::size_t size_t;
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
    //bool _eof = false;
public:
    istream2() { }
    istream2(streambuf2 *sb) : ios2(sb) { }
    virtual ~istream2() { }
    virtual int get() { return _sb->sbumpc(); }
    virtual istream2 &ignore(size_t n = 1, int d = '\n') { while (n--) get(); return *this; }
    virtual int tellg() { return _sb->pubseekoff(0, 0, 0); }
    int gcount() { return _lastRead; }
    //operator void * () const { return (void *)!_eof; }
    operator void * () const { return _sb->in_avail() == -1 ? (void *)false : (void *)true; }
    virtual void getline(char *dest, size_t size);
    virtual void read(char *s, size_t length) { _lastRead = _sb->sgetn(s, length); }
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
    typedef Util2::uint8_t uint8_t;
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
    typedef Util2::uint8_t uint8_t;
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
    typedef Util2::uint8_t uint8_t;
    typedef Util2::uint32_t uint32_t;
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
    ostream2 &printInt(uint32_t u);
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
    ostream2& operator << (int32_t u) { return printInt(u); }
    ostream2& operator << (size_t u) { return printInt(u); }
    ostream2& operator << (uint32_t u) { return printInt(u); }
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
    void open(const char *fn, openmode om = out);
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

template <class T> class vector3
{
    typedef Util2::uint32_t uint32_t;
    T _buf[100];
    uint32_t _capacity;
    uint32_t _size;
    int _busy;
public:
    typedef T *const_iterator;
    vector3() : _capacity(100), _size(0) { }
    void push_back(const T &a) { _buf[_size++] = a; }
    const_iterator begin() { return _buf; }
    const_iterator end() { return _buf + _size; }
};

template <class T> class vector4
{
    typedef Util2::uint32_t uint32_t;
    T *_buf;
    uint32_t _capacity;
    uint32_t _size;
    int _busy;
public:
    typedef T *iterator;
    typedef T *const_iterator;
    vector4() : _capacity(100), _size(0) { }
    void push_back(const T &a) { _buf[_size++] = a; }
    iterator begin() { return _buf; }
    iterator end() { return _buf + _size; }   
};

template <typename T, size_t N> struct array2
{
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
};

namespace mystl
{
    template <class T> using vector = vector2<T>;
    template <typename T, size_t N> using array = array2<T, N>;
    template <Util2::size_t T> using bitset = bitset2<T>;
#ifndef _STDINT_H
    typedef Util2::uint8_t uint8_t;
    typedef Util2::uint16_t uint16_t;
    typedef Util2::int32_t int32_t;
    typedef Util2::uint32_t uint32_t;
    typedef Util2::int64_t int64_t;
    typedef Util2::uint64_t uint64_t;
#endif
#ifndef _TIME_H
    typedef Util2::time_t time_t;
    tm *localtime(time_t *t);
    time_t time(time_t *timer);
    time_t mktime(tm *timeptr);
    size_t strftime(char *p, size_t max, const char *fmt, const tm *tp);
#endif
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
    int isdigit(int c);
    int isspace(int c);
    int isupper(int c);
    int islower(int c);
    int isalpha(int c);
    int isxdigit(int c);
    int toupper(int c);
    void toupper(const char *src, char *dest);
    void toupper(char *s);
    template <typename T, typename U> void sort(T f, T l, U c) { Util2 u; u.sort(f, l, c); }
    template <typename T> void sort(T f, T l) { Util2 u; u.sort(f, l); }
    template <typename T> void reverse(T first, T last) { Util2 u; u.reverse2(first, last); }
    template <typename T> const T &min(const T &a, const T &b) { Util2 u; return u.min(a, b); }
    template <typename T> inline const T &max(const T &a, const T &b) { return a < b ? b : a; }
#ifndef _WCHAR_H
    typedef mbstate_t2 mbstate_t;
#endif
    template<typename T> inline T* addressof(T& r) { Util2 u; return u.addressof(r); }
};

#include "mystl.tcc"

#endif


