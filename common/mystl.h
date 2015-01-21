#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stdio.h>
#include <stdlib.h>

class Util2
{
public:
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned uint32_t;
    typedef long unsigned size_t;
    typedef long int ptrdiff_t;
    struct __true_type { };
    struct __false_type { };
    void reverse(char *str, const int length);
    char *itoa(const int num, char *str, const int base);
    int isdigit(int c) { return c >= '0' && c <= '9'; }
    int isspace(int c) { return c == ' '; }
    int isupper(int c) { return c >= 'A' && c <= 'Z'; }
    int islower(int c) { return c >= 'a' && c <= 'z'; }
    int isalpha(int c) { return isupper(c) || islower(c); }
    int isxdigit(int c) { return isdigit(c) || c >=  'a' && c <= 'f' || c >= 'A' && c <= 'F'; }
    int toupper(int c) { return islower(c) ? c - 32 : c; }
    void toupper(const char *src, char *dest) { while (*src) *dest++ = toupper(*src++); }
    void toupper(char *s) { toupper(s, s); }
    char tocupper(int c) { return toupper(c); }
    void *memcpy(void *dest, const void *src, size_t n);
    void *memset(void *s, const int c, const size_t n);
    char *strcpy(char *dest, const char *src);
    char *strncpy(char *dest, const char *src, size_t n);
    size_t strlen(const char *s);
    int strcmp(const char* s1, const char *s2);
    int strncmp(const char *s1, const char *s2, size_t n);
    char *strtok(char *str, const char *delimiters);
};

typedef class Util2 Util;
template<bool> struct __truth_type { typedef Util::__false_type __type; };
template<> struct __truth_type<true> { typedef Util::__true_type __type; };

template<class T, class _Tp> struct traitor
{
    enum { __value = bool(T::__value) || bool(_Tp::__value) };
    typedef typename __truth_type<__value>::__type __type;
};

template<typename, typename> struct __are_same
{
    enum { __value = 0 };
    typedef Util::__false_type __type;
};

template<typename T> struct __are_same<T, T>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<typename _Tp> struct __is_integer
{
    enum { __value = 0 };
    typedef Util::__false_type __type;
};

template<> struct __is_integer<bool>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<char>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<signed char>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<unsigned char>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<short>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<unsigned short>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<int>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<unsigned int>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<long>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<unsigned long>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<long long>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_integer<unsigned long long>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<typename _Tp> struct __is_floating
{
    enum { __value = 0 };
    typedef Util::__false_type __type;
};

template<> struct __is_floating<float>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_floating<double>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_floating<long double>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<typename _Tp> struct __is_pointer
{
    enum { __value = 0 };
    typedef Util::__false_type __type;
};

template<typename _Tp>  struct __is_pointer<_Tp*>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<typename> struct __is_normal_iterator
{
    enum { __value = 0 };
    typedef Util::__false_type __type;
};

template<typename T> struct is_arithmetic : public traitor<__is_integer<T>, __is_floating<T> > { };
template<typename T> struct __is_scalar : public traitor<is_arithmetic<T>, __is_pointer<T> > { };

template<typename _Tp> struct __is_char
{
    enum { __value = 0 };
    typedef Util::__false_type __type;
};

template<> struct __is_char<char>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<typename _Tp> struct __is_byte
{
    enum { __value = 0 };
    typedef Util::__false_type __type;
};

template<> struct __is_byte<char>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_byte<signed char>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<> struct __is_byte<unsigned char>
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<typename> struct __is_move_iterator
{
    enum { __value = 0 };
    typedef Util::__false_type __type;
};

template<bool, typename> struct __enable_if    { };
template<typename T> struct __enable_if<true, T> { typedef T __type; };
template<bool, typename _Iftrue, typename> struct __conditional_type { typedef _Iftrue __type; };
template<typename T, typename U> struct __conditional_type<false, T, U> { typedef U __type; };
template<typename _Type> inline bool __is_null_pointer(_Type* __ptr) { return __ptr == 0; }
template<typename _Type> inline bool __is_null_pointer(_Type) { return false; }
template<typename T, bool = __is_integer<T>::__value> struct __promote { typedef double __type; };
template<typename _Tp> struct __promote<_Tp, false> { };

template<typename T> struct __numeric_traits_integer
{
    static const T __min = ((T)(-1) < 0) ? (T)1 << sizeof(T) *
        __CHAR_BIT__ - ((T)(-1) < 0) : (T)0;

    static const T __max = ((T)(-1) < 0) ? (((((T)1 <<
            ((sizeof(T) * __CHAR_BIT__ - ((T)(-1) < 0)) - 1)) -1)
            << 1) + 1) : ~(T)0;

    static const bool __is_signed = ((T)(-1) < 0);
    static const int __digits = sizeof(T) * __CHAR_BIT__ - ((T)(-1) < 0);
};

template<typename _Tp> inline _Tp* __addressof(_Tp& __r)
{
    return reinterpret_cast<_Tp*>
    (&const_cast<char&>(reinterpret_cast<const volatile char&>(__r)));
}

template<typename T> inline void swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename T, size_t _Nm> inline void swap(T (&__a)[_Nm], T (&__b)[_Nm])
{
    for (size_t __n = 0; __n < _Nm; ++__n)
        swap(__a[__n], __b[__n]);
}

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template<typename Category, typename T, typename Distance = Util::ptrdiff_t,
    typename Pointer = T*, typename Reference = T&> struct iterator
{
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

template<typename _Iterator> struct iterator_traits
{
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type        value_type;
    typedef typename _Iterator::difference_type   difference_type;
    typedef typename _Iterator::pointer           pointer;
    typedef typename _Iterator::reference         reference;
};

template <typename T> struct iterator_traits<T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Util::ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T &reference;
};

template <typename T> struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Util::ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
};

template <typename T> inline typename iterator_traits<T>::iterator_category
    __iterator_category(const T&)
{
    return typename iterator_traits<T>::iterator_category();
}

template<typename T, bool> struct _Iter_base
{
    typedef T iterator_type;
    static iterator_type _S_base(T it) { return it; }
};

template<typename _Iterator> struct _Iter_base<_Iterator, true>
{
    typedef typename _Iterator::iterator_type iterator_type;
    static iterator_type _S_base(_Iterator __it) { return __it.base(); }
};

template<typename _InputIterator>
    inline typename iterator_traits<_InputIterator>::difference_type
    __distance(_InputIterator __first, _InputIterator __last,
    input_iterator_tag)
{
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    while (__first != __last)
    {
        ++__first;
        ++__n;
    }
    return __n;
}

template<typename T> inline typename iterator_traits<T>::difference_type
    __distance(T __first, T __last, random_access_iterator_tag)
{
    return __last - __first;
}

template<typename T> inline typename iterator_traits<T>::difference_type
    distance(T __first, T __last)
{
    return __distance(__first, __last, __iterator_category(__first));
}

template<typename _BidirectionalIterator, typename _Distance> inline void
    __advance(_BidirectionalIterator& __i, _Distance __n, bidirectional_iterator_tag)
{
    if (__n > 0)
        while (__n--)
            ++__i;
    else
        while (__n++)
            --__i;
}

template<typename T, typename U> inline void __advance(T &__i, U __n, random_access_iterator_tag)
{
    __i += __n;
}

template<typename _InputIterator, typename _Distance>
    inline void advance(_InputIterator& __i, _Distance __n)
{
    typename iterator_traits<_InputIterator>::difference_type __d = __n;
    __advance(__i, __d, __iterator_category(__i));
}

template<typename T> class reverse_iterator
    : public iterator<typename iterator_traits<T>::iterator_category,
          typename iterator_traits<T>::value_type,
          typename iterator_traits<T>::difference_type,
          typename iterator_traits<T>::pointer,
          typename iterator_traits<T>::reference>
{
protected:
    T current;
    typedef iterator_traits<T> __traits_type;
public:
    typedef T iterator_type;
    typedef typename __traits_type::difference_type difference_type;
    typedef typename __traits_type::pointer pointer;
    typedef typename __traits_type::reference reference;
    reverse_iterator() : current() { }
    explicit reverse_iterator(iterator_type __x) : current(__x) { }
    reverse_iterator(const reverse_iterator& __x) : current(__x.current) { }
    template<typename U> reverse_iterator(const reverse_iterator<U>& x) : current(x.base()) { }
    iterator_type base() const { return current; }
    reference operator*() const { T __tmp = current; return *--__tmp; }
    pointer operator->() const { return &(operator*()); }
    reverse_iterator& operator++() { --current; return *this; }
    reverse_iterator operator++(int) { reverse_iterator tmp = *this; --current; return tmp; }
    reverse_iterator& operator--() { ++current; return *this; }
    reverse_iterator operator--(int) { reverse_iterator tmp = *this; ++current; return tmp; }
    reverse_iterator operator+(difference_type n) const { return reverse_iterator(current - n); }
    reverse_iterator& operator+=(difference_type __n) { current -= __n; return *this; }
    reverse_iterator operator-(difference_type n) const { return reverse_iterator(current + n); }
    reverse_iterator& operator-=(difference_type __n) { current += __n; return *this; }
    reference operator[](difference_type __n) const { return *(*this + __n); }
};

template<typename T> inline bool operator==(const reverse_iterator<T> &x,
    const reverse_iterator<T> &y);

template <typename T> inline bool operator<(const reverse_iterator<T> &x,
          const reverse_iterator<T> &y)
{
    return y.base() < x.base();
}

template<typename T> inline bool operator!=(const reverse_iterator<T> &x,
           const reverse_iterator<T> &y)
{
    return !(x == y);
}

template<typename _Iterator> inline bool
    operator>(const reverse_iterator<_Iterator>& __x,
          const reverse_iterator<_Iterator>& __y)
    { return __y < __x; }

template<typename _Iterator>
    inline bool
    operator<=(const reverse_iterator<_Iterator>& __x,
           const reverse_iterator<_Iterator>& __y)
{ return !(__y < __x); }

template<typename _Iterator>
    inline bool
    operator>=(const reverse_iterator<_Iterator>& __x,
           const reverse_iterator<_Iterator>& __y)
{ return !(__x < __y); }

template<typename _Iterator>
    inline typename reverse_iterator<_Iterator>::difference_type
    operator-(const reverse_iterator<_Iterator>& __x,
          const reverse_iterator<_Iterator>& __y)
{ return __y.base() - __x.base(); }

