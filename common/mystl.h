#ifndef _MYSTL_H_
#define _MYSTL_H_
#include <stdio.h>
#include <new>

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
    struct __true_type { };
    struct __false_type { };
    struct true_type { };
    struct false_type { };
    void reverse(char *str, const int length);
    char *itoa(const uint32_t num, char *str, const int base);
    char *itoa(const int32_t num, char *str, const int base);
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
};

template<bool> struct __truth_type { typedef Util2::false_type __type; };
template<> struct __truth_type<true> { typedef Util2::true_type __type; };

template<class T, class _Tp> struct traitor
{
    enum { __value = bool(T::__value) || bool(_Tp::__value) };
    typedef typename __truth_type<__value>::__type __type;
};

template<typename, typename> struct __are_same
{
    enum { __value = 0 };
    typedef Util2::false_type __type;
};

template<typename T> struct __are_same<T, T>
{ enum { __value = 1 }; typedef Util2::true_type __type; };

template<typename _Tp> struct __is_pointer
{
    enum { __value = 0 };
    typedef Util2::false_type __type;
};

template<typename _Tp>  struct __is_pointer<_Tp*>
{
    enum { __value = 1 };
    typedef Util2::true_type __type;
};

template<typename> struct __is_normal_iterator
{
    enum { __value = 0 };
    typedef Util2::false_type __type;
};

template<typename> struct imi
{
    enum { __value = 0 };
    typedef Util2::false_type __type;
};

template<typename T> struct __numeric_traits_integer
{
    static const T __min = ((T)(-1) < 0) ? (T)1 << sizeof(T) *
        __CHAR_BIT__ - ((T)(-1) < 0) : (T)0;

    static const T __max = ((T)(-1) < 0) ? (((((T)1 <<
            ((sizeof(T) * __CHAR_BIT__ - ((T)(-1) < 0)) - 1)) -1) << 1) + 1) : ~(T)0;

    static const bool __is_signed = ((T)(-1) < 0);
    static const int __digits = sizeof(T) * __CHAR_BIT__ - ((T)(-1) < 0);
};

template<typename T> inline T* addressof(T& r)
{ return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(r))); }

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
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template<typename Category, typename T, typename Distance = Util2::ptrdiff_t,
    typename Pointer = T*, typename Reference = T&> struct iterator
{
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

template<typename T> struct iterator_traits
{
    typedef typename T::iterator_category iterator_category;
    typedef typename T::value_type value_type;
    typedef typename T::difference_type  difference_type;
    typedef typename T::pointer pointer;
    typedef typename T::reference reference;
};

template <typename T> struct iterator_traits<T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Util2::ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T &reference;
};

template <typename T> struct iterator_traits<const T*>
{
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Util2::ptrdiff_t difference_type;
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
    static T _S_base(T it) { return it; }
};

template<typename T> struct _Iter_base<T, true>
{
    typedef typename T::iterator_type iterator_type;
    static typename T::iterator_type _S_base(T it) { return it.base(); }
};

