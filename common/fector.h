#ifndef _FECTOR_H_
#define _FECTOR_H_
#include "common.h"

template<class T> class Fector
{
    uint32_t _size;
    uint32_t _pos = 0;
    T *_buf;
    T _max(T a, T b) { return a > b ? a : b; }
    T _min(T a, T b) { return a < b ? a : b; }
public:
    typedef T *iterator;
    typedef T *const_iterator;

    Fector& operator= (const Fector &f)
    {
        _pos = f._pos;
        _size = f._size;
        _buf = new T[_size];
        for (uint32_t i = 0; i < _size; i++) _buf[i] = f._buf[i];
        return *this;
    }

    Fector(uint32_t size) : _size(size), _buf(new T[size]) { }

    Fector(const Fector &f) : _size(f._size), _buf(new T[_size])
    { for (uint32_t i = 0; i < _size; i++) _buf[i] = f._buf[i]; }

    ~Fector() { delete[] _buf; }
    uint32_t size() const { return _size; }
    T at(uint32_t i) const { return _buf[i]; }
    T set(uint32_t i, T val) { return _buf[i] = val; }

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

    bool isFull() const { return _pos >= _size; }
    void testFull() const { if (isFull()) throw "Fector is full"; }
    T max() { return max(_size); }
    T min() { return min(_size); }
    T *begin() const { return _buf; }
    T *end() const { return _buf + _size; }
    void push_back(const T &x) { testFull(); _buf[_pos++] = x; }
    T &operator[](uint32_t i) { return _buf[i]; }
};

class Fugt : public Fector<uint8_t>
{
public:
    Fugt(uint32_t size) : Fector<uint8_t>(size) { }
    void dump(ostream &os) const;
    string toString() const { ostringstream o; dump(o); return o.str(); }
};

#endif


