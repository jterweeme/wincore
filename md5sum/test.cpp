#include <iostream>
#include <new>
using namespace std;

template <class T> class vector2
{
    T *_buf;
    size_t _capacity;
    size_t _size;
public:
    typedef T * iterator;
    vector2(const vector2 &o) : _buf(o._buf), _capacity(o._capacity), _size(o._size) { }
    vector2() : _capacity(100), _size(0) { std::cout << "c"; _buf = new T[100]; }
    void push_back(const T &a)
    {
        new (static_cast<void *>(_buf + _size)) T(a);
        ++_size;
    }
    iterator begin() { return _buf; }
    iterator end() { return _buf + _size; }
    ~vector2() { std::cout << "d"; delete[] _buf; }
};

class Nester
{
    vector2<std::string> foo;
};

int main(int argc, char **argv)
{
    vector2<Nester> bar;
    Nester nester1;
    bar.push_back(nester1);
    return 0;
}


