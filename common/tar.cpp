#include <iostream>
#include <fstream>
#include <stdint.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;

typedef vector<string> Vest;

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

class Options
{
    string _archive;
    bool _verbose = false;
    bool _extract = false;
    bool _table = false;
    bool _create = false;
    bool _bzip2 = false;
    Vest _filenames;
public:
    Options() { }
    void parse(int argc, char **argv);
    Options(int argc, char **argv) { parse(argc, argv); }
    bool verbose() const { return _verbose; }
    bool extract() const { return _extract; }
    bool table() const { return _table; }
    bool create() const { return _create; }
    string archive() const { return _archive; }
};

class Header
{
    SHeader _h;
    string _mode(char c) const;
public:
    Header() { }
    Header(SHeader &h) : _h(h) { }
    Header(istream &is) { read(is); }
    void read(istream &is) { is.read((char *)&_h, sizeof(_h)); }
    uint32_t size() const { return strtol(_h.size, 0, 8); }
    string name() const { return string(_h.name); }
    string uname() const { return string(_h.uname); }
    string gname() const { return string(_h.gname); }
    bool empty() const { return _h.name[0] == 0; }
    SHeader raw() const { return _h; }
    void fullInfo(ostream &os) const;
    void mode(ostream &os) const;
    void timeStamp(ostream &os) const;
    string mode() const { ostringstream s; mode(s); return s.str(); }
    string fullInfo() const { ostringstream s; fullInfo(s); return s.str(); }
};

class App
{
public:
    bool extractFile(istream &is, bool verbose = false);
    bool listFile(istream &is, bool verbose = false);
    void createFile(ostream &os, string fn);
    void extractTar(istream &is, bool verbose = false) { while (extractFile(is, verbose)); }
    void extractTar(string fn, bool v = false) { ifstream i(fn); extractTar(i, v); i.close(); }
    void listTar(istream &is, bool v = false) { while (listFile(is, v)); }
    void listTar(string fn, bool v = false) { ifstream i(fn); listTar(i, v); i.close(); }
    int run(int argc, char **argv);
};

void Header::fullInfo(ostream &os) const
{
    os << mode() << " " << uname() << "/" << gname() << " " << setw(5) << size() << " "
       << "1990-01-01 00-00 " << name() << "\n";
}

void Header::timeStamp(ostream &os) const
{
}

string Header::_mode(char c) const
{
    switch (c)
    {
        case '7': return "rwx";
        case '6': return "rw-";
        case '5': return "r-x";
        case '4': return "r--";
    }

    return "---";
}

void Header::mode(ostream &os) const
{
    os << "-" << _mode(_h.mode[4]) << _mode(_h.mode[5]) << _mode(_h.mode[6]);
}

bool App::listFile(istream &is, bool verbose)
{
    Header h(is);
    if (h.empty()) return false;
    if (verbose) h.fullInfo(cout); else cout << h.name() << "\n";
    is.ignore(512 - is.tellg() % 512);
    is.ignore(h.size());
    is.ignore(512 - is.tellg() % 512);
    return true;
}

void App::createFile(ostream &os, string fn)
{
    Header h;
    ifstream ifs(fn);
    SHeader sh = h.raw();
    os.write((char *)&sh, sizeof(sh));
    ifs.close();
}

bool App::extractFile(istream &is, bool verbose)
{
    Header h(is);
    if (h.empty()) return false;
    if (verbose) cout << h.name() << "\n";
    is.ignore(512 - is.tellg() % 512);
    ofstream ofs(h.name());
    for (uint32_t i = 0; i < h.size(); i++) ofs.put(is.get());
    ofs.close();
    is.ignore(512 - is.tellg() % 512);
    return true;
}

void Options::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            for (uint32_t j = 1; j < strlen(argv[i]); j++)
            {
                switch (argv[i][j])
                {
                case 'j':
                    _bzip2 = true;
                    break;
                case 't':
                    _table = true;
                    break;
                case 'c':
                    _create = true;
                    break;
                case 'f':
                    _archive = string(argv[++i]);
                    j = strlen(argv[i]);
                    break;
                case 'x':
                    _extract = true;
                    break;
                case 'v':
                    _verbose = true;
                    break;
                }
            }
        }
        else
        {
            _filenames.push_back(argv[i]);
        }
    }
}

int App::run(int argc, char **argv)
{
    Options o(argc, argv);
    
    if (o.table())
        listTar(o.archive(), o.verbose());

    if (o.extract())
        extractTar(o.archive(), o.verbose());

    if (o.create());
        
        

    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}

