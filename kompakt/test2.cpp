#include <iostream>
#include <vector>
using namespace std;

template <class T> class vector2
{
    T *_buf;
    size_t _capacity;
    size_t _size;
public:
    typedef T * iterator;
    vector2() : _capacity(100), _size(0) { _buf = new T[100]; }
    void push_back(const T &a) { _buf[_size++] = a; }
    iterator begin() { return _buf; }
    iterator end() { return _buf + _size; }
};

class Nester
{
    vector2<std::string> foo;
};

void test()
{
    vector<Nester> bar;
    Nester nester1;
    bar.push_back(nester1);
}

int main()
{
    test();
    return 0;
}