template <typename T, typename U> class ni
{
protected:
    T _M_current;
public:
    typedef T iterator_type;
    typedef typename iterator_traits<T>::iterator_category iterator_category;
    typedef typename iterator_traits<T>::value_type value_type;
    typedef typename iterator_traits<T>::difference_type difference_type;
    typedef typename iterator_traits<T>::reference reference;
    typedef typename iterator_traits<T>::pointer pointer;
    ni() : _M_current(T()) { }
    explicit ni(const T& __i) : _M_current(__i) { }
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

template <typename T, typename T2> struct __is_normal_iterator<ni<T, T2> >
{
    enum { __value = 1 };
    typedef Util2::true_type __type;
};

template<typename L, typename R, typename C>
    inline bool operator==(const ni<L, C>& lhs, const ni<R, C>& rhs)
{
    return lhs.base() == rhs.base();
}

template <typename T, typename U> inline bool operator==(const ni<T, U>& __lhs,
    const ni<T, U>& __rhs)
{
    return __lhs.base() == __rhs.base();
}

template<typename L, typename R, typename C> inline bool
    operator!=(const ni<L, C>& lhs, const ni<R, C>& rhs)
{
    return lhs.base() != rhs.base();
}

template<typename I, typename C> inline bool
    operator<(const ni<I, C> &lhs, const ni<I, C> &rhs)
{ return lhs.base() < rhs.base(); }

template<typename I, typename C> inline bool operator<=(const ni<I, C>& lhs,
        const ni<I, C>& __rhs)
{ return lhs.base() <= __rhs.base(); }

template<typename L, typename R, typename C> inline bool
    operator>=(const ni<L, C>& lhs, const ni<R, C>& rhs)
{ return lhs.base() >= rhs.base(); }

template<typename _Iterator, typename _Container>
    inline typename ni<_Iterator, _Container>::difference_type
    operator-(const ni<_Iterator, _Container>& __lhs,
    const ni<_Iterator, _Container>& __rhs)
{ return __lhs.base() - __rhs.base(); }

template<typename _Iterator, typename _Container>
    inline ni<_Iterator, _Container>
    operator+(typename ni<_Iterator, _Container>::difference_type
    __n, const ni<_Iterator, _Container>& __i)
{ return ni<_Iterator, _Container>(__i.base() + __n); }

struct _Iter_less_iter
{
    template<typename T, typename T2> bool operator()(T it1, T2 it2) const { return *it1 < *it2; }
};

inline _Iter_less_iter __iter_less_iter() { return _Iter_less_iter(); }

struct _Iter_less_val
{ template<typename I, typename V> bool operator()(I it, V& val) const { return *it < val; } };

inline _Iter_less_val __iter_less_val() { return _Iter_less_val(); }
inline _Iter_less_val __iter_comp_val(_Iter_less_iter) { return _Iter_less_val(); }

struct _Iter_equal_to_iter
{ template<typename I1, typename I2> bool operator()(I1 i1, I2 i2) const { return *i1 == *i2; } };

inline _Iter_equal_to_iter __iter_equal_to_iter() { return _Iter_equal_to_iter(); }

struct _Iter_equal_to_val
{ template<typename T, typename U> bool operator()(T it, U &val) const { return *it == val; } };

inline _Iter_equal_to_val __iter_equal_to_val() { return _Iter_equal_to_val(); }
inline _Iter_equal_to_val __iter_comp_val(_Iter_equal_to_iter) { return _Iter_equal_to_val(); }

template <typename T> struct _Iter_comp_iter
{
    T _M_comp;
    _Iter_comp_iter(T __comp) : _M_comp(__comp) { }

    template<typename U, typename V> bool operator()(U it1, V it2)
    { return bool(_M_comp(*it1, *it2)); }
};

template<typename T> inline _Iter_comp_iter<T> __iter_comp_iter(T comp)
{ return _Iter_comp_iter<T>(comp); }

template <typename T> struct _Val_comp_iter
{
    T _M_comp;
    _Val_comp_iter(T __comp) : _M_comp(__comp) { }
    template<typename V, typename I> bool operator()(V &v, I it) { return bool(_M_comp(v, *it)); }
};

template<typename T> struct _Iter_pred
{
    T _M_pred;
    _Iter_pred(T __pred) : _M_pred(__pred) { }
    template<typename _Iterator> bool operator()(_Iterator __it) { return bool(_M_pred(*__it)); }
};

template<typename T, typename U> struct _Iter_comp_to_val
{
    T _M_comp;
    U &_M_value;
    _Iter_comp_to_val(T __comp, U &__value) : _M_comp(__comp), _M_value(__value) { }
    template<typename V> bool operator()(V __it) { return bool(_M_comp(*__it, _M_value)); }
};

template<typename T> struct _Iter_negate
{
    T _M_pred;
    _Iter_negate(T __pred) : _M_pred(__pred) { }
    template<typename U> bool operator()(U it) { return !bool(_M_pred(*it)); }
};

template<bool> struct __iter_swap
{
    template<typename T, typename T2> static void iter_swap(T a, T2 b)
    {
        typename iterator_traits<T>::value_type tmp = *a;
        *a = *b;
        *b = tmp;
    }
};

template<> struct __iter_swap<true>
{ template<typename T, typename T2> static void iter_swap(T a, T2 b) { swap(*a, *b); } };

template <typename T, typename T2> inline void iter_swap(T __a, T2 __b)
{
    typedef typename iterator_traits<T2>::value_type _ValueType2;
    typedef typename iterator_traits<T>::reference _ReferenceType1;
    typedef typename iterator_traits<T2>::reference _ReferenceType2;

    __iter_swap<__are_same<T, _ValueType2>::__value && __are_same<T&, _ReferenceType1>::__value
        && __are_same<_ValueType2&, _ReferenceType2>::__value>::iter_swap(__a, __b);
}

template <typename T> inline const T &min2(const T &a, const T &b) { return b < a ? b : a; }
template <typename T> inline const T &max2(const T &a, const T &b) { return a < b ? b : a; }
template <typename T> struct _Niter_base : _Iter_base<T, __is_normal_iterator<T>::__value> { };

template <typename T> inline typename _Niter_base<T>::iterator_type __niter_base(T it)
{ return _Niter_base<T>::_S_base(it); }

template <typename T> struct _Miter_base : _Iter_base<T, imi<T>::__value> { };

template<typename T> inline typename _Miter_base<T>::iterator_type mb(T it)
{ return _Miter_base<T>::_S_base(it); }

template<bool, bool, typename> struct __copy_move
{
    template<typename _II, typename _OI> static _OI __copy_m(_II first, _II last, _OI result)
    {
        for (; first != last; ++result, ++first) *result = *first;
        return result;
    }
};

template<> struct __copy_move<false, false, random_access_iterator_tag>
{
    template<typename II, typename OI> static OI __copy_m(II first, II last, OI result)
    {
        for (typename iterator_traits<II>::difference_type n = last - first; n > 0; --n)
            *result++ = *first++;
        
        return result;
    }
};

template<bool Move> struct __copy_move<Move, true, random_access_iterator_tag>
{
    template<typename T> static T *__copy_m(const T *first, const T *last, T *result)
    {
        const Util2::ptrdiff_t _Num = last - first;
        if (_Num) __builtin_memmove(result, first, sizeof(T) * _Num);
        return result + _Num;
    }
};

template<bool T, typename II, typename _OI> inline _OI copy_move_a(II first, II last, _OI result)
{
    typedef typename iterator_traits<II>::value_type _ValueTypeI;
    typedef typename iterator_traits<_OI>::value_type _ValueTypeO;
    typedef typename iterator_traits<II>::iterator_category _Category;

    const bool __simple = (__is_trivial(_ValueTypeI) && __is_pointer<II>::__value
            && __is_pointer<_OI>::__value && __are_same<_ValueTypeI, _ValueTypeO>::__value);

    return __copy_move<T, __simple, _Category>::__copy_m(first, last, result);
}

template<bool T, typename II, typename OI> inline OI copy_move_a2(II first, II last, OI result)
{ return OI(copy_move_a<T>(__niter_base(first), __niter_base(last), __niter_base(result))); }

template<typename T, typename T2> inline T2 copy(T f, T l, T2 r)
{ return copy_move_a2<imi<T>::__value>(mb(f), mb(l), r); }

template<bool, bool, typename> struct __copy_move_backward
{
    template<typename T, typename T2> static T2 copy_move_b(T first, T last, T2 result)
    {
        while (first != last) *--result = *--last;
        return result;
    }
};

template<bool _IsMove> struct __copy_move_backward<_IsMove, true, random_access_iterator_tag>
{
    template<typename T> static T* copy_move_b(const T* first, const T* last, T* result)
    {
        const Util2::ptrdiff_t num = last - first;
        if (num) __builtin_memmove(result - num, first, sizeof(T) * num);
        return result - num;
    }
};

template<bool T, typename _BI1, typename _BI2> inline _BI2
__copy_move_backward_a(_BI1 first, _BI1 last, _BI2 result)
{
    typedef typename iterator_traits<_BI1>::value_type _ValueType1;
    typedef typename iterator_traits<_BI2>::value_type _ValueType2;
    typedef typename iterator_traits<_BI1>::iterator_category _Category;

    const bool __simple = (__is_trivial(_ValueType1) && __is_pointer<_BI1>::__value
               && __is_pointer<_BI2>::__value && __are_same<_ValueType1, _ValueType2>::__value);

    return __copy_move_backward<T, __simple, _Category>::copy_move_b(first, last, result);
}

template<bool T, typename BI1, typename BI2> inline BI2
copy_move_back_a2(BI1 first, BI1 last, BI2 result)
{
    return BI2(__copy_move_backward_a<T>
        (__niter_base(first), __niter_base(last), __niter_base(result)));
}

template<typename I, typename T> inline void fill(I first, I last, const T &value)
{ __fill_a(__niter_base(first), __niter_base(last), value); }

#if 0
inline void* operator new(Util2::size_t, void* p) noexcept { return p; }
#endif

template<typename> class allocator;
template<> class allocator<void>;

template<typename T> class allocator
{
public:
    typedef Util2::size_t size_type;
    typedef Util2::ptrdiff_t difference_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T& const_reference;
    typedef T value_type;
    template<typename T1> struct rebind { typedef allocator<T1> other; };
    allocator() { }
    template<typename T1> allocator(const allocator<T1>&) { }
    ~allocator() { }
    T *address(T &x) const { return __addressof(x); }
    const T *address(const T &x) const { return __addressof(x); }

    T *allocate(size_t n, const void* = 0) const
    { return static_cast<T*>(::operator new(n * sizeof(T))); }

    void deallocate(T *p, size_t) const { ::operator delete(p); }
    size_t max_size() const { return size_t(-1) / sizeof(T); }
    void construct(T *p, const T &val) const { ::new((void *)p) T(val); }
    void destroy(T *p) const { p->~T(); }
};

template<typename A, bool = __is_empty(A)> struct __alloc_swap
{ static void _S_do_it(A&, A&) { } };

template <typename T> struct __alloc_swap<T, false>
{ static void _S_do_it(T &__one, T &__two) { if (__one != __two) swap(__one, __two); } };

template <typename T> struct alloc_traits
{
    typedef typename T::pointer pointer;
    typedef typename T::const_pointer const_pointer;
    typedef typename T::value_type value_type;
    static typename T::pointer allocate(T &a, size_t n) { return a.allocate(n); }
    static void deallocate(T &a, typename T::pointer p, size_t n) { a.deallocate(p, n); }
    template<typename U> static void construct(T &a, pointer p, const U &b) { a.construct(p, b); }
    static void destroy(T &a, pointer p) { a.destroy(p); }
    static size_t max_size(const T &a) { return a.max_size(); }
    static const T &_S_select_on_copy(const T &a) { return a; }
    static void _S_on_swap(T &a, T &b) { __alloc_swap<T>::_S_do_it(a, b); }
    template<typename U> struct rebind { typedef typename T::template rebind<U>::other other; };
};

template <typename T, typename T2> inline void _Construct(T *p, const T2& value)
{ ::new(static_cast<void*>(p)) T(value); }

template<typename T> inline void _Destroy2(T *pointer) { pointer->~T(); }

template<bool> struct _Destroy_aux
{
    template<typename T> static void __destroy(T first, T last)
    { for (; first != last; ++first) _Destroy2(addressof(*first)); }
};

template<> struct _Destroy_aux<true> { template<typename T> static void __destroy(T, T) { } };

template<typename I> inline void _Destroy2(I first, I last)
{
    typedef typename iterator_traits<I>::value_type _Value_type;
    _Destroy_aux<__has_trivial_destructor(_Value_type)>::__destroy(first, last);
}

template<typename T, typename U> void _Destroy2(T first, T last, U &alloc)
{ for (; first != last; ++first) alloc_traits<U>::destroy(alloc, addressof(*first)); }

template<typename T, typename U> inline void _Destroy2(T first, T last, allocator<U>&)
{ _Destroy2(first, last); }

template <bool> struct __uninitialized_copy
{
    template<typename T, typename U> static U __uninit_copy(T first, T last, U result)
    {
        U cur = result;
        for (; first != last; ++first, ++cur) _Construct(cur, *first);
        //for (; first != last; ++first, ++cur) ::new(static_cast<void*>(cur)) (U)(*first);
        return cur;
    }
};

template<> struct __uninitialized_copy<true>
{
    template <typename T, typename U> static U __uninit_copy(T first, T last, U result)
    { return copy(first, last, result); }
};

template <typename T, typename T2> inline T2 uninitialized_copy(T first, T __last, T2 __result)
{
    typedef typename iterator_traits<T>::value_type _ValueType1;
    typedef typename iterator_traits<T2>::value_type _ValueType2;
    const bool __assignable = true;

    return __uninitialized_copy<__is_trivial(_ValueType1) && __is_trivial(_ValueType2)
                   && __assignable>::__uninit_copy(first, __last, __result);
}

template<typename T, typename U, typename V> inline U uninit_copy_a(T f, T l, U r, allocator<V>&)
{ return uninitialized_copy(f, l, r); }

template<typename T, typename T2, typename T3> inline T2
uninit_move_if_noexcept_a(T first, T last, T2 result, T3 &alloc)
{ return uninit_copy_a(first, last, result, alloc); }

template <typename T, typename U>
struct Vector_impl2 : public alloc_traits<U>::template rebind<T>::other
{
    typedef typename alloc_traits<U>::template rebind<T>::other A;
    T *start = 0;
    T *fin = 0;
    T *eos = 0;
    Vector_impl2() { }
    Vector_impl2(A const &a) : A(a) { }
};

template<typename T, typename U> struct Vector_base
{
    typedef typename alloc_traits<U>::template rebind<T>::other A;
    void _M_create_storage(size_t n);
    A& _M_get_Tp_allocator() { return *static_cast<A*>(&this->_M_impl); }
    const A& _M_get_Tp_allocator() const { return *static_cast<const A*>(&this->_M_impl); }
    U get_allocator() const { return U(_M_get_Tp_allocator()); }
    Vector_base() : _M_impl() { }
    Vector_base(const U &a) : _M_impl(a) { }
    Vector_base(size_t n) : _M_impl() { _M_create_storage(n); }
    Vector_base(size_t n, const U &a) : _M_impl(a) { _M_create_storage(n); }
    ~Vector_base(){_M_deallocate(_M_impl.start,_M_impl.eos - _M_impl.start); }
    Vector_impl2<T, U> _M_impl;
    T *_M_allocate(size_t n) { return n != 0 ? alloc_traits<A>::allocate(_M_impl, n) : 0; }
    void _M_deallocate(T *p, size_t n) { if (p) alloc_traits<A>::deallocate(_M_impl, p, n); }
};

template <typename V, typename W = allocator<V> > class vector2 : protected Vector_base<V, W>
{
    typedef Vector_base<V, allocator<V> > Base;
    typedef alloc_traits<typename Vector_base<V, allocator<V> >::A > _Alloc_traits;
protected:
    using Base::_M_allocate;
    using Base::_M_deallocate;
    using Base::_M_impl;
    using Base::_M_get_Tp_allocator;
public:
    typedef ni<V *, vector2> iterator;
    typedef ni<const V *, vector2> const_iterator;
    void _M_fill_initialize(size_t n, const V &value);
    vector2() : Base() { }
    explicit vector2(size_t n) : Base(n, W()) { _M_fill_initialize(n, V()); }
    vector2(const vector2 &x);
    V &front() { return *begin(); }
    ~vector2();
    void assign(size_t n, const V &val) { _M_fill_assign(n, val); }
    iterator begin() { return iterator(_M_impl.start); }
    const_iterator begin() const { return const_iterator(_M_impl.start); }
    iterator end() { return iterator(_M_impl.fin); }
    const_iterator end() const { return const_iterator(_M_impl.fin); }
    size_t size() const { return size_t(_M_impl.fin - _M_impl.start); }
    size_t max_size() const { return _Alloc_traits::max_size(_M_get_Tp_allocator()); }
    void reserve(size_t n);
    const V& operator[](size_t n) const { return *(_M_impl.start + n); }
    void push_back(const V &x);
    void pop_back() { --_M_impl.fin; _Alloc_traits::destroy(_M_impl, _M_impl.fin); }
    void insert(iterator pos, size_t n, const V &x) { _M_fill_insert(pos, n, x); }
    const V &at(size_t n) const { return (*this)[n]; }
    void clear() { _M_erase_at_end(this->_M_impl.start); }
protected:
    template<typename T> V* _M_allocate_and_copy(size_t n, T first, T last)
    {
        V *result = this->_M_allocate(n);
        uninitialized_copy_a(first, last, result, _M_get_Tp_allocator());
        return result;
    }

    template<typename T> void _M_range_initialize(T first, T last, input_iterator_tag)
    { for (; first != last; ++first) push_back(*first); }

    void _M_fill_assign(size_t n, const V &val);
    void _M_fill_insert(iterator pos, size_t n, const V &x);
    void _M_insert_aux(iterator position, const V &x);
    size_t _M_check_len(size_t n, const char * s) const;
    template<typename Ptr> Ptr _M_data_ptr(Ptr ptr) const { return ptr; }
};

enum _Ios_Seekdir
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

class ios_base2
{
public:
    typedef _Ios_Seekdir seekdir;
    typedef int openmode;
};

class streambuf2
{
protected:
    char *inBeg;
    char *inCur;
    char *inEnd;
    char *outBeg;
    char *outCur;
    char *outEnd;
    char *gptr() const { return NULL; }
    char *egptr() const { return NULL; }
    char *eback() const { return NULL; }
    void setg(char *gbeg, char *gnext, char *gend) { }
    virtual int seekoff(int off, ios_base2::seekdir way, int openmode) { return 0; }
    virtual int overflow(int c) { throw "Overflow not implemented"; return 0; }
    virtual int underflow() { throw "Underflow not implemented"; return 0; }
    //virtual int uflow() = 0;
    void gbump(int n) { }
public:
    char *_M_in_beg;
    char *_M_in_cur;
    int sputc(char c) { return overflow(c); }
    int snextc() { throw "Snextc not implemented"; return 0; }
    int sgetc() { return underflow(); }
    int sgetn(char *s, int n) { return 0; }
    int sungetc();
    int sbumpc() { return 0; }
    int pubsync() { return 0; }
    virtual ~streambuf2() { }
    int pubseekoff(int off, ios_base2::seekdir way, int mode) { return seekoff(off, way, mode); }
};

class ios2 : public ios_base2
{
protected:
    streambuf2 *_sb;
public:
    static const Util2::uint8_t binary = 1;
    static const Util2::uint8_t in = 2;
    streambuf2 *rdbuf() const { return _sb; }
    streambuf2 *rdbuf(streambuf2 *sb) { return _sb = sb; }
    ios2(streambuf2 *sb) : _sb(sb) { }
    ios2() { }
};

class filebuf2 : public streambuf2
{
    typedef Util2::uint32_t uint32_t;
    typedef Util2::int64_t int64_t;
    FILE *_fp;
    const uint32_t _put_back = 8;
    char _buffer[264];
public:
    filebuf2(FILE *fp) : _fp(fp) { }
    filebuf2() { }
    //fpos<mbstate_t> seekoff(int64_t off, ios2::seekdir way, ios2::openmode m);
    int overflow(int c) { return fputc(c, _fp); }
    filebuf2 *open(const char *fn, ios2::openmode m) { _fp = fopen(fn, "w"); return this; }
    filebuf2 *open(FILE *fp) { _fp = fp; return this; }
    int underflow();
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
    typedef Util2::uint16_t uint16_t;
protected:
    size_t _pos = 0;
    FILE *_fp;
    size_t _lastRead = 0;
    bool _eof = false;
public:
    istream2() { }
    istream2(streambuf2 *sb) : ios2(sb) { }
    istream2(FILE *fp) : _fp(fp) { }
    virtual ~istream2() { }
    //int peek() { int c = fgetc(_fp); ungetc(c, _fp); return c; }
    virtual istream2 &ignore(size_t n = 1, int d = '\n') { while (n--) get(); return *this; }
    virtual int get() { _pos++; return fgetc(_fp); }
    virtual int tellg() { return _pos; }
    int gcount() { return _lastRead; }
    operator void * () const { return (void *)!_eof; }
    virtual void getline(char *dest, size_t size);
    virtual void read(char *s, size_t length);
};

class fstream2
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
    filebuf2 _fb;
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
    ostream2 &print(const char *s) { while (*s) put(*s++); return *this; }
public:
    //ostream2& operator << (uint8_t u) { return *this; }
    ostream2& operator << (const string2 &s) { return print(s.c_str()); }
    ostream2& operator << (const char *s) { return print(s); }
    ostream2& operator << (const align &a) { return *this; }
    ostream2& operator << (const width2 &w) { _width = w; return *this; }
    ostream2& operator << (const fill2 &f) { _fill = f; return *this; }
    ostream2& operator << (const base2 &base) { _base = base; return *this; }
    ostream2& operator << (uint32_t u);
    ostream2 &write(const char *s, int n) { for (int i = 0; i < n; i++) put(s[i]); return *this; }
    virtual ~ostream2() { }
};