template<typename _Iterator>
    inline reverse_iterator<_Iterator>
    operator+(typename reverse_iterator<_Iterator>::difference_type __n,
          const reverse_iterator<_Iterator>& __x)
{ return reverse_iterator<_Iterator>(__x.base() - __n); }


template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator==(const reverse_iterator<_IteratorL>& __x,
           const reverse_iterator<_IteratorR>& __y)
{ return __x.base() == __y.base(); }

template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator<(const reverse_iterator<_IteratorL>& __x,
          const reverse_iterator<_IteratorR>& __y)
{ return __y.base() < __x.base(); }

template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator!=(const reverse_iterator<_IteratorL>& __x,
           const reverse_iterator<_IteratorR>& __y)
{ return !(__x == __y); }

template<typename _IteratorL, typename _IteratorR>
    inline bool
    operator>(const reverse_iterator<_IteratorL>& __x,
          const reverse_iterator<_IteratorR>& __y)
{ return __y < __x; }

template<typename _IteratorL, typename _IteratorR> inline bool
    operator<=(const reverse_iterator<_IteratorL> &x, const reverse_iterator<_IteratorR> &y)
{
    return !(y < x);
}

template<typename _IteratorL, typename _IteratorR> inline bool
    operator>=(const reverse_iterator<_IteratorL>& __x,
           const reverse_iterator<_IteratorR>& __y)
{ return !(__x < __y); }

  template<typename _IteratorL, typename _IteratorR>

    inline typename reverse_iterator<_IteratorL>::difference_type
    operator-(const reverse_iterator<_IteratorL>& __x,
          const reverse_iterator<_IteratorR>& __y)
    { return __y.base() - __x.base(); }

    template<typename _Container> class back_insert_iterator
      : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
        _Container* container;
    public:
        typedef _Container          container_type;
        explicit back_insert_iterator(_Container& __x) : container(&__x) { }

        back_insert_iterator& operator=(typename _Container::const_reference __value)
        {
            container->push_back(__value);
            return *this;
        }

        back_insert_iterator& operator*() { return *this; }

      back_insert_iterator&
      operator++()
      { return *this; }


      back_insert_iterator
      operator++(int)
      { return *this; }
    };


  template<typename _Container>    inline back_insert_iterator<_Container>
    back_inserter(_Container& __x)
    { return back_insert_iterator<_Container>(__x); }

  template<typename _Container>    class front_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void>
    {
    protected:
      _Container* container;
    public:
      typedef _Container          container_type;
      explicit front_insert_iterator(_Container& __x) : container(&__x) { }
      front_insert_iterator&
      operator=(typename _Container::const_reference __value)
      {
    container->push_front(__value);
    return *this;
      }

      front_insert_iterator&
      operator*()
      { return *this; }

      front_insert_iterator&
      operator++()
      { return *this; }

      front_insert_iterator
      operator++(int)
      { return *this; }
    };

    template <typename T> inline front_insert_iterator<T> front_inserter(T &x)
    {
        return front_insert_iterator<T>(x);
    }

template<typename T> class insert_iterator
  : public iterator<output_iterator_tag, void, void, void, void>
{
protected:
    T *container;
    typename T::iterator iter;
public:
    typedef T container_type;
    insert_iterator(T &__x, typename T::iterator __i) : container(&__x), iter(__i) {}

    insert_iterator& operator=(typename T::const_reference __value)
    {
        iter = container->insert(iter, __value);
        ++iter;
        return *this;
    }

    insert_iterator& operator*() { return *this; }
    insert_iterator& operator++() { return *this; }
    insert_iterator& operator++(int) { return *this; }
};

template<typename T, typename U> inline insert_iterator<T> inserter(T &x, U i)
{
    return insert_iterator<T>(x, typename T::iterator(i));
}

template <typename T, typename U> class __normal_iterator
{
protected:
    T _M_current;
    typedef iterator_traits<T> __traits_type;
public:
    typedef T iterator_type;
    typedef typename __traits_type::iterator_category iterator_category;
    typedef typename __traits_type::value_type value_type;
    typedef typename __traits_type::difference_type difference_type;
    typedef typename __traits_type::reference reference;
    typedef typename __traits_type::pointer pointer;
    __normal_iterator() : _M_current(T()) { }
    explicit __normal_iterator(const T& __i) : _M_current(__i) { }

    template<typename _Iter> __normal_iterator(const __normal_iterator<_Iter,
        typename __enable_if<(__are_same<_Iter, typename U::pointer>::__value),
        U>::__type>& __i)
      : _M_current(__i.base())
    { }

    reference operator*() const { return *_M_current; }
    pointer operator->() const { return _M_current; }
    __normal_iterator& operator++() { ++_M_current; return *this; }
    __normal_iterator operator++(int) { return __normal_iterator(_M_current++); }
    __normal_iterator operator--() { --_M_current; return *this; }
    __normal_iterator operator--(int) { return __normal_iterator(_M_current--); }
    reference operator[](difference_type __n) const { return _M_current[__n]; }
    __normal_iterator& operator+=(difference_type __n) { _M_current += __n; return *this; }

    __normal_iterator operator+(difference_type __n) const
    { return __normal_iterator(_M_current + __n); }

    __normal_iterator& operator-=(difference_type __n) { _M_current -= __n; return *this; }

    __normal_iterator operator-(difference_type __n) const
    { return __normal_iterator(_M_current - __n); }

    const T& base() const { return _M_current; }
};

template <typename T, typename _Container>
    struct __is_normal_iterator<__normal_iterator<T, _Container> >
{
    enum { __value = 1 };
    typedef Util::__true_type __type;
};

template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool operator==(const __normal_iterator<_IteratorL, _Container>& __lhs,
    const __normal_iterator<_IteratorR, _Container>& __rhs)
{
    return __lhs.base() == __rhs.base();
}

template <typename T, typename U> inline bool operator==(const __normal_iterator<T, U>& __lhs,
    const __normal_iterator<T, U>& __rhs)
{
    return __lhs.base() == __rhs.base();
}

template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator!=(const __normal_iterator<_IteratorL, _Container>& __lhs,
    const __normal_iterator<_IteratorR, _Container>& __rhs)
{
    return __lhs.base() != __rhs.base();
}

template<typename _Iterator, typename _Container>
    inline bool
    operator!=(const __normal_iterator<_Iterator, _Container>& __lhs,
    const __normal_iterator<_Iterator, _Container>& __rhs)
{
    return __lhs.base() != __rhs.base();
}

template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
    const __normal_iterator<_IteratorR, _Container>& __rhs)
{
    return __lhs.base() < __rhs.base();
}

template<typename _Iterator, typename _Container> inline bool
operator<(const __normal_iterator<_Iterator, _Container>& __lhs,
      const __normal_iterator<_Iterator, _Container>& __rhs)
{ return __lhs.base() < __rhs.base(); }

template<typename _Iterator, typename _Container>
inline bool
operator<=(const __normal_iterator<_Iterator, _Container>& __lhs,
       const __normal_iterator<_Iterator, _Container>& __rhs)
{ return __lhs.base() <= __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline bool
operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs,
       const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return __lhs.base() >= __rhs.base(); }

template<typename _Iterator, typename _Container>
inline bool
operator>=(const __normal_iterator<_Iterator, _Container>& __lhs,
       const __normal_iterator<_Iterator, _Container>& __rhs)
{ return __lhs.base() >= __rhs.base(); }

template<typename _IteratorL, typename _IteratorR, typename _Container>
inline typename __normal_iterator<_IteratorL, _Container>::difference_type
operator-(const __normal_iterator<_IteratorL, _Container>& __lhs,
      const __normal_iterator<_IteratorR, _Container>& __rhs)
{ return __lhs.base() - __rhs.base(); }

template<typename _Iterator, typename _Container>
inline typename __normal_iterator<_Iterator, _Container>::difference_type
operator-(const __normal_iterator<_Iterator, _Container>& __lhs,
      const __normal_iterator<_Iterator, _Container>& __rhs)
{ return __lhs.base() - __rhs.base(); }

template<typename _Iterator, typename _Container>
inline __normal_iterator<_Iterator, _Container>
operator+(typename __normal_iterator<_Iterator, _Container>::difference_type
      __n, const __normal_iterator<_Iterator, _Container>& __i)
{ return __normal_iterator<_Iterator, _Container>(__i.base() + __n); }

struct _Iter_less_iter
{
    template<typename _Iterator1, typename _Iterator2>
        bool operator()(_Iterator1 __it1, _Iterator2 __it2) const
    {
        return *__it1 < *__it2;
    }
};

inline _Iter_less_iter __iter_less_iter()  { return _Iter_less_iter(); }

struct _Iter_less_val
{
template<typename _Iterator, typename _Value>
  bool
  operator()(_Iterator __it, _Value& __val) const
  { return *__it < __val; }
};

inline _Iter_less_val __iter_less_val() { return _Iter_less_val(); }
inline _Iter_less_val __iter_comp_val(_Iter_less_iter) { return _Iter_less_val(); }

struct _Val_less_iter
{
    template<typename _Value, typename _Iterator> bool
        operator()(_Value& __val, _Iterator __it) const
    {
        return __val < *__it;
    }
};

inline _Val_less_iter __val_less_iter() { return _Val_less_iter(); }
inline _Val_less_iter __val_comp_iter(_Iter_less_iter) { return _Val_less_iter(); }

struct _Iter_equal_to_iter
{
    template<typename _Iterator1, typename _Iterator2>
        bool
        operator()(_Iterator1 __it1, _Iterator2 __it2) const
    {
        return *__it1 == *__it2;
    }
};

inline _Iter_equal_to_iter __iter_equal_to_iter() { return _Iter_equal_to_iter(); }

struct _Iter_equal_to_val
{
    template<typename T, typename U> bool operator()(T it, U &val) const
    {
        return *it == val;
    }
};

inline _Iter_equal_to_val __iter_equal_to_val() { return _Iter_equal_to_val(); }
inline _Iter_equal_to_val __iter_comp_val(_Iter_equal_to_iter) { return _Iter_equal_to_val(); }

template <typename T> struct _Iter_comp_iter
{
    T _M_comp;
    _Iter_comp_iter(T __comp) : _M_comp(__comp) { }

    template<typename U, typename V> bool operator()(U __it1, V __it2)
    {
        return bool(_M_comp(*__it1, *__it2));
    }
};

template<typename T> inline _Iter_comp_iter<T> __iter_comp_iter(T __comp)
{
    return _Iter_comp_iter<T>(__comp);
}

