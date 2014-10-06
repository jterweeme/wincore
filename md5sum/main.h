#ifndef _MAIN_H_
#define _MAIN_H_
#include "common.h"

class Util
{
public:
    void to_bytes(uint32_t val, uint8_t *bytes);
    uint32_t to_int32(const uint8_t *bytes);
};

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
    string toString();
    void dump(ostream &os);

    Hash(uint32_t h0, uint32_t h1, uint32_t h2, uint32_t h3)
      : _h0(h0), _h1(h1), _h2(h2), _h3(h3)
    { }
};

typedef vector<string> Files;

class Options
{
    bool _check;
    Files _files;
public:
    Options() : _check(false) { }
    bool check() const { return _check; }
    Files &files() { return _files; }
    void parse(int argc, char **argv);
};

class Paar
{
    Hash _hash;
    string _fn;
public:
    Paar() { }
    Paar(Hash &hash, string &fn) : _hash(hash), _fn(fn) { }
    void read(istream &is);
    void dump(ostream &os);
};

class Paars : public vector<Paar>
{
public:
    void dump(ostream &os);
    void check();
};

class Chunk
{
    uint32_t _w[16];
public:
    void read(const uint8_t *msg);
    Hash calc(Hash &hash);
    uint32_t w(uint32_t g) const { return _w[g]; }
};

class App
{
    Hash _hash;
    Options _options;
    Paars _paars;
    void md5(uint8_t *msg, size_t new_len);
public:
    static uint32_t const LEFTROTATE(uint32_t x, uint32_t c) { return x << c | x >> 32 - c; }
    void checkFile(const char *fn);
public:
    int run(int argc, char **argv);
};


#endif



