#ifndef _HASHER_H_
#define _HASHER_H_
#include "mystl.h"
#include <endian.h>
//#include <stdlib.h>

class Hash
{
    uint32_t _h0, _h1, _h2, _h3;
public:
    void read(const char *hash);
    Hash(const char *hash) { read(hash); }
    void reset() { _h0 = 0x67452301; _h1 = 0xefcdab89; _h2 = 0x98badcfe; _h3 = 0x10325476; }
    Hash() { reset(); }
    uint32_t h0() const { return _h0; }
    uint32_t h1() const { return _h1; }
    uint32_t h2() const { return _h2; }
    uint32_t h3() const { return _h3; }
    void add(Hash &h) { _h0 += h.h0(); _h1 += h.h1(); _h2 += h.h2(); _h3 += h.h3(); }
    void dump(ostream &os) const;
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
    bool isEqual(Hash h) const { return _h0 == h._h0&&_h1==h._h1&&_h2==h._h2 && _h3 == h._h3; }
    Hash(uint32_t a, uint32_t b, uint32_t c, uint32_t d) : _h0(a), _h1(b), _h2(c), _h3(d) { }
};

class Chunk
{
    uint32_t _w[16];
    uint32_t const leftRotate(uint32_t x, uint32_t c) { return x << c | x >> 32 - c; }
    uint32_t to_int32(const uint8_t *bytes) const;
public:
    void read(const uint8_t *msg) { for (int i = 0; i < 16; i++) _w[i] = to_int32(msg + i * 4); }
    Hash calc(Hash &hash);
    uint32_t w(uint32_t g) const { return _w[g]; }
    void dump(ostream &os) const;
    void fillTail(uint32_t size) { _w[14] = size * 8; _w[15] = size >> 29; }
    void clear() { for (int i = 0; i < 16; i++) _w[i] = 0; }
};

class Hasher
{
    Hash _hash;
public:
    void reset() { _hash.reset(); }
    void stream(istream &is);
    void stream(const char *s, size_t size);
    Hash hash() const { return _hash; }
};

#endif



