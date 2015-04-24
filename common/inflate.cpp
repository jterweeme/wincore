#include "inflate.h"

bool Inflate::read(ostream &os)
{
    bool isFinal = _bi->readBool();
    int type = _bi->readBits(2);
    
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
        Pair2 temp = _makePair();
        _decHuff(temp.a, temp.b, os);
    }   
        break;
    default:
        throw "Assertion Error";
    }

    _nodeDump.clear();
    return isFinal;
}

int Inflate::_decDist(int sym)
{
    int i = sym / 2 - 1;
    return sym <= 3 ? sym + 1 : (sym % 2 + 2 << i) + 1 + _bi->readBits(i);
}

Pair2 Inflate::_makePair()
{
    int nlit = _bi->readBits(5) + 257, ndist = _bi->readBits(5) + 1, ncode = _bi->readBits(4) + 4;
    Nau a(19, 0);
    a.set(16, _bi->readBits(3));
    a.set(17, _bi->readBits(3));
    a.set(18, _bi->readBits(3));
    a.set(0, _bi->readBits(3));

    for (int i = 0; i < ncode - 4; i++)
    {
        int j = i % 2 == 0 ? 8 + i / 2 : 7 - i / 2;
        int x = _bi->readBits(3);
        a.set(j, x);
    }

    Node b = _toct(a);
    Nau c(nlit + ndist, 0);
    for (int i = 0, runVal = -1, runLen = 0, sym; i < c.length(); i++)
    {
        if (runLen > 0)
        {
            c.set(i, runVal);
            runLen--;
        }
        else
        {
            switch (sym = _decSym(&b))
            {
            case 16:
                runLen = _bi->readBits(2) + 3;
                i--;
                break;
            case 17:
                runVal = 0;
                runLen = _bi->readBits(3) + 3;
                i--;
                break;
            case 18:
                runVal = 0;
                runLen = _bi->readBits(7) + 11;
                i--;
                break;
            default:
                c.set(i, sym);
                runVal = sym;
            }
        }
    }

    Nau litLenCodeLen = c.copyOf(nlit);
    Node litLenCode = _toct(litLenCodeLen);
    Nau distCodeLen = c.copyOfRange(nlit, c.length());
    Node distCode = _toct(distCodeLen);
    return Pair2(litLenCode, distCode);
}

Inflate::Inflate(BitInput2 *bi) : _bi(bi), _dict(32 * 1024)
{
    Vint llcodelens(288);
    fill_n(llcodelens.begin(), 144, 8);
    fill_n(llcodelens.begin() + 144, 112, 9);
    fill_n(llcodelens.begin() + 256, 24, 7);
    fill_n(llcodelens.begin() + 280, 8, 8);
    Nau llcodeLens2(llcodelens);
    _lit = _toct(llcodeLens2);
    Nau distcodelens(32, 5);
    _dist = _toct(distcodelens);
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

Nau Nau::copyOfRange(int start, int end) const
{
    Nau r(end - start);
    for (int i = 0; i < end - start; i++) r.set(i, _a[i + start]);
    return r;
}

void Inflate::_decRaw(ostream &os)
{
    int len = _bi->readBits(16);
    _bi->ignore(16);

    for (int i = 0; i < len; i++)
    {
        int temp = _bi->readByte();
        os.put(temp);
        _dict.append(temp);
    }
}

void Inflate::_decHuff(Node lit, Node dist, ostream &os)
{
    for (int sym; (sym = _decSym(&lit)) != 256;)
    {
        if (sym < 256)
        {
            os.put(sym);
            _dict.append(sym);
        }
        else
        {
            int len = _decRll(sym), distSym = _decSym(&dist);
            _dict.copy(_decDist(distSym), len, os);
        }
    }
}

int Inflate::_decSym(Node *n)
{
    Node *next = _bi->readBool() ? n->right : n->left;
    for (n = next; next->type == 1; n = next) next = _bi->readBool() ? n->right : n->left;
    return next->symbol;
}

int Inflate::_decRll(int sym)
{
    int i = (sym - 261) / 4;
    if (sym <= 264) return sym - 254;
    if (sym <= 284) return ((sym - 265) % 4 + 4 << i) + 3 + _bi->readBits(i);
    return 258;
}

Node Inflate::_toct(Nau &x)
{
    vector<Node *> nodes;

    for (int i = x.max(); i >= 1; i--)
    {
        vector<Node *> newNodes;

        for (int j = 0; j < x.length(); j++)
        {
            if (x.get(j) == i)
            {
                _nodeDump.push_back(Node(j));
                newNodes.push_back(&_nodeDump.back());
            }
        }

        for (uint32_t j = 0; j < nodes.size(); j+= 2)
        {
            _nodeDump.push_back(Node(nodes.at(j), nodes.at(j + 1)));
            newNodes.push_back(&_nodeDump.back());
        }

        nodes.clear();

        for (vector<Node *>::const_iterator it = newNodes.cbegin(); it != newNodes.cend(); it++)
            nodes.push_back(*it);
    }

    return Node(nodes.at(0), nodes.at(1));
}