template<typename T> struct _Iter_comp_val
{
    T _M_comp;
    _Iter_comp_val(T __comp) : _M_comp(__comp) { }

    template <typename U, typename V> bool operator()(U it, V& val)
    {
        return bool(_M_comp(*it, val));
    }
};

template<typename Compare> inline _Iter_comp_val<Compare> __iter_comp_val(Compare __comp)
{
    return _Iter_comp_val<Compare>(__comp);
}

template <typename Compare> inline _Iter_comp_val<Compare>
    __iter_comp_val(_Iter_comp_iter<Compare> comp)
{
    return _Iter_comp_val<Compare>(comp._M_comp);
}

template <typename _Compare> struct _Val_comp_iter
{
    _Compare _M_comp;
    _Val_comp_iter(_Compare __comp) : _M_comp(__comp) { }

    template<typename _Value, typename _Iterator> bool operator()(_Value& __val, _Iterator __it)
    {
        return bool(_M_comp(__val, *__it));
    }
};

template<typename T> inline _Val_comp_iter<T> __val_comp_iter(T comp)
{ return _Val_comp_iter<T>(comp); }

template<typename T> inline _Val_comp_iter<T> __val_comp_iter(_Iter_comp_iter<T> __comp)
{ return _Val_comp_iter<T>(__comp._M_comp); }

template<typename _Value> struct _Iter_equals_val
{
    _Value& _M_value;
    _Iter_equals_val(_Value& __value) : _M_value(__value) { }
    template<typename _Iterator> bool operator()(_Iterator __it) { return *__it == _M_value; }
};

template<typename _Value> inline _Iter_equals_val<_Value> __iter_equals_val(_Value& __val)
{
    return _Iter_equals_val<_Value>(__val);
}

template<typename _Iterator1> struct _Iter_equals_iter
{
    typename iterator_traits<_Iterator1>::reference _M_ref;
    _Iter_equals_iter(_Iterator1 __it1) : _M_ref(*__it1) { }
    template<typename _Iterator2> bool operator()(_Iterator2 __it2) { return *__it2 == _M_ref; }
};

template<typename _Iterator> inline _Iter_equals_iter<_Iterator>
    __iter_comp_iter(_Iter_equal_to_iter, _Iterator __it)
{ return _Iter_equals_iter<_Iterator>(__it); }

template<typename T> struct _Iter_pred
{
    T _M_pred;
    _Iter_pred(T __pred) : _M_pred(__pred) { }
    template<typename _Iterator> bool operator()(_Iterator __it) { return bool(_M_pred(*__it)); }
};

template<typename T> inline _Iter_pred<T> __pred_iter(T __pred) { return _Iter_pred<T>(__pred); }

template<typename T, typename U> struct _Iter_comp_to_val
{
    T _M_comp;
    U &_M_value;
    _Iter_comp_to_val(T __comp, U &__value) : _M_comp(__comp), _M_value(__value) { }
    template<typename V> bool operator()(V __it) { return bool(_M_comp(*__it, _M_value)); }
};

template<typename T, typename U> _Iter_comp_to_val<T, U> __iter_comp_val(T __comp, U &__val)
{
    return _Iter_comp_to_val<T, U>(__comp, __val);
}

template<typename _Compare, typename _Iterator1> struct _Iter_comp_to_iter
{
    _Compare _M_comp;
    typename iterator_traits<_Iterator1>::reference _M_ref;
    _Iter_comp_to_iter(_Compare __comp, _Iterator1 __it1) : _M_comp(__comp), _M_ref(*__it1) { }

    template<typename _Iterator2> bool operator()(_Iterator2 __it2)
    {
        return bool(_M_comp(*__it2, _M_ref));
    }
};

template<typename _Compare, typename _Iterator> inline _Iter_comp_to_iter<_Compare, _Iterator>
    __iter_comp_iter(_Iter_comp_iter<_Compare> __comp, _Iterator __it)
{
    return _Iter_comp_to_iter<_Compare, _Iterator>(__comp._M_comp, __it);
}

template<typename T> struct _Iter_negate
{
    T _M_pred;
    _Iter_negate(T __pred) : _M_pred(__pred) { }
    template<typename U> bool operator()(U it) { return !bool(_M_pred(*it)); }
};

template<typename _Predicate> inline _Iter_negate<_Predicate>
    __negate(_Iter_pred<_Predicate> __pred)
{
    return _Iter_negate<_Predicate>(__pred._M_pred);
}

template<bool> struct __iter_swap
{
    template<typename _ForwardIterator1, typename _ForwardIterator2> static void
        iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b)
    {
        typedef typename iterator_traits<_ForwardIterator1>::value_type _ValueType1;
        _ValueType1 __tmp = (*__a);
        *__a = (*__b);
        *__b = (__tmp);
    }
};

template<> struct __iter_swap<true>
{
    template<typename _ForwardIterator1, typename _ForwardIterator2> static void
        iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b)
    {
        swap(*__a, *__b);
    }
};

template <typename T, typename _ForwardIterator2>
    inline void iter_swap(T __a, _ForwardIterator2 __b)
{
    //typedef typename iterator_traits<T>::value_type _ValueType1;
    typedef typename iterator_traits<_ForwardIterator2>::value_type _ValueType2;
    typedef typename iterator_traits<T>::reference _ReferenceType1;
    typedef typename iterator_traits<_ForwardIterator2>::reference _ReferenceType2;

    __iter_swap<__are_same<T, _ValueType2>::__value
        && __are_same<T&, _ReferenceType1>::__value
        && __are_same<_ValueType2&, _ReferenceType2>::__value>::iter_swap(__a, __b);
}

template<typename _ForwardIterator1, typename _ForwardIterator2>
    _ForwardIterator2
    swap_ranges(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
    _ForwardIterator2 __first2)
{
    for (; __first1 != __last1; ++__first1, ++__first2)
        iter_swap(__first1, __first2);

    return __first2;
}

template <typename T> inline const T &min(const T &a, const T &b) { return b < a ? b : a; }
template <typename T> inline const T &max(const T &a, const T &b) { return a < b ? b : a; }

template <typename _Iterator> struct _Niter_base
  : _Iter_base<_Iterator, __is_normal_iterator<_Iterator>::__value>
{ };

template <typename T> inline typename _Niter_base<T>::iterator_type __niter_base(T __it)
{ return _Niter_base<T>::_S_base(__it); }

template <typename _Iterator> struct _Miter_base
  : _Iter_base<_Iterator, __is_move_iterator<_Iterator>::__value>
{ };

template<typename T> inline typename _Miter_base<T>::iterator_type __miter_base(T __it)
{
    return _Miter_base<T>::_S_base(__it);
}

template<bool, bool, typename> struct __copy_move
{
    template<typename _II, typename _OI> static _OI
        __copy_m(_II __first, _II __last, _OI __result)
    {
        for (; __first != __last; ++__result, ++__first)
            *__result = *__first;

        return __result;
    }
};

template<> struct __copy_move<false, false, random_access_iterator_tag>
{
    template<typename _II, typename _OI> static _OI
        __copy_m(_II __first, _II __last, _OI __result)
    {
        typedef typename iterator_traits<_II>::difference_type _Distance;
        for(_Distance __n = __last - __first; __n > 0; --__n)
        {
            *__result = *__first;
            ++__first;
            ++__result;
        }
        return __result;
    }
};


template<bool _IsMove> struct __copy_move<_IsMove, true, random_access_iterator_tag>
{
    template<typename _Tp> static _Tp*
        __copy_m(const _Tp* __first, const _Tp* __last, _Tp* __result)
    {
        const Util::ptrdiff_t _Num = __last - __first;
        if (_Num)
            __builtin_memmove(__result, __first, sizeof(_Tp) * _Num);
        return __result + _Num;
    }
};

template<bool _IsMove, typename _II, typename _OI> inline _OI
    __copy_move_a(_II __first, _II __last, _OI __result)
{
    typedef typename iterator_traits<_II>::value_type _ValueTypeI;
    typedef typename iterator_traits<_OI>::value_type _ValueTypeO;
    typedef typename iterator_traits<_II>::iterator_category _Category;
    const bool __simple = (__is_trivial(_ValueTypeI)
                         && __is_pointer<_II>::__value
                         && __is_pointer<_OI>::__value
                 && __are_same<_ValueTypeI, _ValueTypeO>::__value);

    return __copy_move<_IsMove, __simple, _Category>::__copy_m(__first, __last, __result);
}

template<typename _CharT> struct char_traits;
template<typename _CharT, typename _Traits> class istreambuf_iterator;
template<typename _CharT, typename _Traits> class ostreambuf_iterator;

template<bool _IsMove, typename _CharT> typename __enable_if<__is_char<_CharT>::__value,
         ostreambuf_iterator<_CharT, char_traits<_CharT> > >::__type
    __copy_move_a2(_CharT*, _CharT*,
           ostreambuf_iterator<_CharT, char_traits<_CharT> >);

template<bool _IsMove, typename _CharT>
    typename __enable_if<__is_char<_CharT>::__value,
         ostreambuf_iterator<_CharT, char_traits<_CharT> > >::__type
    __copy_move_a2(const _CharT*, const _CharT*,
           ostreambuf_iterator<_CharT, char_traits<_CharT> >);

template<bool _IsMove, typename _CharT>
    typename __enable_if<__is_char<_CharT>::__value,
                    _CharT*>::__type
    __copy_move_a2(istreambuf_iterator<_CharT, char_traits<_CharT> >,
           istreambuf_iterator<_CharT, char_traits<_CharT> >, _CharT*);

template<bool _IsMove, typename _II, typename _OI>
    inline _OI __copy_move_a2(_II __first, _II __last, _OI __result)
{
    return _OI(__copy_move_a<_IsMove>(__niter_base(__first),
                         __niter_base(__last), __niter_base(__result)));
}

template<typename _II, typename _OI> inline _OI copy(_II __first, _II __last, _OI __result)
{
    return (__copy_move_a2<__is_move_iterator<_II>::__value>
          (__miter_base(__first), __miter_base(__last), __result));
}

template<bool, bool, typename> struct __copy_move_backward
{
    template<typename _BI1, typename _BI2> static _BI2
        __copy_move_b(_BI1 __first, _BI1 __last, _BI2 __result)
    {
        while (__first != __last)
            *--__result = *--__last;
        return __result;
    }
};

template<bool _IsMove> struct __copy_move_backward<_IsMove, true, random_access_iterator_tag>
{
    template<typename T> static T* __copy_move_b(const T* first, const T* last, T* result)
    {
        const Util::ptrdiff_t num = last - first;

        if (num)
            __builtin_memmove(result - num, first, sizeof(T) * num);

        return result - num;
    }
};

