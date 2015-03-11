#include <iostream>
#include <stdint.h>
using namespace std;

#if 0
template <class T> class MyVector
{
    T _buf[100];
    uint32_t _size = 0;
public:
    typedef T *iterator;
    void push_back(const T &a) { _buf[_size++] = a; }
    iterator begin() { return _buf; }
    iterator end() { return _buf + _size; }
};
#else
template <class T> class MyVector
{
    T *_buf;
    uint32_t _size = 0;
public:
    typedef T *iterator;
    MyVector() { _buf = new T[100]; }
    ~MyVector() { delete[] _buf; }
    void push_back(const T &a) { _buf[_size++] = a; }
    iterator begin() { return _buf; }
    iterator end() { return _buf + _size; }
};
#endif

int main()
{
    MyVector<uint32_t> v1;
    v1.push_back(5800);
    v1.push_back(63241);
    v1.push_back(5432);
    v1.push_back(900);

    for (MyVector<uint32_t>::iterator it = v1.begin(); it != v1.end(); it++)
        cout << *it << "\n";

    MyVector<string> v2;
    v2.push_back("Hello");
    v2.push_back("world");

    for (MyVector<string>::iterator it = v2.begin(); it != v2.end(); it++)
        cout << *it << "\n";

    return 0;
}


