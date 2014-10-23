#ifndef _MYSTL_TCC_
#define _MYSTL_TCC_

template<typename T> inline bool operator==(const reverse_iterator<T> &x,
           const reverse_iterator<T> &y)
{
    return x.base() == y.base();
}

template<typename T, typename U, typename V> U transform(T first, T last, U result, V unary_op)
{
    for (; first != last; ++first, ++result)
        *result = unary_op(*first);

    return result;
}

template <typename T, typename U, typename V>
    inline V set_intersection(T first1, T last1, U first2, U last2, V result)
{
    return __set_intersection(first1, last1, first2, last2, result, __iter_less_iter());
}

template<typename T, typename U, typename V, typename W>
    V __set_intersection(T __first1, T __last1,
               U __first2, U __last2,
               V __result, W __comp)
{
    while (__first1 != __last1 && __first2 != __last2)
        if (__comp(__first1, __first2))
            ++__first1;
        else if (__comp(__first2, __first1))
            ++__first2;
        else
        {
            *__result = *__first1;
            ++__first1;
            ++__first2;
            ++__result;
        }
        return __result;
}


#endif


