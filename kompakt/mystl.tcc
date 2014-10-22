#ifndef _MYSTL_TCC_
#define _MYSTL_TCC_

template<typename T> inline bool operator==(const reverse_iterator<T> &x,
           const reverse_iterator<T> &y)
{
    return x.base() == y.base();
}

template<typename _InputIterator, typename _OutputIterator, typename _UnaryOperation>
    _OutputIterator transform(_InputIterator __first, _InputIterator __last,
    _OutputIterator __result, _UnaryOperation __unary_op)
{
    for (; __first != __last; ++__first, ++__result)
        *__result = __unary_op(*__first);

    return __result;
}

#endif


