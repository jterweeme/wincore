#ifndef _MYSTL_TCC_
#define _MYSTL_TCC_

template <typename T, typename U> void Vector_base<T, U>::_M_create_storage(size_t n)
{
    _M_impl.start = _M_allocate(n);
    _M_impl.fin = _M_impl.start;
    _M_impl.eos = _M_impl.start + n;
}

template<typename T, typename U, typename V> U transform(T first, T last, U result, V unary_op)
{
    for (; first != last; ++first, ++result) *result = unary_op(*first);
    return result;
}

template <typename V> void vector2<V>::_M_fill_initialize(size_t n, const V &value)
{ _M_impl.fin = _M_impl.eos; }

template <typename V> vector2<V>::~vector2()
{ _Destroy2(_M_impl.start, _M_impl.fin, _M_get_Tp_allocator()); }

template <typename V> void vector2<V>::push_back(const V &x)
{
    if (_M_impl.fin != _M_impl.eos)
        _Alloc_traits::construct(_M_impl, _M_impl.fin++, x);
    else
        _M_insert_aux(end(), x);
}

template <typename T> void reverse(T first, T last, random_access_iterator_tag)
{
    if (first == last--) return;
    while (first < last) iter_swap(first++, last++);
}

template <typename T> vector2<T>::vector2(const vector2 &x)
  :
    Base(x.size(), _Alloc_traits::_S_select_on_copy(x._M_get_Tp_allocator()))
{
    _M_impl.fin = uninit_copy_a(x.begin(), x.end(), _M_impl.start, _M_get_Tp_allocator());
}

template <typename T> size_t vector2<T>::_M_check_len(size_t n, const char *s) const
{
    if (max_size() - size() < n) throw;
    const size_t __len = size() + max2(size(), n);
    return (__len < size() || __len > max_size()) ? max_size() : __len;
}

template <typename T> void vector2<T>::reserve(size_t n)
{
    if (n > this->max_size())
        throw;

    if (this->capacity() < n)
    {
        const size_t __old_size = size();
        T *tmp = _M_allocate_and_copy(n,_M_impl.start,_M_impl.fin);
        _Destroy2(_M_impl.start, _M_impl.fin, _M_get_Tp_allocator());
        _M_deallocate(_M_impl.start, _M_impl._M_end_of_storage - _M_impl.start);
        _M_impl.start = tmp;
        _M_impl.fin = tmp + __old_size;
        _M_impl._M_end_of_storage = _M_impl.start + n;
    }
}

template<typename T> void
vector2<T>::_M_fill_insert(iterator pos, size_t n, const T &x)
{
    if (size_t(_M_impl._M_end_of_storage - _M_impl.fin) >= n)
    {
        T x_copy = x;
        const size_t elems_after = end() - pos;
        T *old_finish(_M_impl.fin);

        if (elems_after > n)
        {
            __uninitialized_move_a(_M_impl.fin - n,_M_impl.fin,_M_impl.fin,_M_get_Tp_allocator());
            _M_impl.fin += n;
            (pos.base(), old_finish - n, old_finish);
            fill(pos.base(), pos.base() + n, x_copy);
        }
        else
        {
            __uninitialized_fill_n_a(_M_impl.fin, n - elems_after, x_copy, _M_get_Tp_allocator());
            _M_impl.fin += n - elems_after;
            __uninitialized_move_a(pos.base(), old_finish, _M_impl.fin, _M_get_Tp_allocator());
            _M_impl.fin += elems_after;
            fill(pos.base(), old_finish, x_copy);
        }
    }
    else
    {
        const size_t __len = _M_check_len(n, "vector::_M_fill_insert");
        const size_t __elems_before = pos - begin();
        T *ns(_M_allocate(__len));
        T *nf(ns);
        __uninitialized_fill_n_a(ns + __elems_before, n, x, _M_get_Tp_allocator());
        nf = 0;
        nf = uninit_move_if_noexcept_a(_M_impl.start, pos.base(), ns, _M_get_Tp_allocator());
        nf += n;
        nf = uninit_move_if_noexcept_a(pos.base(), _M_impl.fin, nf, _M_get_Tp_allocator());
        _Destroy2(_M_impl.start, _M_impl.fin, _M_get_Tp_allocator());
        _M_deallocate(_M_impl.start, _M_impl._M_end_of_storage - _M_impl.start);
        _M_impl.start = ns;
        _M_impl.fin = nf;
        _M_impl._M_end_of_storage = ns + __len;
    }
}

template<typename T> void vector2<T>::_M_insert_aux(iterator pos, const T &x)
{
    if (_M_impl.fin != _M_impl.eos)
    {
        _Alloc_traits::construct(_M_impl, _M_impl.fin, *(_M_impl.fin - 1));
        ++_M_impl.fin;
        T x_copy = x;
        copy_move_back_a2<imi<T>::__value>(mb(pos.base()), mb(_M_impl.fin - 2), _M_impl.fin - 1);
        *pos = x_copy;
    }
    else
    {
        const size_t __len = _M_check_len(size_t(1), "vector::_M_insert_aux");
        const size_t __elems_before = pos - begin();
        T *ns(_M_allocate(__len));
        T *nf(ns);
        _Alloc_traits::construct(_M_impl, ns + __elems_before, x);
        nf = 0;
        nf = uninit_move_if_noexcept_a(_M_impl.start, pos.base(), ns, _M_get_Tp_allocator());
        ++nf;
        nf = uninit_move_if_noexcept_a(pos.base(), _M_impl.fin, nf, _M_get_Tp_allocator());
        _Destroy2(_M_impl.start, _M_impl.fin, _M_get_Tp_allocator());
        _M_deallocate(_M_impl.start, _M_impl.eos - _M_impl.start);
        _M_impl.start = ns;
        _M_impl.fin = nf;
        _M_impl.eos = ns + __len;
    }
}


#endif


