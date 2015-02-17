#ifndef _MYSTL_TCC_
#define _MYSTL_TCC_

template <typename T, typename U> void Vector_base<T, U>::_M_create_storage(size_t n)
{
    this->_M_impl._M_start = this->_M_allocate(n);
    this->_M_impl._M_finish = this->_M_impl._M_start;
    this->_M_impl._M_end_of_storage = this->_M_impl._M_start + n;
}

template<typename T, typename U, typename V> U transform(T first, T last, U result, V unary_op)
{
    for (; first != last; ++first, ++result) *result = unary_op(*first);
    return result;
}

template <typename V, typename W> void vector2<V, W>::_M_fill_initialize(size_t n,
    const value_type& __value)
{
    __uninitialized_fill_n_a(this->_M_impl._M_start, n, __value, _M_get_Tp_allocator());
    this->_M_impl._M_finish = this->_M_impl._M_end_of_storage;
}

template <typename V, typename W> vector2<V, W>::~vector2()
{
    _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
}

template <typename V, typename W> void vector2<V, W>::push_back(const value_type& x)
{
    if (_M_impl._M_finish != _M_impl._M_end_of_storage)
        _Alloc_traits::construct(_M_impl, _M_impl._M_finish++, x);
    else
        _M_insert_aux(end(), x);
}

template <typename T> void reverse(T __first, T __last, random_access_iterator_tag)
{
    if (__first == __last)
        return;

    --__last;

    while (__first < __last)
    {
        iter_swap(__first, __last);
        ++__first;
        --__last;
    }
}

template <typename T, typename A> vector2<T, A>::vector2(size_t n,
    const value_type &value, const allocator_type &a)
  : _Base(n, a)
{
    _M_fill_initialize(n, value);
}

template <typename T, typename U> void Vector_impl2<T, U>::_M_swap_data(Vector_impl2 &x)
{
    swap(_M_start, x._M_start);
    swap(_M_finish, x._M_finish);
    swap(_M_end_of_storage, x._M_end_of_storage);
}

template <typename T, typename A> vector2<T, A>::vector2(const vector2 &x)
  :
    _Base(x.size(), _Alloc_traits::_S_select_on_copy(x._M_get_Tp_allocator()))
{
    _M_impl._M_finish = uninitialized_copy_a(x.begin(), x.end(), _M_impl._M_start,
                _M_get_Tp_allocator());
}

template <typename T, typename U> typename vector2<T, U>::iterator
            vector2<T, U>::insert(iterator position, const value_type &x)
{
    const size_t __n = position - begin();

    if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage && position == end())
    {
        _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish, x);
        ++this->_M_impl._M_finish;
    }
    else
    {
        _M_insert_aux(position, x);
    }
    return iterator(this->_M_impl._M_start + __n);
}

template <typename T, typename A> size_t vector2<T, A>::_M_check_len(size_t n, const char *s) const
{
    if (max_size() - size() < n) throw;
    const size_t __len = size() + max(size(), n);
    return (__len < size() || __len > max_size()) ? max_size() : __len;
}

template <typename T, typename _Alloc> void vector2<T, _Alloc>::reserve(size_t n)
{
    if (n > this->max_size())
        throw;

    if (this->capacity() < n)
    {
        const size_t __old_size = size();
        pointer tmp = _M_allocate_and_copy(n,_M_impl._M_start,_M_impl._M_finish);
        _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
        _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
        _M_impl._M_start = tmp;
        _M_impl._M_finish = tmp + __old_size;
        _M_impl._M_end_of_storage = _M_impl._M_start + n;
    }
}


template <typename T, typename A> void vector2<T, A>::swap(vector2 &x)
{
    _M_impl._M_swap_data(x._M_impl);
    _Alloc_traits::_S_on_swap(_M_get_Tp_allocator(), x._M_get_Tp_allocator());
}

