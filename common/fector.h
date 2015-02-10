#ifndef _FECTOR_H_
#define _FECTOR_H_
#include <stdint.h>
#include <stdexcept>
#include <iostream>
#include <sstream>
using namespace std;

template<class T> class Fector
{
    uint32_t _size;
    T *_buf;
    T _max(T a, T b) { return a > b ? a : b; }
    T _min(T a, T b) { return a < b ? a : b; }
public:
    Fector(uint32_t size) : _size(size), _buf(new T[size]) { }

    Fector(const Fector &f) : _size(f._size), _buf(new T[_size])
    { for (uint32_t i = 0; i < _size; i++) _buf[i] = f._buf[i]; }

    ~Fector() { delete[] _buf; }
    uint32_t size() const { return _size; }

    T at(uint32_t i) const
    {
        if (i >= _size) throw out_of_range("Fector");
        return _buf[i];
    }

    T set(uint32_t i, T val)
    {
        if (i >= _size) throw out_of_range("Fector");
        return _buf[i] = val;
    }

    T max(uint32_t range)
    {
        T a = 0;
        for (uint32_t i = 0; i < range; i++) a = _max(_buf[i], a);
        return a;
    }

    T min(uint32_t range)
    {
        T a = 0;
        for (uint32_t i = 0; i < range; i++) a = _min(_buf[i], a);
        return a;
    }

    T max() { return max(_size); }
    T min() { return min(_size); }
};

class Fugt : public Fector<uint8_t>
{
public:
    Fugt(uint32_t size) : Fector<uint8_t>(size) { }
    void dump(ostream &os) const { for (uint32_t i = 0; i < size(); i++) os << (int)at(i) << " "; }
    string toString() const { ostringstream o; dump(o); return o.str(); }
};

#endif


