#ifndef _INFLATE_H_
#define _INFLATE_H_
#if 1
#include "common.h"
#else
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <algorithm>
#include <sstream>
#include <vector>
#endif
//#include "common.h"
//#include "fector.h"

template<class T> class FectorTemp
{
    uint32_t _size;
    uint32_t _pos = 0;
    T *_buf;
    T _max(T a, T b) { return a > b ? a : b; }
    T _min(T a, T b) { return a < b ? a : b; }
public:
    typedef T *iterator;
    typedef T *const_iterator;
    FectorTemp(uint32_t size) : _size(size), _buf(new T[size]) { }

    FectorTemp(const FectorTemp &f) : _size(f._size), _buf(new T[_size])
    { for (uint32_t i = 0; i < _size; i++) _buf[i] = f._buf[i]; }

    ~FectorTemp() { delete[] _buf; }
    uint32_t size() const { return _size; }
    T at(uint32_t i) const { return _buf[i]; }
    T set(uint32_t i, T val) { return _buf[i] = val; }
    T &operator[](uint32_t i) { return _buf[i]; }

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
    T *begin() const { return _buf; }
    T *end() const { return _buf + _size; }
    void push_back(const T &x) { _buf[_pos++] = x; }
};


typedef vector<int> Vint;
typedef vector<uint8_t> Vugt;

class BitInput
{
    istream *_is;
    int _nextBits, _bitPos = 8;
    uint8_t _getBitPos() { return _bitPos % 8; }
public:
    BitInput(istream *is) : _is(is) { }
    uint32_t readBit();
    bool readBool() { return readBit() == 1; }
    void ignore(int n) { while (n--) readBool(); }
    void ignoreBuf() { ignore(_getBitPos()); }
    void ignoreBytes(int n) { while (n--) readByte(); }
    int readByte() { return _is->get(); }
    void read(uint8_t *s, int n) { _is->read((char *)s, n); }
    int readInt(int n);
};

class Nau
{
    int _length;
    int *_a;
public:
    Nau(int n) : _length(n), _a(new int[n]) { }
    Nau(Vint a) : Nau(a.size()) { for (int i = 0; i < _length; i++) _a[i] = a[i]; }
    Nau(int n, int val) : Nau(n) { for (int i = 0; i < n; i++) _a[i] = val; }
    ~Nau() { delete[] _a; }
    void set(int i, int val) { _a[i] = val; }
    int get(int i) const { return _a[i]; }
    Nau copyOfRange(int a, int b) const;
    Nau copyOf(int n) const { return copyOfRange(0, n); }
    int length() const { return _length; }
    int max() const { return *max_element(_a, _a + _length); }
    void summary(ostream &os) const { os << "Length: " << _length << ", Max: " << max(); }
    string summary() const { ostringstream o; summary(o); return o.str(); }
    void dumpCompleet(ostream &os) const { summary(os); os << "\n"; dump(os); }
    void dump(ostream &os) const { for (int i = 0; i < _length; i++) os << _a[i] << " "; }
    string toString() const { ostringstream o; dumpCompleet(o); return o.str(); }
};

struct Node
{
    Node *left, *right;
    int symbol, type;
    Node() { }
    Node(Node *l, Node *r) : left(l), right(r), type(1) { }
    Node(int s) : symbol(s), type(2) { }
};

class CodeTree
{   
    Node _root;
    vector<Node> _nodes;
public:
    void import(Nau &x);
    CodeTree(Node root) : _root(root) { }
};

class Pair2
{
public:
    const Node a, b;
    Pair2() { }
    Pair2(const Pair2 &p2) : a(p2.a), b(p2.b) { }
    Pair2(Node a, Node b) : a(a), b(b) { }
};

class CircularDict
{
    FectorTemp<uint8_t> _data;
    int _index = 0;
    const int _mask;
public:
    CircularDict(int n) : _data(n), _mask(n > 0 && (n & (n - 1)) == 0 ? n - 1 : 0) { }
    void append(int b);
    void copy(int dist, int len, ostream &os);
};


class Inflate
{
    BitInput *_bi;
    CircularDict _dict;
    Node _lit, _dist;
    vector<Node> _nodeDump;
    void _decRaw(ostream &os);
    void _decHuff(Node lit, Node dist, ostream &os);
    int _decSym(Node *code);
    int _decRll(int sym);
    int _decDist(int sym);
    Pair2 _makePair();
    Node _toct(Nau &x);
public:
    Inflate(BitInput *bi);
    void extractTo(ostream &os);
};


#endif


