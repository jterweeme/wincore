#include "common.h"

class Options
{
    const char *_if;
    const char *_of;
public:
    int parse(int argc, char **argv);
    void dump(ostream &os);
    const char * const ifs() { return _if; }
    const char * const ofs() { return _of; }
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class App
{
    Options _options;
    char *_buf;
public:
    int run(int argc, char **argv);
};

void Options::dump(ostream &os)
{
    os << "Input file: " << _if << "\n";
    os << "Output file: " << _of;
}

int Options::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], "if", 2) == 0)
            _if = argv[i] + 3;

        if (strncmp(argv[i], "of", 2) == 0)
            _of = argv[i] + 3;

        
    }
    return 0;
}

int App::run(int argc, char **argv)
{
    _options.parse(argc, argv);
    _options.dump(cout);
    cout << "\n";
    _buf = new char[512];
    ifstream ifs(_options.ifs());
    ofstream ofs(_options.ofs());

    while (ifs)
    {
        ifs.read((char *)_buf, 512);
        ofs.write(_buf, ifs.gcount());
    }

    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