template<bool _IsMove, typename _BI1, typename _BI2> inline _BI2
    __copy_move_backward_a(_BI1 __first, _BI1 __last, _BI2 __result)
{
      typedef typename iterator_traits<_BI1>::value_type _ValueType1;
      typedef typename iterator_traits<_BI2>::value_type _ValueType2;
      typedef typename iterator_traits<_BI1>::iterator_category _Category;
      const bool __simple = (__is_trivial(_ValueType1)
                         && __is_pointer<_BI1>::__value
                         && __is_pointer<_BI2>::__value
                 && __are_same<_ValueType1, _ValueType2>::__value);

      return __copy_move_backward<_IsMove, __simple,
                                   _Category>::__copy_move_b(__first, __last, __result);
}

template<bool _IsMove, typename _BI1, typename _BI2>
    inline _BI2
    __copy_move_backward_a2(_BI1 __first, _BI1 __last, _BI2 __result)
{
      return _BI2(__copy_move_backward_a<_IsMove>
          (__niter_base(__first), __niter_base(__last),
           __niter_base(__result)));
}

template<typename _BI1, typename _BI2>
    inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return (__copy_move_backward_a2<__is_move_iterator<_BI1>::__value>
          (__miter_base(__first), __miter_base(__last),  __result));
}


template<typename _ForwardIterator, typename T> inline typename
    __enable_if<!__is_scalar<T>::__value, void>::__type
    __fill_a(_ForwardIterator __first, _ForwardIterator __last, const T& __value)
{
    for (; __first != __last; ++__first)
        *__first = __value;
}

template<typename T, typename U> inline typename
    __enable_if<__is_scalar<U>::__value, void>::__type __fill_a(T first, T last, const U &__value)
{
    const U __tmp = __value;

    for (; first != last; ++first)
        *first = __tmp;
}

template<typename _Tp> inline typename __enable_if<__is_byte<_Tp>::__value, void>::__type
    __fill_a(_Tp* __first, _Tp* __last, const _Tp& __c)
{
    const _Tp __tmp = __c;
    __builtin_memset(__first, static_cast<unsigned char>(__tmp), __last - __first);
}

template<typename _ForwardIterator, typename _Tp>
    inline void fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value)
{
    __fill_a(__niter_base(__first), __niter_base(__last), __value);
}

template<typename _OutputIterator, typename _Size, typename _Tp>
    inline typename __enable_if<!__is_scalar<_Tp>::__value, _OutputIterator>::__type
    __fill_n_a(_OutputIterator __first, _Size __n, const _Tp& __value)
{
    for (__decltype(__n + 0) __niter = __n; __niter > 0; --__niter, ++__first)
        *__first = __value;
    return __first;
}

template<bool> struct __equal
{
    template<typename T, typename _II2> static bool equal(T __first1, T __last1, _II2 __first2)
    {
        for (; __first1 != __last1; ++__first1, ++__first2)
            if (!(*__first1 == *__first2))
                return false;
        return true;
    }
};

template<> struct __equal<true>
{
    template<typename T> static bool equal(const T* first1, const T* last1, const T* first2)
    {
        return !__builtin_memcmp(first1, first2, sizeof(T) * (last1 - first1));
    }
};

template<typename _II1, typename _II2>
    inline bool
    __equal_aux(_II1 __first1, _II1 __last1, _II2 __first2)
{
      typedef typename iterator_traits<_II1>::value_type _ValueType1;
      typedef typename iterator_traits<_II2>::value_type _ValueType2;
      const bool __simple = ((__is_integer<_ValueType1>::__value
                  || __is_pointer<_ValueType1>::__value)
                         && __is_pointer<_II1>::__value
                         && __is_pointer<_II2>::__value
                 && __are_same<_ValueType1, _ValueType2>::__value);

      return __equal<__simple>::equal(__first1, __last1, __first2);
}

template<typename, typename> struct __lc_rai
{
    template<typename _II1, typename _II2> static _II1 __newlast1(_II1, _II1 __last1, _II2, _II2)
    { return __last1; }

    template<typename _II> static bool __cnd2(_II __first, _II __last)
    { return __first != __last; }
};

template<> struct __lc_rai<random_access_iterator_tag, random_access_iterator_tag>
{
    template<typename _RAI1, typename _RAI2>
        static _RAI1 __newlast1(_RAI1 __first1, _RAI1 __last1, _RAI2 __first2, _RAI2 __last2)
    {
        const typename iterator_traits<_RAI1>::difference_type __diff1 = __last1 - __first1;
        const typename iterator_traits<_RAI2>::difference_type __diff2 = __last2 - __first2;
        return __diff2 < __diff1 ? __first1 + __diff2 : __last1;
    }

    template<typename _RAI> static bool __cnd2(_RAI, _RAI)
    {
        return true;
    }
};

template<typename _ForwardIterator, typename _Tp>
    inline _ForwardIterator
    lower_bound(_ForwardIterator __first, _ForwardIterator __last,
    const _Tp& __val)
{
    __glibcxx_requires_partitioned_lower(__first, __last, __val);
    return __lower_bound(__first, __last, __val, __iter_less_val());
}

template<typename _II1, typename _II2> inline bool
    equal(_II1 __first1, _II1 __last1, _II2 __first2)
{
    return __equal_aux(__niter_base(__first1), __niter_base(__last1),  __niter_base(__first2));
}

template <typename T, typename U, typename V> inline bool equal(T first1,
    T last1, U first2, V binary_pred)
{
    for (; first1 != last1; ++first1, ++first2)
        if (!bool(binary_pred(*first1, *first2)))
            return false;
    return true;
}

struct nothrow_t { };
extern const nothrow_t nothrow;
inline void* operator new(Util::size_t, void* p) throw() { return p; }

template<typename _Tp> class new_allocator
{
public:
    typedef Util::size_t size_type;
    typedef Util::ptrdiff_t difference_type;
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef _Tp& reference;
    typedef const _Tp& const_reference;
    typedef _Tp value_type;
    template<typename _Tp1> struct rebind { typedef new_allocator<_Tp1> other; };
    new_allocator() throw() { }
    new_allocator(const new_allocator&) throw() { }
    template<typename _Tp1> new_allocator(const new_allocator<_Tp1>&) throw() { }
    ~new_allocator() throw() { }
    pointer address(reference __x) const { return __addressof(__x); }
    const_pointer address(const_reference __x) const { return __addressof(__x); }

    pointer allocate(size_type __n, const void* = 0)
    {
        if (__n > this->max_size())
            throw;

        return static_cast<_Tp*>(::operator new(__n * sizeof(_Tp)));
    }

    void deallocate(pointer __p, size_type) { ::operator delete(__p); }
    size_type max_size() const throw() { return size_t(-1) / sizeof(_Tp); }
    void construct(pointer __p, const _Tp& __val)      { ::new((void *)__p) _Tp(__val); }
    void destroy(pointer __p) { __p->~_Tp(); }
};

template<typename T> inline bool  operator==(const new_allocator<T>&, const new_allocator<T>&)
{
    return true;
}

template<typename T> inline bool operator!=(const new_allocator<T>&, const new_allocator<T>&)
{
    return false;
}

template<typename> class allocator;
template<> class allocator<void>;
template<typename, typename> struct uses_allocator;

template<> class allocator<void>
{
public:
    typedef Util::size_t size_type;
    typedef Util::ptrdiff_t difference_type;
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef void value_type;
    template<typename _Tp1> struct rebind { typedef allocator<_Tp1> other; };
};

template<typename T> class allocator: public new_allocator<T>
{
public:
    typedef Util::size_t size_type;
    typedef Util::ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T& const_reference;
    typedef T value_type;
    template<typename T1> struct rebind { typedef allocator<T1> other; };
    allocator() throw() { }
    allocator(const allocator& __a) throw() : new_allocator<T>(__a) { }
    template<typename T1> allocator(const allocator<T1>&) throw() { }
    ~allocator() throw() { }
};

template<typename _T1, typename _T2> inline bool
    operator==(const allocator<_T1>&, const allocator<_T2>&)
{
    return true;
}

template<typename T> inline bool operator==(const allocator<T>&, const allocator<T>&)
{
    return true;
}

template<typename _T1, typename _T2> inline bool
    operator!=(const allocator<_T1>&, const allocator<_T2>&)
{
    return false;
}

template<typename _Tp> inline bool operator!=(const allocator<_Tp>&, const allocator<_Tp>&)
{
    return false;
}

extern template class allocator<char>;
extern template class allocator<wchar_t>;

template<typename _Alloc, bool = __is_empty(_Alloc)> struct __alloc_swap
{
    static void _S_do_it(_Alloc&, _Alloc&) { }
};

template <typename T> struct __alloc_swap<T, false>
{
    static void _S_do_it(T &__one, T &__two)
    {
        if (__one != __two)
            swap(__one, __two);
    }
};

template <typename T> struct alloc_traits
{
    typedef T allocator_type;
    typedef typename T::pointer pointer;
    typedef typename T::const_pointer const_pointer;
    typedef typename T::value_type value_type;
    typedef typename T::reference reference;
    typedef typename T::const_reference const_reference;
    typedef typename T::size_type size_type;
    typedef typename T::difference_type difference_type;
    static pointer allocate(T &__a, size_type __n) { return __a.allocate(__n); }
    static void deallocate(T &__a, pointer __p, size_type __n) { __a.deallocate(__p, __n); }

    template<typename U> static void construct(T &a, pointer p, const U &arg)
    { a.construct(p, arg); }

    static void destroy(T &__a, pointer __p) { __a.destroy(__p); }
    static size_type max_size(const T &__a) { return __a.max_size(); }
    static const T &_S_select_on_copy(const T &__a) { return __a; }
    static void _S_on_swap(T &__a, T &__b) { __alloc_swap<T>::_S_do_it(__a, __b); }
    template<typename U> struct rebind { typedef typename T::template rebind<U>::other other; };
};

template <typename _T1, typename _T2> inline void _Construct(_T1* __p, const _T2& __value)
{
    ::new(static_cast<void*>(__p)) _T1(__value);
}

template<typename T> inline void _Destroy(T *pointer) { pointer->~T(); }

template<bool> struct _Destroy_aux
{
    template<typename _ForwardIterator> static void
        __destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
        for (; __first != __last; ++__first)
            _Destroy(__addressof(*__first));
    }
};

