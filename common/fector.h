#ifndef _FECTOR_H_
#define _FECTOR_H_
#include <stdint.h>
#include <stdexcept>

template<class T> class Fector
{
    uint32_t _size;
    T *_buf;
public:
    Fector(uint32_t size) : _size(size), _buf(new T[size]) { }

    Fector(const Fector &f) : _size(f._size), _buf(new T[_size])
    { for (uint32_t i = 0; i < _size; i++) _buf[i] = f._buf[i]; }

    ~Fector() { delete[] _buf; }
    uint32_t size() const { return _size; }
    T at(uint32_t i) const { return _buf[i]; }

    void set(uint32_t i, T val)
    {
        _buf[i] = val;
    }
};

#endif


