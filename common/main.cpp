#include "hasher.h"

class Options
{
    bool _check;
    bool _cin;
    int _argc;
    char **_argv;
    int _argp;
public:
    Options(int argc, char **argv)
      : _check(false), _cin(false), _argc(argc), _argv(argv), _argp(0)
    { }

    bool check() const { return _check; }
    void parse(int argc, char **argv);
    void parse() { parse(_argc, _argv); }
    void dump(ostream &os) const;
    bool cin() const { return _cin; }
    bool isFile(char *arg) const { return arg[0] != '-'; }
    bool hasNextFileName() const;
    string nextFileName();
};

class Paar
{
    Hash _hash;
    string _fn;
public:
    Paar() { }
    Paar(Hash &hash, string &fn) : _hash(hash), _fn(fn) { }
    void read(istream &is) const;
    Hash hash() const { return _hash; }
    string fn() const { return _fn; }
    void dump(ostream &os) const;
};

class App
{
    Hasher _hasher;
    Options _options;
    void checkFile2(const char *fn);
    void checkPaar(Paar &paar);
public:
    App(int argc, char **argv) : _options(argc, argv) { }
    int run();
};

bool Options::hasNextFileName() const
{
    for (int i = _argp + 1; i < _argc; i++)
        if (isFile(_argv[i]))
            return true;

    return false;
}

void Paar::dump(ostream &os) const
{
    _hash.dump(os);
    os << "  " << _fn << "\n";
}

string Options::nextFileName()
{
    while (!isFile(_argv[++_argp]));
    return string(_argv[_argp]);
}

void Options::parse(const int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'c':
                _check = true;
                break;
            case '\0':
                _cin = true;
                break;
            }
        }
    }
}

void Options::dump(ostream &os) const
{
    if (_cin)
        os << "Standard Input\n";
}

void App::checkFile2(const char *fn)
{
    _hasher.reset();
    ifstream file;
    file.open(fn, fstream::in | ios::binary);

    if (!file.is_open())
        throw "Cannot open file";

    _hasher.stream(file);
    file.close();
    _hasher.hash().dump(cout);
    cout << "  " << fn << "\n";
}

void Paar::read(istream &is) const
{
    char line[255] = {0};
    is.getline(line, sizeof(line));
}

int App::run()
{
    _options.parse();

    if (_options.cin())
    {
        _hasher.stream(cin);
        _hasher.hash().dump(cout);
        cout << "  -\n";
    }

    while (_options.hasNextFileName() && !_options.check())
        checkFile2(_options.nextFileName().c_str());
    
    while (_options.hasNextFileName() && _options.check())
    {
        ifstream foo;
        char line[255] = {0};
        foo.open(_options.nextFileName().c_str(), fstream::in);
        
        while (foo)
        {
            foo.getline(line, sizeof(line));

            if (foo)
            {
                Hash hash(line);
                string fn = string(line + 34);
                Paar paar(hash, fn);
                checkPaar(paar);
            }
        }
        foo.close();
    }

    return 0;
}

void App::checkPaar(Paar &paar)
{
    ifstream file;
    file.open(paar.fn().c_str(), fstream::in | fstream::binary);
    _hasher.stream(file);
    file.close();
    Hash hash2 = _hasher.hash();

    if (paar.hash().isEqual(hash2))
        cout << paar.fn() << ": OK\n";
    else
        cout << paar.fn() << ": FAILED\n";
}

int main(const int argc, char **argv)
{
    App app(argc, argv);
    return app.run();
}


