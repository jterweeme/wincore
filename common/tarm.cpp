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
    Options _options;
    int _createArchive();
public:
    int run(int argc, char **argv);
};

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

int AppTar::_createArchive()
{
    TarWriter t;
    ofstream ofs(_options.archive().c_str());
    ifstream ifs("yes.cpp");
    //ofs << "Onzin\n";
    SHeader sh = {};
    strcpy(sh.name, "yes.cpp");
    strcpy(sh.mode, "0000644");
    strcpy(sh.uid, "0001750");
    strcpy(sh.gid, "0001750");
    strcpy(sh.size, "00000000450");
    strcpy(sh.mtime, "12345678123");
    strcpy(sh.chksum, "000000");
    sh.typeflag = 1;
    t.writeFile(ofs, ifs, sh);
    ifs.close();
    ofs.close();
    return 0;
}

int AppTar::run(int argc, char **argv)
{
    _options.parse(argc, argv);
    FILE *fp;

    if (_options.create())
        return _createArchive();

    fp = fopen(_options.archive().c_str(), "r");
    BitInputFile bif;
    BitInput2File bif2;
    streambuf *bt;
    istream *is;
    TarStream *ts;
    
    switch (_options.compression())
    {
    case 3:
        bif.open(fp);
        bt = new DecStreamBuf(&bif);
        break;
    case 2:
        bif2.open(fp);
        bt = new GzipStreamBuf(&bif2);
        break;
    default:
        bt = new buftest(fp);
    }

    is = new istream(bt);
    ts = new TarStream(is);
    
    if (_options.table())
        ts->list(_options.verbose());

    if (_options.extract())
        ts->extract(_options.verbose());

    delete ts;
    delete is;
    delete bt;
    fclose(fp);
    return 0;
}

int main(int argc, char **argv)
{
    AppTar app;
    int ret = -1;

    try
    {
        ret = app.run(argc, argv);
    }
    catch (const char *e)
    {
        cerr << e << "\n";
    }

    return ret;
}