class ifpstream2 : public istream2
{
    filebuf2 _fb;
public:
    ifpstream2(FILE *fp) : istream2(&_fb) { _fb.open(fp); }
    int get() { return _fb.sgetc(); }
    int tellg() { return _fb.pubseekoff(0, ios_base2::seekdir::_S_beg, 0); }
};

class fpstream2 : public ostream2
{
    filebuf2 _fb;
public:
    fpstream2(FILE *fp) : ostream2(&_fb) { _fb.open(fp); }
};

class ofstream2 : public ostream2
{
    filebuf2 _fb;
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
    ofstream2() : ostream2(&_fb) { }
    ofstream2(const char *f) : ostream2(&_fb) { _fb.open(f, 0); }
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

template <class OI, class S, class T> OI fill_n2(OI first, S n, const T &val)
{
    for (;n > 0; --n)
        *first++ = val;

    return first;
}

template <typename T, size_t N> struct array2
{
    //typedef T Type[N];
    //typename array_traits2<T, N>::Type elems;
    T elems[N];
    //static constexpr T& ref2(const Type& t, size_t n) { return const_cast<T&>(t[n]); }
    //T *data() { return addressof(ref2(elems, 0)); }
    //T *begin() { return (T*)(data()); }
    constexpr size_t size() const { return N; }
    constexpr size_t max_size() const { return N; }
    constexpr bool empty() const { return size() == 0; }
    //void fill(const T &u) { fill_n2(begin(), size(), u); }
    //const T* data() const noexcept { return __addressof(ref2(elems, 0)); }
    //const T *begin() const { return const_iterator(data()); }
    //T *end() { return (T*)(data() + N); }
    //const T *end() const { return const_iterator(data() + N); }
    //const T *rbegin() const { return const_reverse_iterator(end()); }
    //const T *cbegin() const { return const_iterator(data()); }
    //const T *cend() const { return const_iterator(data() + N); }
    T& operator[](size_t n) { return const_cast<T&>(elems[n]); }
    //constexpr const T& operator[](size_t n) const { return const_cast<T&>(elems[n]); }
    T &at(size_t n) { return const_cast<T&>(elems[n]); }
    constexpr const T &at(size_t n) const { return const_cast<T&>(elems[n]); }
    void fill(const T &u) { for (size_t i = 0; i < size(); i++) at(i) = u; }
    //T &front() noexcept { return *begin(); }
    constexpr const T &front() const { return const_cast<T&>(elems[0]); }
    //T &back() noexcept { return N ? *(end() - 1) : *end(); }
};

namespace mystl
{
    template <class T> using vector = vector2<T>;
    template <typename T, size_t N> using array = array2<T, N>;
    template <Util2::size_t T> using bitset = bitset2<T>;
    typedef Util2::uint8_t uint8_t;
    typedef Util2::uint16_t uint16_t;
    typedef Util2::int32_t int32_t;
    typedef Util2::uint32_t uint32_t;
    typedef Util2::int64_t int64_t;
    typedef Util2::uint64_t uint64_t;
    void *memcpy(void *dest, const void *src, size_t n);
    char *strcpy(char *dest, const char *src);
    void *memset(void *s, const int c, const size_t n);
    void *memmove(char *dst, const char *src, uint32_t n);
    char *strncpy(char *dest, const char *src, size_t n);
    size_t strlen(const char *s);
    uint32_t strtol(const char *a, const char *b, int base);
    int strcmp(const char* s1, const char *s2);
    int strncmp(const char *s1, const char *s2, size_t n);
    template <typename T> using fpos = fpos2<T>;
    typedef string2 string;
    typedef fstream2 fstream;
    typedef ios2 ios;
    typedef istream2 istream;
    typedef ostream2 ostream;
    typedef ofstream2 ofstream;
    typedef ifstream2 ifstream;
    typedef ostringstream2 ostringstream;
    typedef streambuf2 streambuf;
    extern ifpstream2 cin;
    extern fpstream2 cout;
    extern fpstream2 cerr;
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
    template <typename T> inline const T &min(const T &a, const T &b) { return min2(a, b); }
    template <typename T> inline const T &max(const T &a, const T &b) { return a < b ? b : a; }
    typedef mbstate_t2 mbstate_t;
    //ostream& operator << (ostream& os, char c);
};

#include "mystl.tcc"

#endif