template<typename T, typename _Alloc> void
    vector2<T, _Alloc>::_M_fill_insert(iterator __position, size_t n, const value_type& __x)
{
    if (n != 0)
    {
        if (size_t(this->_M_impl._M_end_of_storage - this->_M_impl._M_finish) >= n)
        {
            value_type __x_copy = __x;
            const size_t __elems_after = end() - __position;
            pointer __old_finish(this->_M_impl._M_finish);

            if (__elems_after > n)
            {
                __uninitialized_move_a(this->_M_impl._M_finish - n,
                       this->_M_impl._M_finish, this->_M_impl._M_finish, _M_get_Tp_allocator());

                this->_M_impl._M_finish += n;
                (__position.base(), __old_finish - n, __old_finish);
                fill(__position.base(), __position.base() + n, __x_copy);
            }
            else
            {
                __uninitialized_fill_n_a(this->_M_impl._M_finish,
                        n - __elems_after, __x_copy, _M_get_Tp_allocator());

                this->_M_impl._M_finish += n - __elems_after;

                __uninitialized_move_a(__position.base(), __old_finish,
                          this->_M_impl._M_finish, _M_get_Tp_allocator());

                this->_M_impl._M_finish += __elems_after;
                fill(__position.base(), __old_finish, __x_copy);
            }
        }
        else
        {
            const size_t __len = _M_check_len(n, "vector::_M_fill_insert");
            const size_t __elems_before = __position - begin();
            pointer __new_start(this->_M_allocate(__len));
            pointer __new_finish(__new_start);

            try
            {
                __uninitialized_fill_n_a(__new_start + __elems_before,
                        n, __x, _M_get_Tp_allocator());
                __new_finish = 0;

                __new_finish = __uninitialized_move_if_noexcept_a
                    (this->_M_impl._M_start, __position.base(),
                    __new_start, _M_get_Tp_allocator());

                __new_finish += n;

            __new_finish
            = __uninitialized_move_if_noexcept_a
            (__position.base(), this->_M_impl._M_finish,
             __new_finish, _M_get_Tp_allocator());
            }
            catch(...)
            {
                if (!__new_finish)
                    _Destroy(__new_start + __elems_before,
                        __new_start + __elems_before + n, _M_get_Tp_allocator());
                else
                    _Destroy(__new_start, __new_finish, _M_get_Tp_allocator());

                _M_deallocate(__new_start, __len);
            }
            _Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());

            _M_deallocate(this->_M_impl._M_start,
                this->_M_impl._M_end_of_storage - this->_M_impl._M_start);

            this->_M_impl._M_start = __new_start;
            this->_M_impl._M_finish = __new_finish;
            this->_M_impl._M_end_of_storage = __new_start + __len;
        }
    }
}

template<typename T, typename U>
    vector2<T, U> &vector2<T, U>::operator=(const vector2<T, U>& __x)
{
    if (&__x != this)
    {
        const size_t __xlen = __x.size();
        if (__xlen > capacity())
        {
            pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
            _Destroy(this->_M_impl._M_start, this->_M_impl._M_finish, _M_get_Tp_allocator());

            _M_deallocate(this->_M_impl._M_start, this->_M_impl._M_end_of_storage
                        - this->_M_impl._M_start);

            this->_M_impl._M_start = __tmp;
            this->_M_impl._M_end_of_storage = this->_M_impl._M_start + __xlen;
        }
        else if (size() >= __xlen)
        {
            _Destroy(copy(__x.begin(), __x.end(), begin()), end(), _M_get_Tp_allocator());
        }
        else
        {
            copy(__x._M_impl._M_start, __x._M_impl._M_start + size(), this->_M_impl._M_start);

            uninitialized_copy_a(__x._M_impl._M_start + size(), __x._M_impl._M_finish,
                      this->_M_impl._M_finish, _M_get_Tp_allocator());
        }
        this->_M_impl._M_finish = this->_M_impl._M_start + __xlen;
    }
    return *this;
}

template<typename T, typename U> void
        vector2<T, U>::_M_insert_aux(iterator __position, const T &x)
{
    if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
    {
        _Alloc_traits::construct(_M_impl, _M_impl._M_finish, *(_M_impl._M_finish  - 1));
        ++_M_impl._M_finish;
        T __x_copy = x;
        copy_backward(__position.base(), _M_impl._M_finish - 2, _M_impl._M_finish - 1);
        *__position = __x_copy;
    }
    else
    {
        const size_t __len = _M_check_len(size_t(1), "vector::_M_insert_aux");
        const size_t __elems_before = __position - begin();
        pointer __new_start(this->_M_allocate(__len));
        pointer __new_finish(__new_start);

        try
        {
            _Alloc_traits::construct(this->_M_impl, __new_start + __elems_before, x);
            __new_finish = 0;

            __new_finish = __uninitialized_move_if_noexcept_a(this->_M_impl._M_start,
                    __position.base(), __new_start, _M_get_Tp_allocator());

            ++__new_finish;

            __new_finish = __uninitialized_move_if_noexcept_a(__position.base(),
                    this->_M_impl._M_finish, __new_finish, _M_get_Tp_allocator());
        }
        catch (...)
        {
            if (!__new_finish)
                _Alloc_traits::destroy(this->_M_impl, __new_start + __elems_before);
            else
                _Destroy(__new_start, __new_finish, _M_get_Tp_allocator());

            _M_deallocate(__new_start, __len);
            throw;
        }

        _Destroy(_M_impl._M_start, _M_impl._M_finish, _M_get_Tp_allocator());
        _M_deallocate(_M_impl._M_start, _M_impl._M_end_of_storage - _M_impl._M_start);
        _M_impl._M_start = __new_start;
        _M_impl._M_finish = __new_finish;
        _M_impl._M_end_of_storage = __new_start + __len;
    }
}


#endif


