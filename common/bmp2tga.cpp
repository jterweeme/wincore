#include "common.h"
#include "bitinput.h"

struct BitmapHeader
{
    uint16_t magic;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t nColorPanes;
    uint16_t depth;
    uint32_t compression;
    uint32_t imgSize;
} PACKED;

class CBitmapHeader
{
    BitmapHeader _bh;
public:
    void read(BitInputBase &is) { is.read((uint8_t *)&_bh, sizeof(_bh)); }
    void dump(ostream &os) const;
};

class Options
{
    string _input;
    string _output;
public:
    void parse(int argc, char **argv);
    string input() const { return _input; }
    string output() const { return _output; }
};

class App
{
public:
    int run(int argc, char **argv);
};

void Options::parse(int argc, char **argv)
{
    _input = argv[1];
    _output = argv[2];
}

void CBitmapHeader::dump(ostream &os) const
{
    os << "Magic: " << _bh.magic << "\n";
    os << "FileSize: " << _bh.size << "\n";
    os << "Dimensions: " << _bh.width << "x" << _bh.height << "x" << _bh.depth << "\n";
    os << "Image offset: " << _bh.offset << "\n";
    os << "Image Size: " << _bh.imgSize << "\n";
}

int App::run(int argc, char **argv)
{
    Options o;
    o.parse(argc, argv);
    ifstream ifs(o.input().c_str());
    BitInputStream bis(&ifs);
    ofstream ofs(o.output().c_str());

    CBitmapHeader bh;
    bh.read(bis);
    bh.dump(cout);

    for (int i = 0; i < 32 * 32; i++);
        


    return 0;
}

int main(int argc, char **argv)
{
    App app;
    int ret = -1;
    ret = app.run(argc, argv);

    return ret;
}


