#include "hasher.h"

typedef vector<string> Vest;

class Options
{
    bool _check = false;
    bool _cin = false;
    bool _compare = false;
    string _compareSum;
    bool _help;
    Vest _files;
public:
    Vest files() const { return _files; }
    bool compare() const { return _compare; }
    string compareSum() const { return _compareSum; }
    bool check() const { return _check; }
    void parse(int argc, char **argv);
    bool cin() const { return _cin; }
};

/*
Class Paar stores a known MD5 sum in _hash and
a filename in _fn. Given the filename, a file can
be opened and hashed. The hash can be compared to the
known good hash in _hash.
*/
class Paar
{
    const Hash _hash;
    const string _fn;
public:
    Paar(Hash &hash, string &fn) : _hash(hash), _fn(fn) { }
    Hash hash() const { return _hash; }
    string fn() const { return _fn; }
};

class AppMD5Sum
{
    Hasher _hasher;
    void _hashStream(istream &is, const char *name, ostream &os);
    void _hashCin(ostream &os) { _hashStream(cin, "-", os); }
    void _hashFile(const char *fn, ostream &os);
    void _hashFile(string fn, ostream &os) { _hashFile(fn.c_str(), os); }
    void _checkFile(string fn, ostream &os);
    void _checkPaar(Paar &paar, ostream &os);
public:
    int run(int argc, char **argv);
};

void Options::parse(const int argc, char **argv)
{
    if (argc <= 1)
        _cin = true;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'c':
                _check = true;
                break;
            case 'x':
                _compare = true;
                _compareSum = string(argv[++i]);
                break;
            case '\0':
                _cin = true;
                break;
            }
        }
        else
        {
            _files.push_back(argv[i]);
        }
    }
}

void AppMD5Sum::_hashStream(istream &is, const char *name, ostream &os)
{
    _hasher.stream(is);
    os << _hasher.hash().toString() << "  " << name << "\n";
}

void AppMD5Sum::_hashFile(const char *fn, ostream &os)
{
    //cerr << fn << "\n";
    _hasher.reset();
    ifstream file;
    file.open(fn, fstream::in | ios::binary);

    if (!file.is_open())
        throw "Cannot open file";

    _hashStream(file, fn, os);
    file.close();
}

void AppMD5Sum::_checkFile(string fn, ostream &os)
{
    ifstream foo;
    char line[255] = {0};
    foo.open(fn.c_str(), fstream::in);
    
    while (foo)
    {
        foo.getline(line, sizeof(line));

        if (foo)
        {
            Hash hash(line);
            string fn = string(line + 34);
            Paar paar(hash, fn);
            _checkPaar(paar, os);
        }
    }
    foo.close();
}

int AppMD5Sum::run(int argc, char **argv)
{
    Options options;
    options.parse(argc, argv);

    if (options.cin())
        _hashCin(cout);

    if (options.compare())
    {
        _hasher.stream(cin);
        Hash h(options.compareSum().c_str());

        if (_hasher.hash().isEqual(h))
            cout << "\e[1;32mOK\e[0m\n";
        else
            cout << "\e[1;31mFAILED\e[0m\n";

        //cout << _hasher.hash().toString() << "\n";
    }

    Vest files = options.files();

    for (Vest::iterator it = files.begin(); !options.check() && it != files.end(); it++)
        _hashFile(*it, cout);

    for (Vest::iterator it = files.begin(); options.check() && it != files.end(); it++)
        _checkFile(*it, cout);
    
    return 0;
}

void AppMD5Sum::_checkPaar(Paar &paar, ostream &os)
{
    ifstream file;
    file.open(paar.fn().c_str(), fstream::in | fstream::binary);
    _hasher.stream(file);
    file.close();
    Hash hash2 = _hasher.hash();

    if (paar.hash().isEqual(hash2))
        os << "\e[1;32m" << paar.fn() << ": OK\e[0m\n";
    else
        os << "\e[1;31m" << paar.fn() << ": FAILED\e[0m\n";
}

int main(const int argc, char **argv)
{
    AppMD5Sum app;
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


