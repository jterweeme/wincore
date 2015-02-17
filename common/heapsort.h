#ifndef _HEAPSORT_H_
#define _HEAPSORT_H_

template<typename T> struct _Iter_comp_val
{
    T _M_comp;
    _Iter_comp_val(T __comp) : _M_comp(__comp) { }

    template <typename U, typename V> bool operator()(U it, V& val)
    { return bool(_M_comp(*it, val)); }
};

template<typename T> inline _Iter_comp_val<T> __iter_comp_val(T comp)
{ return _Iter_comp_val<T>(comp); }

template <typename T> inline _Iter_comp_val<T> __iter_comp_val(_Iter_comp_iter<T> comp)
{ return _Iter_comp_val<T>(comp._M_comp); }


template<typename T> inline void reverse(T first, T last)
{
    reverse(first, last, __iterator_category(first));
}

template<typename I, typename D, typename T, typename C> void push_heap(I first,
        D holeIndex, D topIndex, T value, C comp)
{
    D parent = (holeIndex - 1) / 2;

    while (holeIndex > topIndex && comp(first + parent, value))
    {
        *(first + holeIndex) = (*(first + parent));
        holeIndex = parent; 
        parent = (holeIndex - 1) / 2;
    }

    *(first + holeIndex) = (value);
}

template<typename T, typename C> inline void pop_heap(T first, T last, T result, C comp)
{
    typedef typename iterator_traits<T>::value_type _ValueType;
    typedef typename iterator_traits<T>::difference_type _DistanceType;
    _ValueType __value = (*result);
    *result = (*first);
    adjust_heap(first, _DistanceType(0), _DistanceType(last - first), (__value), comp);
}

template<typename T, typename U, typename T4, typename T5> void
    adjust_heap(T first, U holeIndex, U len, T4 value, T5 comp)
{
    const U topIndex = holeIndex;
    U secondChild = holeIndex;

    while (secondChild < (len - 1) / 2)
    {
        secondChild = 2 * (secondChild + 1);

        if (comp(first + secondChild, first + (secondChild - 1)))
            secondChild--;

        *(first + holeIndex) = (*(first + secondChild));
        holeIndex = secondChild;
    }

    if ((len & 1) == 0 && secondChild == (len - 2) / 2)
    {
        secondChild = 2 * (secondChild + 1);
        *(first + holeIndex) = (*(first + (secondChild - 1)));
        holeIndex = secondChild - 1;
    }

    push_heap(first, holeIndex, topIndex, (value), __iter_comp_val(comp));
}


template<typename I, typename C> void __make_heap(I first, I last, C comp)
{
    typedef typename iterator_traits<I>::value_type _ValueType;
    typedef typename iterator_traits<I>::difference_type _DistanceType;
    if (last - first < 2) return;
    const _DistanceType __len = last - first;
    _DistanceType __parent = (__len - 2) / 2;

    while (true)
    {
        _ValueType __value = (*(first + __parent));
        adjust_heap(first, __parent, __len, (__value), comp);
        if (__parent == 0) return;
        __parent--;
    }
}


template <typename T> inline void make_heap(T first, T last)
{
    __make_heap(first, last, __iter_less_iter());
}

template<typename T, typename U> inline void make_heap(T __first, T __last, U __comp)
{
    __make_heap(__first, __last, __iter_comp_iter(__comp));
}

template<typename T, typename U> inline void sort_heap(T first, T last, U comp)
{
    while (last - first > 1)
    {
        --last;
        pop_heap(first, last, last, __iter_comp_iter(comp));
    }
}


#endif


