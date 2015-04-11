#ifndef _INFLATE_H_
#define _INFLATE_H_
#include "fector.h"
#include "bitinput.h"

typedef vector<int> Vint;
typedef vector<uint8_t> Vugt;

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
    Fector<uint8_t> _data;
    int _index = 0;
    const int _mask;
public:
    CircularDict(int n) : _data(n), _mask(n > 0 && (n & (n - 1)) == 0 ? n - 1 : 0) { }
    void append(int b);
    void copy(int dist, int len, ostream &os);
};


class Inflate
{
    BitInput2 *_bi;
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
    Inflate(BitInput2 *bi);
    void extractTo(ostream &os);
};


#endif


