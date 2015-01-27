#ifndef _GUNZIP_H_
#define _GUNZIP_H_
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;

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
    Nau(Vint a);
    Nau(int n) : _length(n), _a(new int[n]) { }
    Nau(int n, int val) : _length(n), _a(new int[n]) { for (int i = 0; i < n; i++) _a[i] = val; }
    void set(int i, int val) { _a[i] = val; }
    int get(int i) const { return _a[i]; }
    Nau copyOf(int n) const;
    Nau copyOfRange(int a, int b) const;
    int length() const { return _length; }
    int max() const { return *max_element(_a, _a + _length); }
    void dump(ostream &os) const { for (int i = 0; i < _length; i++) os << _a[i] << " "; }
    string toString() const { ostringstream o; dump(o); return o.str(); }
};

struct Node
{
    Node *left, *right;
    int symbol, type;
    Node() { }
    Node(Node *l, Node *r) : left(l), right(r), type(1) { }
    Node(int s) : symbol(s), type(2) { }
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
    Vugt _data;
    int _index = 0;
    const int _mask;
public:
    CircularDict(int n) : _data(n), _mask(n > 0 && (n & (n - 1)) == 0 ? n - 1 : 0) { }
    void append(int b);
    void copy(int dist, int len, ostream &os);
};

class Decompressor
{
    BitInput *_bi;
    CircularDict _dict;
    Node _lit, _dist;
    void _decRaw(ostream &os);
    void _decHuff(Node lit, Node dist, ostream &os);
    int _decSym(Node *code);
    int _decRll(int sym);
    int _decDist(int sym);
    Pair2 _makePair();
    Node _toct3(Nau &x);
public:
    Decompressor(BitInput *bi);
    void extractTo(ostream &os);
};

class GzipStream
{
    BitInput *_bi;
    string _readString();
public:
    GzipStream(BitInput *bi) : _bi(bi) { cerr << "GzipStream constructor\n"; }
    void extractTo(ostream &os);
};

class App
{
public:
    int run(int argc, char **argv);
};


#endif