template<> struct _Destroy_aux<true>
{
    template<typename T> static void __destroy(T, T) { }
};

template<typename _ForwardIterator> inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type  _Value_type;
    _Destroy_aux<__has_trivial_destructor(_Value_type)>::__destroy(__first, __last);
}

template<typename T, typename U> void _Destroy(T __first, T __last, U &__alloc)
{
    typedef alloc_traits<U> __traits;

    for (; __first != __last; ++__first)
        __traits::destroy(__alloc, __addressof(*__first));
}

template<typename T, typename U> inline void _Destroy(T first, T last, allocator<U>&)
{
    _Destroy(first, last);
}

template <bool> struct __uninitialized_copy
{
    template<typename T, typename U> static U __uninit_copy(T first, T last, U result)
    {
        U cur = result;

        try
        {
            for (; first != last; ++first, ++cur)
                _Construct(__addressof(*cur), *first);

            return cur;
        }
        catch(...)
        {
            _Destroy(result, cur);
            throw;
        }
    }
};

template<> struct __uninitialized_copy<true>
{
    template <typename T, typename U> static U __uninit_copy(T first, T last, U result)
    {
        return copy(first, last, result);
    }
};

template <typename T, typename _ForwardIterator> inline _ForwardIterator
    uninitialized_copy(T __first, T __last, _ForwardIterator __result)
{
    typedef typename iterator_traits<T>::value_type _ValueType1;
    typedef typename iterator_traits<_ForwardIterator>::value_type _ValueType2;
    const bool __assignable = true;

    return __uninitialized_copy<__is_trivial(_ValueType1)
                   && __is_trivial(_ValueType2)
                   && __assignable>::__uninit_copy(__first, __last, __result);
}

template<bool _TrivialValueType> struct __uninitialized_fill
{
    template<typename _ForwardIterator, typename _Tp>
        static void __uninit_fill(_ForwardIterator __first, _ForwardIterator __last,
        const _Tp& __x)
    {
        _ForwardIterator __cur = __first;
        try
        {
            for (; __cur != __last; ++__cur)
                _Construct(__addressof(*__cur), __x);
        }
        catch(...)
        {
            _Destroy(__first, __cur);
            throw;
        }
    }
};

template<typename T, typename U, typename V>
    inline U __uninitialized_copy_a(T __first, T __last, U __result, allocator<V>&)
{
    return uninitialized_copy(__first, __last, __result);
}

template<typename _InputIterator, typename _ForwardIterator, typename _Allocator>
    inline _ForwardIterator
    __uninitialized_move_a(_InputIterator __first, _InputIterator __last,
    _ForwardIterator __result, _Allocator& __alloc)
{
    return __uninitialized_copy_a((__first), (__last), __result, __alloc);
}

template<typename _InputIterator, typename _ForwardIterator, typename _Allocator>
    inline _ForwardIterator  __uninitialized_move_if_noexcept_a(_InputIterator __first,
    _InputIterator __last, _ForwardIterator __result, _Allocator& __alloc)
{
    return __uninitialized_copy_a((__first), (__last), __result, __alloc);
}

template<typename _InputIterator1, typename _InputIterator2, typename _ForwardIterator,
    typename _Allocator>
    inline _ForwardIterator __uninitialized_copy_move(_InputIterator1 __first1,
    _InputIterator1 __last1,
    _InputIterator2 __first2,
    _InputIterator2 __last2,
    _ForwardIterator __result,
    _Allocator& __alloc)
{
    _ForwardIterator __mid = __uninitialized_copy_a(__first1, __last1, __result, __alloc);

    try
    {
        return __uninitialized_move_a(__first2, __last2, __mid, __alloc);
    }
    catch(...)
    {
        _Destroy(__result, __mid, __alloc);
        throw;
    }
}

template<typename T, typename U, typename V, typename W>
    inline V __uninitialized_move_copy(T first1, T last1, U first2, U last2, V result, W &alloc)
{
    V mid = __uninitialized_move_a(first1, last1, result, alloc);

    try
    {
        return __uninitialized_copy_a(first2, last2, mid, alloc);
    }
    catch(...)
    {
        _Destroy(result, mid, alloc);
        throw;
    }
}

template<typename _ForwardIterator, typename _Tp, typename _InputIterator,
   typename _Allocator>
    inline _ForwardIterator
    __uninitialized_fill_move(_ForwardIterator __result, _ForwardIterator __mid,
              const _Tp& __x, _InputIterator __first,
              _InputIterator __last, _Allocator& __alloc)
{
    __uninitialized_fill_a(__result, __mid, __x, __alloc);
    try
    {
        return __uninitialized_move_a(__first, __last, __mid, __alloc);
    }
    catch(...)
    {
        _Destroy(__result, __mid, __alloc);
        throw;
    }
}

template<typename T, typename U, typename V, typename _Allocator> inline void
    __uninitialized_move_fill(T __first1, T __last1,
    U __first2, U __last2, const V &__x, _Allocator &__alloc)
{
    U __mid2 = __uninitialized_move_a(__first1, __last1, __first2, __alloc);

    try
    {
        __uninitialized_fill_a(__mid2, __last2, __x, __alloc);
    }
    catch(...)
    {
        _Destroy(__first2, __mid2, __alloc);
        throw;
    }
}

template<typename T, typename U> struct Vector_base
{
    typedef typename alloc_traits<U>::template rebind<T>::other _Tp_alloc_type;
    typedef typename alloc_traits<_Tp_alloc_type>::pointer pointer;

    struct Vector_impl : public _Tp_alloc_type
    {
        pointer _M_start;
        pointer _M_finish;
        pointer _M_end_of_storage;
        Vector_impl() : _Tp_alloc_type(), _M_start(0), _M_finish(0), _M_end_of_storage(0) { }

        Vector_impl(_Tp_alloc_type const& __a)
            : _Tp_alloc_type(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0)
        { }

        void _M_swap_data(Vector_impl& __x)
        {
            swap(_M_start, __x._M_start);
            swap(_M_finish, __x._M_finish);
            swap(_M_end_of_storage, __x._M_end_of_storage);
        }
    };
public:
    typedef U allocator_type;

    _Tp_alloc_type& _M_get_Tp_allocator()
    { return *static_cast<_Tp_alloc_type*>(&this->_M_impl); }

    const _Tp_alloc_type& _M_get_Tp_allocator() const
    { return *static_cast<const _Tp_alloc_type*>(&this->_M_impl); }

    allocator_type get_allocator() const { return allocator_type(_M_get_Tp_allocator()); }
    Vector_base() : _M_impl() { }
    Vector_base(const allocator_type& __a) : _M_impl(__a) { }
    Vector_base(size_t n) : _M_impl() { _M_create_storage(n); }
    Vector_base(size_t n, const allocator_type &a) : _M_impl(a) { _M_create_storage(n); }

    ~Vector_base()
    {
        _M_deallocate(this->_M_impl._M_start,
            this->_M_impl._M_end_of_storage - this->_M_impl._M_start);
    }

public:
    Vector_impl _M_impl;

    pointer _M_allocate(size_t __n)
    {
        typedef alloc_traits<_Tp_alloc_type> _Tr;
        return __n != 0 ? _Tr::allocate(_M_impl, __n) : 0;
    }

    void _M_deallocate(pointer __p, size_t __n)
    {
        typedef alloc_traits<_Tp_alloc_type> _Tr;

        if (__p)
            _Tr::deallocate(_M_impl, __p, __n);
    }
private:
    void _M_create_storage(size_t __n)
    {
        this->_M_impl._M_start = this->_M_allocate(__n);
        this->_M_impl._M_finish = this->_M_impl._M_start;
        this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
    }
};

template<typename T, typename _Size, typename _Tp, typename _Allocator> void
    __uninitialized_fill_n_a(T __first, _Size __n, const _Tp& __x, _Allocator& __alloc)
{
    T __cur = __first;
    try
    {
        typedef alloc_traits<_Allocator> __traits;

        for (; __n > 0; --__n, ++__cur)
            __traits::construct(__alloc, __addressof(*__cur), __x);
    }
    catch(...)
    {
        _Destroy(__first, __cur, __alloc);
    }
}

