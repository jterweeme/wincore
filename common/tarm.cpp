#include "tar.h"

class Options
{
    string _archive;
    bool _verbose = false;
    bool _extract = false;
    bool _table = false;
    bool _create = false;
    bool _bzip2 = false;
    bool _gzip = false;
    Vest _filenames;
public:
    Options() { }
    void parse(int argc, char **argv);
    Options(int argc, char **argv) { parse(argc, argv); }
    bool verbose() const { return _verbose; }
    bool extract() const { return _extract; }
    bool table() const { return _table; }
    bool create() const { return _create; }
    bool gzip() const { return _gzip; }
    string archive() const { return _archive; }
};

class AppTar
{
public:
    bool extractFile(istream &is, bool verbose = false);
    bool listFile(istream &is, bool verbose = false);
    void createFile(ostream &os, string fn);
    void extractTar(istream &is, bool verbose = false) { while (extractFile(is, verbose)); }
    void extractTar(string fn, bool v = false) { ifstream i(fn); extractTar(i, v); i.close(); }
    void listTar(istream &is, bool v = false) { while (listFile(is, v)); }
    void listTar(string fn, bool v = false);
    int run(int argc, char **argv);
};

bool AppTar::listFile(istream &is, bool verbose)
{
    Header h(is);
    if (h.empty()) return false;
    if (verbose) h.fullInfo(cout); else cout << h.name() << "\n";
    is.ignore(512 - is.tellg() % 512);
    is.ignore(h.size());
    is.ignore(512 - is.tellg() % 512);
    return true;
}

void AppTar::createFile(ostream &os, string fn)
{
    Header h;
    ifstream ifs(fn);
    SHeader sh = h.raw();
    os.write((char *)&sh, sizeof(sh));
    ifs.close();
}

bool AppTar::extractFile(istream &is, bool verbose)
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

void AppTar::listTar(string fn, bool v)
{
    ifstream i(fn);
    listTar(i, v);
    i.close();
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
                case 'z':
                    _gzip = true;
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

int AppTar::run(int argc, char **argv)
{
    Options o(argc, argv);
    istream *_is;
    
    
    if (o.table())
        listTar(o.archive(), o.verbose());

    if (o.extract())
        extractTar(o.archive(), o.verbose());

    if (o.create());
        
        

    return 0;
}

int main(int argc, char **argv)
{
    AppTar app;
    return app.run(argc, argv);
}



