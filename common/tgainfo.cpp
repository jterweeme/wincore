#include "common.h"

struct STgaHeader
{
    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t imgType;
    uint8_t onbekend1;
    uint32_t onbekend2;
    uint16_t offset_x;
    uint16_t offset_y;
    uint16_t width;
    uint16_t height;
} PACKED;

class CTgaHeader
{
    STgaHeader _th;
public:
    void read(istream &is) { is.read((char *)&_th, sizeof(_th)); }
    void dump(ostream &os);
};

class Options
{
    string _input;
public:
    void parse(int argc, char **argv);
    string input() const { return _input; }
};

class App
{
public:
    int run(int argc, char **argv);
};

void CTgaHeader::dump(ostream &os)
{
    os << "ID Length: " << (int)_th.idLength << "\n";
    os << "Colormap: " << (int)_th.colorMapType << "\n";
    os << "Image type: " << (int)_th.imgType << "\n";
    os << _th.width << "x" << _th.height << "\n";
}

void Options::parse(int argc, char **argv)
{
    _input = argv[1];
}

int App::run(int argc, char **argv)
{
    Options o;
    o.parse(argc, argv);
    
    ifstream ifs(o.input().c_str());
    CTgaHeader th;
    th.read(ifs);
    th.dump(cout);
    

    return 0;
}

int main(int argc, char **argv)
{
    App app;
    int ret = -1;
    ret = app.run(argc, argv);
    return ret;
}