template <typename V, typename W = allocator<V> > class vector2 : protected Vector_base<V, W>
{
    typedef typename W::value_type _Alloc_value_type;
    typedef Vector_base<V, W> _Base;
    typedef typename _Base::_Tp_alloc_type _Tp_alloc_type;
    typedef alloc_traits<_Tp_alloc_type> _Alloc_traits;
protected:
    using _Base::_M_allocate;
    using _Base::_M_deallocate;
    using _Base::_M_impl;
    using _Base::_M_get_Tp_allocator;
public:
    typedef V value_type;
    typedef typename _Base::pointer pointer;
    typedef typename _Alloc_traits::const_pointer const_pointer;
    typedef typename _Alloc_traits::reference reference;
    typedef typename _Alloc_traits::const_reference const_reference;
    typedef __normal_iterator<pointer, vector2> iterator;
    typedef __normal_iterator<const_pointer, vector2> const_iterator;
    typedef Util::size_t size_type;
    typedef Util::ptrdiff_t difference_type;
    typedef W allocator_type;
    void _M_fill_initialize(size_type __n, const value_type& __value);
    vector2() : _Base() { }
    explicit vector2(const allocator_type &a) : _Base(a) { }

    explicit vector2(size_type n, const value_type& value = value_type(),
        const allocator_type& a = allocator_type())  : _Base(n, a)
    {
        _M_fill_initialize(n, value);
    }

    vector2(const vector2 &x) : _Base(x.size(),
        _Alloc_traits::_S_select_on_copy(x._M_get_Tp_allocator()))
    {
        this->_M_impl._M_finish = __uninitialized_copy_a(x.begin(), x.end(),
                  this->_M_impl._M_start, _M_get_Tp_allocator());
    }

    template<typename _InputIterator>  vector2(_InputIterator __first, _InputIterator __last,
        const allocator_type& __a = allocator_type())
      : _Base(__a)
    {
        typedef typename __is_integer<_InputIterator>::__type _Integral;
        _M_initialize_dispatch(__first, __last, _Integral());
    }

    reference front() { return *begin(); }
    ~vector2();
    vector2& operator=(const vector2& __x);
    void assign(size_type __n, const value_type& __val) { _M_fill_assign(__n, __val); }

    template<typename _InputIterator> void
        assign(_InputIterator __first, _InputIterator __last)
    {
        typedef typename __is_integer<_InputIterator>::__type _Integral;
        _M_assign_dispatch(__first, __last, _Integral());
    }

    using _Base::get_allocator;
    iterator begin() { return iterator(this->_M_impl._M_start); }
    const_iterator begin() const { return const_iterator(this->_M_impl._M_start); }
    iterator end() { return iterator(this->_M_impl._M_finish); }
    const_iterator end() const { return const_iterator(this->_M_impl._M_finish); }
    size_type size() const { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
    size_type max_size() const { return _Alloc_traits::max_size(_M_get_Tp_allocator()); }

    size_type capacity() const
    { return size_type(this->_M_impl._M_end_of_storage - this->_M_impl._M_start); }

    void reserve(size_type __n);
    reference operator[](size_type __n) { return *(this->_M_impl._M_start + __n); }
    const_reference operator[](size_type __n) const { return *(this->_M_impl._M_start + __n); }
protected:
    void _M_range_check(size_type __n) const { if (__n >= this->size()) throw; }
public:
    void push_back(const value_type& __x);

    void pop_back()
    {
        --this->_M_impl._M_finish;
        _Alloc_traits::destroy(this->_M_impl, this->_M_impl._M_finish);
    }

    iterator insert(iterator __position, const value_type& __x);

    void insert(iterator __position, size_type __n, const value_type& __x)
    { _M_fill_insert(__position, __n, __x); }

    template<typename T> void insert(iterator position, T first, T last)
    {
        typedef typename __is_integer<T>::__type _Integral;
        _M_insert_dispatch(position, first, last, _Integral());
    }

    iterator erase(iterator __position) { return _M_erase(__position); }
    iterator erase(iterator __first, iterator __last) { return _M_erase(__first, __last); }

    void swap(vector2& __x)
    {
        this->_M_impl._M_swap_data(__x._M_impl);
        _Alloc_traits::_S_on_swap(_M_get_Tp_allocator(), __x._M_get_Tp_allocator());
    }

    void clear() { _M_erase_at_end(this->_M_impl._M_start); }
protected:
    template<typename _ForwardIterator> pointer
        _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_allocate(__n);
        try
        {
            __uninitialized_copy_a(__first, __last, __result, _M_get_Tp_allocator());
            return __result;
        }
        catch(...)
        {
            _M_deallocate(__result, __n);
            throw;
        }
    }

    template <typename T> void _M_initialize_dispatch(T n, T __value, Util::__true_type)
    {
        this->_M_impl._M_start = _M_allocate(static_cast<size_type>(n));
        this->_M_impl._M_end_of_storage = this->_M_impl._M_start + static_cast<size_type>(n);
        _M_fill_initialize(static_cast<size_type>(n), __value);
    }

    template<typename T> void _M_initialize_dispatch(T __first, T __last, Util::__false_type)
    {
        typedef typename iterator_traits<T>::iterator_category _IterCategory;
        _M_range_initialize(__first, __last, _IterCategory());
    }

   template<typename T> void _M_range_initialize(T __first, T __last, input_iterator_tag)
    {
        for (; __first != __last; ++__first)
            push_back(*__first);
    }

    template<typename _Integer> void _M_assign_dispatch(_Integer __n,
        _Integer __val, Util::__true_type)
    {
        _M_fill_assign(__n, __val);
    }

    template<typename _InputIterator> void
    _M_assign_dispatch(_InputIterator __first, _InputIterator __last,
           Util::__false_type)
    {
        typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
        _M_assign_aux(__first, __last, _IterCategory());
    }

    template<typename _InputIterator> void
    _M_assign_aux(_InputIterator __first, _InputIterator __last,
          input_iterator_tag);

    template<typename _ForwardIterator> void
        _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last,
        forward_iterator_tag);

    void _M_fill_assign(size_type __n, const value_type& __val);

    template<typename _Integer> void
        _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val, Util::__true_type)
    { _M_fill_insert(__pos, __n, __val); }

    template<typename _InputIterator>        void
        _M_insert_dispatch(iterator __pos, _InputIterator __first,
        _InputIterator __last, Util::__false_type)
    {
        typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
        _M_range_insert(__pos, __first, __last, _IterCategory());
    }

    template<typename _InputIterator> void
        _M_range_insert(iterator __pos, _InputIterator __first,
        _InputIterator __last, input_iterator_tag);

    template<typename _ForwardIterator> void
        _M_range_insert(iterator __pos, _ForwardIterator __first,
        _ForwardIterator __last, forward_iterator_tag);

    void  _M_fill_insert(iterator __pos, size_type __n, const value_type& __x);
    void _M_insert_aux(iterator __position, const value_type& __x);

    size_type _M_check_len(size_type __n, const char* __s) const
    {
        if (max_size() - size() < __n)
            throw;

        const size_type __len = size() + max(size(), __n);
        return (__len < size() || __len > max_size()) ? max_size() : __len;
    }

    void _M_erase_at_end(pointer __pos)
    {
        _Destroy(__pos, this->_M_impl._M_finish, _M_get_Tp_allocator());
        this->_M_impl._M_finish = __pos;
    }

    iterator _M_erase(iterator __position);
    iterator _M_erase(iterator __first, iterator __last);
    template<typename _Ptr> _Ptr _M_data_ptr(_Ptr __ptr) const { return __ptr; }
};

template<typename _Tp, typename _Alloc> inline bool
    operator==(const vector2<_Tp, _Alloc>& __x, const vector2<_Tp, _Alloc>& __y)
{
    return (__x.size() == __y.size() && equal(__x.begin(), __x.end(), __y.begin()));
}

template<typename _Tp, typename _Alloc> inline bool
    operator!=(const vector2<_Tp, _Alloc>& __x, const vector2<_Tp, _Alloc>& __y)
{
    return !(__x == __y);
}

template<typename _Tp, typename _Alloc>   inline bool
    operator>(const vector2<_Tp, _Alloc>& __x, const vector2<_Tp, _Alloc>& __y)
{
    return __y < __x;
}

template<typename _Tp, typename _Alloc>
    inline bool
    operator<=(const vector2<_Tp, _Alloc>& __x, const vector2<_Tp, _Alloc>& __y)
{
    return !(__y < __x);
}

template<typename _Tp, typename _Alloc>
    inline bool
    operator>=(const vector2<_Tp, _Alloc>& __x, const vector2<_Tp, _Alloc>& __y)
{
    return !(__x < __y);
}

template <typename T, typename U> inline void swap(vector2<T, U> &x, vector2<T, U> &y)
{
    x.swap(y);
}

template <typename _Tp, typename _Alloc> void vector2<_Tp, _Alloc>::reserve(size_type __n)
{
    if (__n > this->max_size())
        throw;

    if (this->capacity() < __n)
    {
        const size_type __old_size = size();
        pointer __tmp = _M_allocate_and_copy(__n, (this->_M_impl._M_start),
            (this->_M_impl._M_finish));
        _Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());

        _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage
            - this->_M_impl._M_start);

        this->_M_impl._M_start = __tmp;
        this->_M_impl._M_finish = __tmp + __old_size;
        this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __n;
    }
}

template <typename T, typename U> typename vector2<T, U>::iterator
            vector2<T, U>::insert(iterator __position, const value_type& __x)
{
    const size_type __n = __position - begin();

    if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage && __position == end())
    {
        _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, __x);
        ++this->_M_impl._M_finish;
    }
    else
    {
        _M_insert_aux(__position, __x);
    }
    return iterator(this->_M_impl._M_start + __n);
}

template<typename _Iterator, typename _Compare>
    void __move_median_to_first(_Iterator __result,_Iterator __a, _Iterator __b,
               _Iterator __c, _Compare __comp)
{
    if (__comp(__a, __b))
    {
        if (__comp(__b, __c))
            iter_swap(__result, __b);
        else if (__comp(__a, __c))
            iter_swap(__result, __c);
        else
            iter_swap(__result, __a);
    }
    else if (__comp(__a, __c))
        iter_swap(__result, __a);
    else if (__comp(__b, __c))
        iter_swap(__result, __c);
    else
        iter_swap(__result, __b);
}


template<typename T, typename U> inline T __find_if(T first, T last, U pred, input_iterator_tag)
{
    while (first != last && !pred(first))
        ++first;

    return first;
}

template<typename T, typename U> T __find_if(T first, T last, U pred, random_access_iterator_tag)
{
    typename iterator_traits<T>::difference_type __trip_count = (last - first) >> 2;

    for (; __trip_count > 0; --__trip_count)
    {
        if (pred(first))
            return first;
        ++first;

        if (pred(first))
            return first;
        ++first;

        if (pred(first))
            return first;
        ++first;

        if (pred(first))
            return first;
        ++first;
    }

    switch (last - first)
    {
    case 3:
        if (pred(first))
            return first;
        ++first;
    case 2:
        if (pred(first))
            return first;
        ++first;
    case 1:
        if (pred(first))
            return first;
        ++first;
    case 0:
    default:
        return last;
    }
}

template<typename _Iterator, typename _Predicate>
    inline _Iterator __find_if(_Iterator __first, _Iterator __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, __iterator_category(__first));
}

template<typename _InputIterator, typename _Predicate>
    inline _InputIterator
    __find_if_not(_InputIterator __first, _InputIterator __last,
          _Predicate __pred)
{
      return __find_if(__first, __last,
                __negate(__pred),
                __iterator_category(__first));
}

template<typename _InputIterator, typename _Predicate, typename _Distance>
    _InputIterator
    __find_if_not_n(_InputIterator __first, _Distance& __len, _Predicate __pred)
{
      for (; __len; --__len, ++__first)
    if (!__pred(__first))
      break;
      return __first;
}

template<typename _ForwardIterator1, typename _ForwardIterator2,
       typename _BinaryPredicate>
    _ForwardIterator1
    __search(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
         _ForwardIterator2 __first2, _ForwardIterator2 __last2,
         _BinaryPredicate  __predicate)
{
    if (__first1 == __last1 || __first2 == __last2)
        return __first1;

    _ForwardIterator2 __p1(__first2);

    if (++__p1 == __last2)
        return __find_if(__first1, __last1, __iter_comp_iter(__predicate, __first2));

    _ForwardIterator2 __p;
    _ForwardIterator1 __current = __first1;

    for (;;)
    {
        __first1 = __find_if(__first1, __last1, __iter_comp_iter(__predicate, __first2));

        if (__first1 == __last1)
            return __last1;

        __p = __p1;
        __current = __first1;

        if (++__current == __last1)
            return __last1;

        while (__predicate(__current, __p))
        {
            if (++__p == __last2)
                return __first1;
            if (++__current == __last1)
                return __last1;
        }
        ++__first1;
    }
    return __first1;
}

