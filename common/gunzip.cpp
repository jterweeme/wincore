#include <iostream>
#include <fstream>
#include <stdint.h>
#include <algorithm>
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
    Nau(int n) : _length(n), _a(new int[n]) { }
    Nau(int n, int val) : _length(n), _a(new int[n]) { for (int i = 0; i < n; i++) _a[i] = val; }
    void set(int i, int val) { _a[i] = val; }
    int get(int i) const { return _a[i]; }
    Nau copyOf(int n) const;
    Nau copyOfRange(int a, int b) const;
    int length() const { return _length; }
    int oneCount();
    int otherPositive();
    int max() const { return *max_element(_a, _a + _length); }
};

Nau Nau::copyOf(int n) const
{
    Nau r(n);
    for (int i = 0; i < n; i++) r.set(i, _a[i]);
    return r;
}

Nau Nau::copyOfRange(int start, int end) const
{
    Nau r(end - start);
    for (int i = 0; i < end - start; i++) r.set(i, _a[i + start]);
    return r;
}

struct Node
{
    Node *left, *right;
    int symbol, type;
    Node(int s) : symbol(s), type(2) { }
    Node(Node *l, Node *r) : left(l), right(r), type(2) { }
};

class CodeTree
{
    vector<Vint> _codes;
    void _buildCodeList(Node *node, Vint *prefix);
public:
    Node *root;
    CodeTree(Node *root, int symbolLimit);
    CodeTree() { }
};

struct Pair
{
    CodeTree a, b;
    Pair(CodeTree a, CodeTree b) : a(a), b(b) { }
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
    CodeTree *_lit, *_dist;
    void _decRaw(ostream &os);
    void _decHuff(Node *lit, Node *dist, ostream &os);
    void _decHuff(CodeTree *l, CodeTree *d, ostream &os) { _decHuff(l->root, d->root, os); }
    int _decSym(Node *code);
    int _decSym(CodeTree *c) { return _decSym(c->root); }
    int _decRll(int sym);
    int _decDist(int sym);
    Pair _makePair();
    int _max(Vint a) const { return *max_element(a.begin(), a.end()); }
    CodeTree *_toct(Vint c);
    CodeTree _toct(Nau &x);
public:
    Decompressor(BitInput *bi);
    void extractTo(ostream &os);
};

class GzipStream
{
    BitInput *_bi;
    string _readString();
public:
    GzipStream(BitInput *bi) : _bi(bi) { }
    void extractTo(ostream &os);
};

class App
{
public:
    int run(int argc, char **argv);
};

void GzipStream::extractTo(ostream &os)
{
    uint8_t x[10];
    _bi->read(x, 10);
    uint8_t flags = x[3];
    
    if (flags & 4)
    {
        uint8_t b[2];
        _bi->read(b, 2);
        int len = b[0] | b[1] << 8;
        _bi->ignoreBytes(len);
    }

    if (flags & 8)
        cerr << _readString();

    if (flags & 2)
        _bi->ignoreBytes(2);

    if (flags & 0x10)
        cerr << _readString();

    Decompressor d(_bi);
    d.extractTo(os);
}

string GzipStream::_readString()
{
    string s;
    for (int c = _bi->readByte(); c != 0; c = _bi->readByte()) s.push_back(c);
    return s;
}

CodeTree *Decompressor::_toct(Vint cl)
{
    vector<Node> nodes;

    for (int i = _max(cl); i >= 1; i--)
    {
        vector<Node> newNodes;

        for (uint32_t j = 0; j < cl.size(); j++)
            if (cl[j] == i)
                newNodes.push_back(Node(j));

        for (uint32_t j = 0; j < cl.size(); j += 2)
            newNodes.push_back(Node(&nodes[j], &nodes[j + 1]));

        nodes = newNodes;
    }

    return new CodeTree(new Node(&nodes[0], &nodes[1]), cl.size());
}

CodeTree Decompressor::_toct(Nau &x)
{
    vector<Node> nodes;
    
    for (int i = x.max(); i >= 1; i--)
    {
        vector<Node> newNodes;
        
        for (int j = 0; j < x.length(); j++)
            if (x.get(j) == i)
                newNodes.push_back(Node(j));

        for (uint32_t j = 0; j < nodes.size(); j+= 2)
            newNodes.push_back(Node(&nodes[j], &nodes[j + 1]));

        nodes = newNodes;
    }

    return CodeTree(new Node(&nodes[0], &nodes[1]), x.length());
}

void Decompressor::extractTo(ostream &os)
{
    for (bool isFinal = false; !isFinal;)
    {
        isFinal = _bi->readBool();
        int type = _bi->readInt(2);

        switch (type)
        {
        case 0:
            _decRaw(os);
            break;
        case 1:
            _decHuff(_lit, _dist, os);
            break;
        case 2:
        {
            Pair temp = _makePair();
            _decHuff(temp.a.root, temp.b.root, os);
        }
            break;
        default:
            throw "Assertion Error";
        }
    }
}

