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

struct Node
{
    Node *left, *right;
    int symbol, type;
    Node(int s) : symbol(s), type(2) { }
};

class CodeTree
{
    vector<Vint> _codes;
    void _buildCodeList(Node *node, Vint *prefix);
public:
    Node *root;
    CodeTree(Node *root, int symbolLimit);
};

class CircularDict
{
    Vugt _data;
    int _index = 0, _mask;
public:
    CircularDict(int size);
    void append(int b);
    void copy(int dist, int len, ostream &os);
};

class CanonicalCode
{
    Vint _codeLengths;
    void _buildCodeLengths(Node node, int depth);
    int _max(Vint a);
public:
    CanonicalCode(Vint c) : _codeLengths(c) { }
    CanonicalCode(CodeTree tree, int symbolLimit) { }
    CodeTree toCodeTree();
};

class Decompressor
{
    BitInput *_bi;
    CircularDict _dict;
    CodeTree *_lit, *_dist;
    Vint _llcodelens;
    void _decRaw(ostream &os);
    void _decHuff(CodeTree *lit, CodeTree *dist, ostream &os);
    int _decSym(CodeTree *code);
    int _decRll(int sym);
    int _decDist(int sym);
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

    for (int c = _bi->readByte(); c != 0; c = _bi->readByte())
        s.push_back(c);

    return s;
}

void Decompressor::extractTo(ostream &os)
{
    for (bool isFinal = false; !isFinal;)
    {
        isFinal = _bi->readBool();
        int type = _bi->readInt(2);
        //CodeTree *lit, *dist;

        switch (type)
        {
        case 0:
            _decRaw(os);
            break;
        case 1:
            _decHuff(_lit, _dist, os);
        }
    }
}

Decompressor::Decompressor(BitInput *bi) : _bi(bi), _dict(32 * 1024)
{
    fill_n(_llcodelens.begin(), 144, 8);
    fill_n(_llcodelens.begin() + 144, 112, 9);
    fill_n(_llcodelens.begin() + 256, 24, 7);
    fill_n(_llcodelens.begin() + 280, 8, 8);

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

void Decompressor::_decHuff(CodeTree *lit, CodeTree *dist, ostream &os)
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

int Decompressor::_decSym(CodeTree *code)
{
    for (Node *cur = code->root;;)
    {
        Node *nextNode = _bi->readBool() ? cur->right : cur->left;

        if (nextNode->type == 2)
            return nextNode->symbol;
        else if (nextNode->type == 1)
            cur = nextNode;
    }
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

void CanonicalCode::_buildCodeLengths(Node node, int depth)
{
    if (node.type == 1)
    {
    }
}

uint32_t BitInput::readBit()
{
    if (_bitPos == 8)
        _nextBits = _is->get(), _bitPos = 0;

    return _nextBits >> _bitPos++ & 1;
}

int BitInput::readInt(int n)
{
    int result = 0;

    for (int i = 0; i < n; i++)
        result |= readBit() << i;

    return result;
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

void CircularDict::append(int b)
{
    _data[_index] = (uint8_t)b;
    _index = _mask != 0 ? (_index + 1) & _mask : (_index + 1) % _data.size();
}

CircularDict::CircularDict(int n) : _data(8)
{
    _mask = n > 0 && (n & (n - 1)) == 0 ? n - 1 : 0;
}

void CircularDict::copy(int dist, int len, ostream &os)
{
    if (_mask != 0)
    {
        for (int i = 0, readIndex = (_index - dist + _data.size()) & _mask; i < len; i++)
        {
            os.put(_data[readIndex]);
            _data[_index] = _data[readIndex];
            readIndex = (readIndex + 1) & _mask;
            _index = (_index + 1) & _mask;
        }
    }
    else
    {
        for (int i = 0, j = (_index - dist + _data.size()) % _data.size(); i < len; i++)
        {
            os.put(_data[j]);
            _data[_index] = _data[j];
            j = (j + 1) % _data.size();
            _index = (_index + 1) % _data.size();
        }
    }
}

CodeTree::CodeTree(Node *root, int symbolLimit) : _root(root)
{
    for (int i = 0; i < symbolLimit; i++) _codes.push_back(0);
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