template<typename _ForwardIterator, typename _Integer,
       typename _UnaryPredicate>
    _ForwardIterator
    __search_n_aux(_ForwardIterator __first, _ForwardIterator __last,
           _Integer __count, _UnaryPredicate __unary_pred,
           forward_iterator_tag)
{
    __first = __find_if(__first, __last, __unary_pred);

    while (__first != __last)
    {
        typename iterator_traits<_ForwardIterator>::difference_type __n = __count;
        _ForwardIterator __i = __first;
        ++__i;

        while (__i != __last && __n != 1 && __unary_pred(__i))
        {
            ++__i;
            --__n;
        }
        if (__n == 1)
            return __first;
        if (__i == __last)
            return __last;
        __first = __find_if(++__i, __last, __unary_pred);
    }
    return __last;
}

template<typename _RandomAccessIter, typename _Integer, typename _UnaryPredicate>
    _RandomAccessIter
    __search_n_aux(_RandomAccessIter __first, _RandomAccessIter __last,
           _Integer __count, _UnaryPredicate __unary_pred,
           random_access_iterator_tag)
{
    typedef typename iterator_traits<_RandomAccessIter>::difference_type _DistanceType;
    _DistanceType __tailSize = __last - __first;
    _DistanceType __remainder = __count;

    while (__remainder <= __tailSize)
    {
        __first += __remainder;
        __tailSize -= __remainder;
        _RandomAccessIter __backTrack = __first;

        while (__unary_pred(--__backTrack))
        {
            if (--__remainder == 0)
                return (__first - __count);
        }
        __remainder = __count + 1 - (__first - __backTrack);
    }
    return __last;
}

template<typename _ForwardIterator, typename _Integer, typename _UnaryPredicate>
    _ForwardIterator __search_n(_ForwardIterator __first, _ForwardIterator __last,
    _Integer __count,
    _UnaryPredicate __unary_pred)
{
    if (__count <= 0)
        return __first;

    if (__count == 1)
        return __find_if(__first, __last, __unary_pred);

    return __search_n_aux(__first, __last, __count, __unary_pred, __iterator_category(__first));
}

template<typename _ForwardIterator1, typename _ForwardIterator2,
       typename _BinaryPredicate>
    _ForwardIterator1
    __find_end(_ForwardIterator1 __first1, _ForwardIterator1 __last1,
           _ForwardIterator2 __first2, _ForwardIterator2 __last2,
           forward_iterator_tag, forward_iterator_tag,
           _BinaryPredicate __comp)
{
    if (__first2 == __last2)
        return __last1;

    _ForwardIterator1 __result = __last1;

    while (1)
    {
        _ForwardIterator1 __new_result = __search(__first1, __last1, __first2, __last2, __comp);

        if (__new_result == __last1)
            return __result;
        else
        {
            __result = __new_result;
            __first1 = __new_result;
            ++__first1;
        }
    }
}

template<typename _BidirectionalIterator1, typename _BidirectionalIterator2,
       typename _BinaryPredicate>
    _BidirectionalIterator1
    __find_end(_BidirectionalIterator1 __first1,
           _BidirectionalIterator1 __last1,
           _BidirectionalIterator2 __first2,
           _BidirectionalIterator2 __last2,
           bidirectional_iterator_tag, bidirectional_iterator_tag,
           _BinaryPredicate __comp)
{
    typedef reverse_iterator<_BidirectionalIterator1> _RevIterator1;
    typedef reverse_iterator<_BidirectionalIterator2> _RevIterator2;

    _RevIterator1 __rlast1(__first1);
    _RevIterator2 __rlast2(__first2);
    _RevIterator1 __rresult = __search(_RevIterator1(__last1), __rlast1,
                          _RevIterator2(__last2), __rlast2, __comp);

    if (__rresult == __rlast1)
        return __last1;
    else
    {
        _BidirectionalIterator1 __result = __rresult.base();
        advance(__result, - distance(__first2, __last2));
        return __result;
    }
}

template <typename T> void reverse(T first, T last, bidirectional_iterator_tag)
{
    while (true)
    {
        if (first == last || first == --last)
        {
            return;
        }
        else
        {
            iter_swap(first, last);
            ++first;
        }
    }
}

template <typename T> void reverse(T __first, T __last, random_access_iterator_tag);

template<typename _InputIterator, typename _Predicate>
    typename iterator_traits<_InputIterator>::difference_type
    count_if(_InputIterator __first, _InputIterator __last, _Predicate __pred)
{
      typename iterator_traits<_InputIterator>::difference_type __n = 0;
      for (; __first != __last; ++__first)
    if (__pred(__first))
      ++__n;
      return __n;
}


template<typename _InputIterator, typename _Tp>
    inline typename iterator_traits<_InputIterator>::difference_type
    count(_InputIterator __first, _InputIterator __last, const _Tp& __value)
{
    return count_if(__first, __last, __iter_equals_val(__value));
}


template<typename _BidirectionalIterator>
    inline void
    reverse(_BidirectionalIterator __first, _BidirectionalIterator __last)
{
    reverse(__first, __last, __iterator_category(__first));
}

template <typename> struct remove_reference;
template<typename _Tp> struct remove_reference { typedef _Tp   type; };
template<typename _Tp> struct remove_reference<_Tp&> { typedef _Tp   type; };

template<typename _RandomAccessIterator, typename _Compare>
    void __unguarded_linear_insert(_RandomAccessIterator __last, _Compare __comp)
{
    typename iterator_traits<_RandomAccessIterator>::value_type
    __val = (*__last);
    _RandomAccessIterator __next = __last;
    --__next;

    while (__comp(__val, __next))
    {
        *__last = (*__next);
        __last = __next;
        --__next;
    }
    *__last = (__val);
}

template<typename _RandomAccessIterator, typename _Compare>
    inline void
    __unguarded_insertion_sort(_RandomAccessIterator __first,
                   _RandomAccessIterator __last, _Compare __comp)
{
    for (_RandomAccessIterator __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, __val_comp_iter(__comp));
}

enum { _S_threshold = 16 };

template<typename _RandomAccessIterator, typename _Compare>
    void
    __final_insertion_sort(_RandomAccessIterator __first,
               _RandomAccessIterator __last, _Compare __comp)
{
    if (__last - __first > int(_S_threshold))
    {
        __insertion_sort(__first, __first + int(_S_threshold), __comp);
        __unguarded_insertion_sort(__first + int(_S_threshold), __last, __comp);
    }
    else
    {
        __insertion_sort(__first, __last, __comp);
    }
}

template<typename _RandomAccessIterator, typename _Compare>
    _RandomAccessIterator
    __unguarded_partition(_RandomAccessIterator __first,
              _RandomAccessIterator __last,
              _RandomAccessIterator __pivot, _Compare __comp)
{
    while (true)
    {
        while (__comp(__first, __pivot))
            ++__first;

        --__last;

        while (__comp(__pivot, __last))
            --__last;

        if (!(__first < __last))
            return __first;

        iter_swap(__first, __last);
        ++__first;
    }
}

template<typename T, typename U> inline T __unguarded_partition_pivot(T first, T last, U comp)
{
    T __mid = first + (last - first) / 2;
    __move_median_to_first(first, first + 1, __mid, last - 1, comp);
    return __unguarded_partition(first + 1, last, first, comp);
}

template<typename _RandomAccessIterator, typename _Distance>
    inline bool __is_heap(_RandomAccessIterator __first, _Distance __n)
{
    return __is_heap_until(__first, __n, __iter_less_iter()) == __n;
}

template<typename _RandomAccessIterator, typename _Compare,
       typename _Distance>
    inline bool
    __is_heap(_RandomAccessIterator __first, _Compare __comp, _Distance __n)
{
    return __is_heap_until(__first, __n,
    __iter_comp_iter(__comp)) == __n;
}

template<typename _RandomAccessIterator>
    inline bool
    __is_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
    return __is_heap(__first, distance(__first, __last));
}

template<typename T, typename U> inline bool __is_heap(T __first, T __last, U __comp)
{
    return __is_heap(__first, __comp, distance(__first, __last));
}

template<typename _RandomAccessIterator, typename _Distance, typename _Tp,
       typename _Compare>
    void
    __push_heap(_RandomAccessIterator __first,
        _Distance __holeIndex, _Distance __topIndex, _Tp __value,
        _Compare __comp)
{
      _Distance __parent = (__holeIndex - 1) / 2;
      while (__holeIndex > __topIndex && __comp(__first + __parent, __value))
    {
      *(__first + __holeIndex) = (*(__first + __parent));
      __holeIndex = __parent; 
      __parent = (__holeIndex - 1) / 2;
    }
      *(__first + __holeIndex) = (__value);
}

template<typename T, typename _Compare>
    inline void __pop_heap(T __first, T __last, T __result, _Compare __comp)
{
    typedef typename iterator_traits<T>::value_type _ValueType;
    typedef typename iterator_traits<T>::difference_type _DistanceType;
    _ValueType __value = (*__result);
    *__result = (*__first);
    __adjust_heap(__first, _DistanceType(0), _DistanceType(__last - __first), (__value), __comp);
}

template<typename T, typename U, typename _Tp, typename _Compare> void
    __adjust_heap(T __first, U __holeIndex, U __len, _Tp __value, _Compare __comp)
{
    const U __topIndex = __holeIndex;
    U __secondChild = __holeIndex;

    while (__secondChild < (__len - 1) / 2)
    {
        __secondChild = 2 * (__secondChild + 1);

        if (__comp(__first + __secondChild, __first + (__secondChild - 1)))
            __secondChild--;
      *(__first + __holeIndex) = (*(__first + __secondChild));
      __holeIndex = __secondChild;
    }

    if ((__len & 1) == 0 && __secondChild == (__len - 2) / 2)
    {
        __secondChild = 2 * (__secondChild + 1);
        *(__first + __holeIndex) = (*(__first + (__secondChild - 1)));
        __holeIndex = __secondChild - 1;
    }

    __push_heap(__first, __holeIndex, __topIndex, (__value), __iter_comp_val(__comp));
}


template <typename T, typename U> inline void pop_heap(T first, T last, U comp)
{
    if (last - first > 1)
    {
        --last;
        __pop_heap(first, last, last, __iter_comp_iter(comp));
    }
}


