#include <iostream>
#include <fstream>
#include <stdint.h>
using namespace std;

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
};

class Decompressor
{
    BitInput *_is;
public:
};

class GzipStream
{
    BitInput *_is;
    string _readString();
public:
    GzipStream(BitInput *is) : _is(is) { }
    GzipStream(BitInput &is) : _is(&is) { }
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
    _is->read(x, 10);
    uint8_t flags = x[3];
    
    if (flags & 4)
    {
        uint8_t b[2];
        _is->read(b, 2);
        int len = b[0] | b[1] << 8;
        _is->ignoreBytes(len);
    }

    if (flags & 8)
        cerr << _readString();

    if (flags & 2)
        _is->ignoreBytes(2);

    if (flags & 0x10)
        cerr << _readString();

    
}

string GzipStream::_readString()
{
    string s;

    for (int c = _is->readByte(); c != 0; c = _is->readByte())
        s.push_back(c);

    return s;
}

uint32_t BitInput::readBit()
{
    if (_bitPos == 8)
        _nextBits = _is->get(), _bitPos = 0;

    return _nextBits >> _bitPos++ & 1;
}


int App::run(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    ofstream ofs(argv[2]);
    ofs.close();
    ifs.close();
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


