#include <iostream>
#include <fstream>
#include <stdint.h>
#include <cstdlib>
using namespace std;

struct SHeader
{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
} __attribute__ ((packed));

class Header
{
    SHeader _h;
public:
    Header() { }
    Header(SHeader &h) : _h(h) { }
    Header(istream &is) { read(is); }
    void read(istream &is) { is.read((char *)&_h, sizeof(_h)); }
    uint32_t size() const { return strtol(_h.size, 0, 8); }
    string name() const { return string(_h.name); }
    bool empty() const { return _h.name[0] == 0; }
};

class App
{
public:
    bool extractFile(istream &is);
    void extractTar(istream &is) { while (extractFile(is)); }
    int run(int argc, char **argv);
};

bool App::extractFile(istream &is)
{
    Header h2(is);
    
    if (h2.empty())
        return false;

    is.ignore(512 - is.tellg() % 512);
    ofstream ofs(h2.name());

    for (uint32_t i = 0; i < h2.size(); i++)
        ofs.put(is.get());

    ofs.close();
    is.ignore(512 - is.tellg() % 512);
    return true;
}

int App::run(int argc, char **argv)
{
    extractTar(cin);
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