template<typename _RandomAccessIterator, typename _Compare> void
    __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    typedef typename iterator_traits<_RandomAccessIterator>::value_type _ValueType;
    typedef typename iterator_traits<_RandomAccessIterator>::difference_type _DistanceType;

    if (__last - __first < 2)
        return;

    const _DistanceType __len = __last - __first;
    _DistanceType __parent = (__len - 2) / 2;
    while (true)
    {
        _ValueType __value = (*(__first + __parent));
        __adjust_heap(__first, __parent, __len, (__value), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}


template <typename T, typename U> void __heap_select(T first, T middle, T last, U comp)
{
    __make_heap(first, middle, comp);

    for (T i = middle; i < last; ++i)
        if (comp(i, first))
            __pop_heap(first, middle, i, comp);
}

template <typename T, typename _Compare> void __sort_heap(T __first, T __last, _Compare __comp)
{
    while (__last - __first > 1)
    {
        --__last;
        __pop_heap(__first, __last, __last, __comp);
    }
}

template <typename T> inline void make_heap(T __first, T __last)
{
    __make_heap(__first, __last, __iter_less_iter());
}

template<typename T, typename U> inline void make_heap(T __first, T __last, U __comp)
{
    __make_heap(__first, __last, __iter_comp_iter(__comp));
}

template <typename T, typename _Compare> inline void
    __partial_sort(T __first, T __middle, T __last, _Compare __comp)
{
    __heap_select(__first, __middle, __last, __comp);
    __sort_heap(__first, __middle, __comp);
}

template<typename T, typename _Size, typename _Compare>
    void introsort_loop(T __first, T __last, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > int(_S_threshold))
    {
        if (__depth_limit == 0)
        {
            __partial_sort(__first, __last, __last, __comp);
            return;
        }

        --__depth_limit;
        T __cut =
        __unguarded_partition_pivot(__first, __last, __comp);
        introsort_loop(__cut, __last, __depth_limit, __comp);
        __last = __cut;
    }
}

inline int
__lg(int __n)
{ return sizeof(int) * __CHAR_BIT__  - 1 - __builtin_clz(__n); }

inline unsigned
__lg(unsigned __n)
{ return sizeof(int) * __CHAR_BIT__  - 1 - __builtin_clz(__n); }

inline long
__lg(long __n)
{ return sizeof(long) * __CHAR_BIT__ - 1 - __builtin_clzl(__n); }

inline unsigned long
__lg(unsigned long __n)
{ return sizeof(long) * __CHAR_BIT__ - 1 - __builtin_clzl(__n); }

inline long long
__lg(long long __n)
{ return sizeof(long long) * __CHAR_BIT__ - 1 - __builtin_clzll(__n); }

inline unsigned long long
__lg(unsigned long long __n)
{ return sizeof(long long) * __CHAR_BIT__ - 1 - __builtin_clzll(__n); }

template <typename T, typename U> inline void sort(T first, T last, U comp)
{
    if (first != last)
    {
        introsort_loop(first, last, __lg(last - first) * 2, comp);
        __final_insertion_sort(first, last, comp);
    }
}

template <typename T, typename U, typename V>
    inline V set_intersection(T first1, T last1, U first2, U last2, V result);

template<typename T, typename U, typename V> U transform(T first, T last, U result, V unary_op);

template<typename T, typename U> inline void sort_heap(T first, T last, U __comp)
{
    __sort_heap(first, last, __iter_comp_iter(__comp));
}


template <Util2::size_t T> class bitset
{
    typedef Util2::uint32_t uint32_t;
    typedef Util2::size_t size_t;
    uint32_t _set;
public:
    bitset(uint32_t val) : _set(val) { }
    size_t size() const { return T; }
    bool test(size_t pos) const { return _set & (1 << pos); }
    bitset& set() { _set = 0xffffffff; return *this; }
    bitset& set(size_t pos) { _set |= (1<<pos); return *this; }
    bitset& reset() { _set = 0; return *this; }
    bitset& reset(size_t pos) { _set &= ~(1<<pos); return *this; }
    bool any() const { return _set != 0; }
    bool none() const { return _set == 0; }
    size_t count() const { size_t x = 0; for (size_t i = 0; i < T; i++) x += test(i); return x; }
};

class string2
{
    char _s[255];
    typedef Util2::size_t size_t;
public:
    string2() { Util2 u; u.memset(_s, 0, sizeof(_s)); }
    string2(const char *s) { Util2 util; util.strcpy(_s, s); }
    string2(const char *s1, const char *s2);
    string2(const char *s1, size_t n);
    string2(size_t n, char c) { _s[n] = '\0'; while (n) _s[--n] = c; }
    const char *c_str() const { return _s; }
    size_t length() const { Util2 util; return util.strlen(_s); }
};

class istream2
{
    typedef Util2::uint16_t uint16_t;
protected:
    size_t _pos;
    FILE *_fp;
    size_t _lastRead;
    bool _eof;
public:
    istream2() : _pos(0), _lastRead(0), _eof(false) { }
    istream2(FILE *fp) : _pos(0), _fp(fp), _lastRead(0), _eof(false) { }
    virtual ~istream2() { }
    int peek() { int c = fgetc(_fp); ungetc(c, _fp); return c; }
    virtual istream2& ignore(size_t n = 1, int d = '\n') { while (n--) get(); return *this; }
    virtual int get() { _pos++; return fgetc(_fp); }
    int tellg() { return _pos; }
    int gcount() { return _lastRead; }
    operator void * () const { return (void *)!_eof; }
    virtual void getline(char *dest, size_t size);
    virtual void read(char *s, size_t length);
};

class istreamDebug : public istream2
{
public:
    istreamDebug() : istream2() { }
    istreamDebug(FILE *fp) : istream2(fp) { }
    virtual int get()
    {
        _pos++;
        int c = fgetc(_fp);
        fprintf(stderr, "Get:\n%x\n\n", c);
        return c;
    }

    virtual istreamDebug& ignore(size_t n = 1, int d = '\n')
    {
        while (n--)
            istream2::get();
        return *this;
    }

    virtual void read(char *s, size_t length)
    {
        char *buf = new char[length];
        Util2 u;
        _lastRead = fread(buf, 1, length, _fp);
        fprintf(stderr, "Read:\n");

        for (size_t i = 0; i < length; i++)
            fprintf(stderr, "%x ", buf[i]);

        _pos += _lastRead;
        _eof = _lastRead < length;
        u.memcpy(s, buf, length);
        delete[] buf;
    }
};

class fstream
{
    typedef Util2::uint8_t uint8_t;
public:
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
};

class ifstream2 : public istream2
{
    typedef Util2::uint8_t uint8_t;
    bool _open;
public:
    bool is_open() const { return _open; }
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    ifstream2() : _open(false) { }
    ifstream2(const char *s, int m = 1) : istream2(fopen(s, "rb")) { _open = _fp; }
    void open(const char *fn, int mode = 1) { _open = _fp = fopen(fn, "rb"); }
    void close() { if (_open) { fclose(_fp); _open = false; } }
    virtual ~ifstream2() { close(); }
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
    fill2() : _fill('a') { }
    fill2(char fill) : _fill(fill) { }
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

class ostream2
{
protected:
    typedef Util2::uint8_t uint8_t;
    typedef Util2::uint32_t uint32_t;
    uint8_t _mode;
    base2 _base;
    width2 _width;
    FILE *_fp;
public:
    static const uint8_t DEC = 0;
    static const uint8_t OCT = 1;
    static const uint8_t HEX = 2;
    ostream2() : _base(base2::DEC), _width(1) { }
    ostream2(FILE *fp) : _base(base2::DEC), _width(1), _fp(fp) { }
    ostream2& operator << (const string2 s) { fprintf(_fp, s.c_str()); return *this; }
    ostream2& operator << (const char *s) { fprintf(_fp, s); fflush(_fp); return *this; }
    ostream2& operator << (const align &a) { return *this; }
    ostream2& operator << (const width2 &w) { _width = w; return *this; }
    ostream2& operator << (const fill2 &f) { return *this; }
    ostream2& operator << (const base2 &base) { _base = base; return *this; }
    virtual ostream2& operator << (const uint32_t u);
    ostream2& write(const char *s, int n) { fwrite(s, n, 1, _fp); return *this; }
    virtual ~ostream2() { }
    void put(int c) { fputc(c, _fp); }
};

class ofstream2 : public ostream2
{
public:
    typedef int openmode;
    static const uint8_t app = 1 << 0;
    static const uint8_t ate = 1 << 1;
    static const uint8_t bin = 1 << 2;
    static const uint8_t in = 1 << 3;
    static const uint8_t out = 1 << 4;
    static const uint8_t trunc = 1 << 5;
    void open(const char *fn, openmode om = out);
    void close() { }
};

class ostringstream2 : public ostream2
{
public:
    string2 str() const { return string2("hello"); }
};

template <class T> class vector3
{
    T *_buf;
    size_t _capacity;
    size_t _size;
    int _busy;
public:
    typedef T *iterator;
    vector3() : _capacity(100), _size(0) { _buf = (T *)calloc(sizeof(T), 100); }
    void push_back(const T &a) { _buf[_size++] = a; }
    iterator begin() { return _buf; }
    iterator end() { return _buf + _size; }
    virtual ~vector3() { free(_buf); }
    T operator[](Util2::size_t n) { return _buf[n]; }
};

class ios2
{
public:
    static const Util2::uint8_t binary = 1;
};

namespace mystl
{
    template <class T> class vector : public vector2<T>
    {
    public:
        vector() : vector2<T>() { }
        vector(Util::size_t n, const T &val = T()) : vector2<T>(n, val) { }
        template <typename U> vector(U first, U last) : vector2<T>(first, last) { }
    };

    typedef Util2::uint8_t uint8_t;
    typedef Util2::uint16_t uint16_t;
    typedef Util2::uint32_t uint32_t;
    void *memcpy(void *dest, const void *src, size_t n);
    char *strcpy(char *dest, const char *src);
    void *memset(void *s, const int c, const size_t n);
    char *strncpy(char *dest, const char *src, size_t n);
    size_t strlen(const char *s);
    int strcmp(const char* s1, const char *s2);
    int strncmp(const char *s1, const char *s2, size_t n);
    typedef string2 string;
    typedef ios2 ios;
    typedef istream2 istream;
    typedef ostream2 ostream;
    typedef ofstream2 ofstream;
    typedef ifstream2 ifstream;
    typedef ostringstream2 ostringstream;
    extern istream cin;
    extern ostream cout;
    extern ostream cerr;
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
    char *strtok(char *s, const char *delim);
};

#include "mystl.tcc"

#endif


