using namespace std;
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
    bool bzip2() const { return _bzip2; }
    bool gzip() const { return _gzip; }
    uint8_t compression() const;
    string archive() const { return _archive; }
};

uint8_t Options::compression() const
{
    if (bzip2()) return 3;
    if (gzip()) return 2;
    return 1;
}

class AppTar
{
    uint8_t _width = 5;
public:
    void createFile(ostream &os, string fn);
    int run(int argc, char **argv);
};

void AppTar::createFile(ostream &os, string fn)
{
    Header h;
    ifstream ifs(fn);
    SHeader sh = h.raw();
    os.write((char *)&sh, sizeof(sh));
    ifs.close();
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
    FILE *fp = fopen(o.archive().c_str(), "r");
    TarStream *ts;
    streambuf *bt;
    istream *is;
    
    switch (o.compression())
    {
    case 3:
        bt = new DecStreamBuf(fp);
        break;
    default:
        bt = new buftest(fp);
    }

    is = new istream(bt);
    ts = new TarStream(is);
    
    if (o.table())
        ts->list(o.verbose());

    if (o.extract())
        ts->extract(o.verbose());

    delete ts;
    delete is;
    delete bt;
    fclose(fp);
    return 0;
}

int main(int argc, char **argv)
{
    AppTar app;
    return app.run(argc, argv);
}