Decompressor::Decompressor(BitInput *bi) : _bi(bi), _dict(32 * 1024)
{
    Vint llcodelens;
    fill_n(llcodelens.begin(), 144, 8);
    fill_n(llcodelens.begin() + 144, 112, 9);
    fill_n(llcodelens.begin() + 256, 24, 7);
    fill_n(llcodelens.begin() + 280, 8, 8);
    _lit = _toct(llcodelens);
    Vint distcodelens(32, 5);
    _dist = _toct(distcodelens);
}

Pair Decompressor::_makePair()
{
    int nlit = _bi->readInt(5) + 257, ndist = _bi->readInt(5) + 1, ncode = _bi->readInt(4) + 4;
    Nau a(19);
    a.set(16, _bi->readInt(3));
    a.set(17, _bi->readInt(3));
    a.set(18, _bi->readInt(3));
    a.set(0, _bi->readInt(3));

    for (int i = 0; i < ncode - 4; i++)
    {
        int j = i % 2 == 0 ? 8 + i / 2 : 7 - i / 2;
        a.set(j, _bi->readInt(3));
    }

    CodeTree b = _toct(a);
    Nau c(nlit + ndist);

    for (int i = 0, runVal = -1, runLen = 0, sym; i < c.length(); i++)
    {
        if (runLen > 0)
        {
            c.set(i, runVal);
            runLen--;
        }
        else
        {
            switch (sym = _decSym(b.root))
            {
            case 16:
                runLen = _bi->readInt(2) + 3;
                i--;
                break;
            case 17:
                runVal = 0;
                runLen = _bi->readInt(3) + 3;
                i--;
                break;
            case 18:
                runVal = 0;
                runLen = _bi->readInt(7) + 11;
                i--;
                break;
            default:
                c.set(i, sym);
                runVal = sym;
            }
        }
    }

    Nau litLenCodeLen = c.copyOf(nlit);
    CodeTree litLenCode = _toct(litLenCodeLen);
    Nau distCodeLen = c.copyOfRange(nlit, c.length());
    CodeTree distCode = _toct(distCodeLen);
    return Pair(litLenCode, distCode);
}

void Decompressor::_decRaw(ostream &os)
{
    _bi->ignoreBuf();
    int len = _bi->readInt(16);
    _bi->ignore(16);
    
    for (int i = 0; i < len; i++)
    {
        int temp = _bi->readByte();
        os.put(temp);
        _dict.append(temp);
    }
}

void Decompressor::_decHuff(Node *lit, Node *dist, ostream &os)
{
    while (true)
    {
        int sym = _decSym(lit);
        
        if (sym == 256)
            break;
        
        if (sym < 256)
        {
            os.put(sym);
            _dict.append(sym);
        }
    }
}

int Decompressor::_decSym(Node *n)
{
    Node *next = _bi->readBool() ? n->right : n->left;

    for (n = next; next->type == 1; n = next)
        next = _bi->readBool() ? n->right : n->left;

    return next->symbol;
}

int Decompressor::_decRll(int sym)
{
    int i = (sym - 261) / 4;
    if (sym <= 264) return sym - 254;
    if (sym <= 284) return ((sym - 265) % 4 + 4 << i) + 3 + _bi->readInt(i);
    return 258;
}

int Decompressor::_decDist(int sym)
{
    int i = sym / 2 - 1;
    return sym <= 3 ? sym + 1 : (sym % 2 + 2 << i) + 1 + _bi->readInt(i);
}

void CircularDict::append(int b)
{
    _data[_index] = (uint8_t)b;
    _index = _mask != 0 ? (_index + 1) & _mask : (_index + 1) % _data.size();
}

void CircularDict::copy(int dist, int len, ostream &os)
{
    for (int readIndex = (_index - dist + _data.size()) & _mask; len > 0 && _mask != 0; len--)
    {
        os.put(_data[readIndex]);
        _data[_index] = _data[readIndex];
        readIndex = (readIndex + 1) & _mask;
        _index = (_index + 1) & _mask;
    }

    for (int j = (_index - dist + _data.size()) % _data.size(); len > 0 && _mask == 0; len--)
    {
        os.put(_data[j]);
        _data[_index] = _data[j];
        j = (j + 1) % _data.size();
        _index = (_index + 1) % _data.size();
    }
}

CodeTree::CodeTree(Node *root, int symbolLimit) : root(root)
{
    for (int i = 0; i < symbolLimit; i++) _codes.push_back(Vint());
    _buildCodeList(root, new Vint());
}

void CodeTree::_buildCodeList(Node *node, Vint *prefix)
{
    if (node->type == 1)
    {
        prefix->push_back(0);
        _buildCodeList(node->left, prefix);
        prefix->pop_back();
        prefix->push_back(1);
        _buildCodeList(node->right, prefix);
        prefix->pop_back();
        return;
    }

    _codes[node->symbol] = *prefix;
}

uint32_t BitInput::readBit()
{
    if (_bitPos == 8) _nextBits = _is->get(), _bitPos = 0;
    return _nextBits >> _bitPos++ & 1;
}

int BitInput::readInt(int n)
{
    int r = 0;
    for (int i = 0; i < n; i++) r |= readBit() << i;
    return r;
}

int App::run(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    ofstream ofs(argv[2]);
    BitInput bi(&ifs);
    GzipStream gz(&bi);
    gz.extractTo(ofs);
    ofs.close();
    ifs.close();
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


