#ifndef _MYSTL_TCC_
#define _MYSTL_TCC_

template <typename T> void Vector_base<T>::_M_create_storage(size_t n)
{
    _M_impl.start = _M_allocate(n);
    _M_impl.fin = _M_impl.start;
    _M_impl.eos = _M_impl.start + n;
}

template <typename T, typename T2> void Util2::construct(T *p, const T2 &value)
{
    ::new(static_cast<void*>(p)) T(value);
}

template <typename T, typename U> void Util2::sort(T first, T last, U comp)
{
    for (T i = first; i != last; i++)
        for (T j = first; j < i; j++)
            if (comp(*i, *j))
                iter_swap(i, j);
}

template<typename T> inline T* Util2::addressof(T& r)
{ return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(r))); }

template<typename T> void Util2::reverse2(T first, T last)
{
    if (first == last--) return;
    while (first < last) swap(*first++, *last++);
    //while (first < last) iter_swap(first++, last++);
}


template <typename V> vector2<V>::~vector2()
{ Util2 u; u.destroy(_M_impl.start, _M_impl.fin, _M_get_Tp_allocator()); }

template <typename V> void vector2<V>::push_back(const V &x)
{
    if (_M_impl.fin != _M_impl.eos)
        atraits::construct(_M_impl, _M_impl.fin++, x);
    else
        _M_insert_aux(end(), x);
}

template <typename T> vector2<T>::vector2(const vector2 &x)
  :
    Base(x.size(), atraits::_S_select_on_copy(x._M_get_Tp_allocator()))
{
    Util2 u;
    _M_impl.fin = u.uninitialized_copy(x.begin(), x.end(), _M_impl.start);
}

template <typename T, typename T2> T2 Util2::uninitialized_copy(T first, T last, T2 result)
{
    T2 cur = result;
    for (; first != last; ++first, ++cur) construct(cur, *first);
    return cur;
}

template <typename T> size_t vector2<T>::_M_check_len(size_t n, const char *s) const
{
    Util2 u;
    if (max_size() - size() < n) throw;
    const size_t __len = size() + u.max(size(), n);
    return (__len < size() || __len > max_size()) ? max_size() : __len;
}

template<typename T, typename BI2> BI2 Util2::copy_move_back_a(T f, T l, BI2 result)
{
    while (f != l) *--result = *--l;
    return result;
}

template<typename T, typename U> void Util2::destroy(T first, T last, U &alloc)
{
    for (; first != last; ++first)
    alloc.destroy(first);
}

template <typename T> void vector2<T>::reserve(size_t n)
{
    Util2 u;

    if (n > this->max_size())
        throw;

    if (this->capacity() < n)
    {
        const size_t __old_size = size();
        T *tmp = _M_allocate_and_copy(n,_M_impl.start,_M_impl.fin);
        u.destroy(_M_impl.start, _M_impl.fin, _M_get_Tp_allocator());
        _M_deallocate(_M_impl.start, _M_impl._M_end_of_storage - _M_impl.start);
        _M_impl.start = tmp;
        _M_impl.fin = tmp + __old_size;
        _M_impl._M_end_of_storage = _M_impl.start + n;
    }
}

template<typename T> void vector2<T>::_M_insert_aux(iterator pos, const T &x)
{
    Util2 u;

    if (_M_impl.fin != _M_impl.eos)
    {
        atraits::construct(_M_impl, _M_impl.fin, *(_M_impl.fin - 1));
        ++_M_impl.fin;
        T x_copy = x;
        u.copy_move_back_a(nb(mb(pos.base())), nb(mb(_M_impl.fin - 2)), nb(_M_impl.fin - 1));
        *pos = x_copy;
    }
    else
    {
        const size_t __len = _M_check_len(size_t(1), "vector::_M_insert_aux");
        const size_t __elems_before = pos - begin();
        T *ns(_M_allocate(__len));
        T *nf(ns);
        atraits::construct(_M_impl, ns + __elems_before, x);
        nf = u.uninitialized_copy(_M_impl.start, pos.base(), ns);
        nf = u.uninitialized_copy(pos.base(), _M_impl.fin, nf + 1);
        u.destroy(_M_impl.start, _M_impl.fin, _M_get_Tp_allocator());
        _M_deallocate(_M_impl.start, _M_impl.eos - _M_impl.start);
        _M_impl.start = ns;
        _M_impl.fin = nf;
        _M_impl.eos = ns + __len;
    }
}

namespace mystl
{

template <class T> T max_element(T first, T last)
{
    if (first == last) return last;
    T largest = first;

    while (++first != last)
        if (*largest < *first)
            largest = first;

    return largest;
};

template <class I, class N, class T> I fill_n(I first, N n, const T &v)
{
    while (n > 0)
    {
        *first = v;
        ++first;
        --n;
    }

    return first;
}

}

#endif


