#ifndef _MAIN_H_
#define _MAIN_H_
#include "hasher.h"

typedef vector<string> Files;

class Options
{
    bool _check;
#ifdef __GNUC__foo
    Files _files;
#endif
    bool _cin;
    int _argc;
    char **_argv;
    int _argp;
public:
    Options(int argc, char **argv)
      : _check(false), _cin(false), _argc(argc), _argv(argv), _argp(0)
    { }

    bool check() const { return _check; }
#ifdef __GNUC__foo
    Files &files() { return _files; }
#endif
    void parse(int argc, char **argv);
    void parse() { parse(_argc, _argv); }
    void dump(ostream &os);
    bool cin() const { return _cin; }
    bool isFile(char *arg) { return arg[0] != '-'; }

    bool hasNextFileName()
    {
        for (int i = _argp + 1; i < _argc; i++)
            if (isFile(_argv[i]))
                return true;

        return false;
    }

    string nextFileName()
    {
        while (!isFile(_argv[++_argp]));
        return string(_argv[_argp]);
    }
};

class Paar
{
    Hash _hash;
    string _fn;
public:
    Paar() { }
    Paar(Hash &hash, string &fn) : _hash(hash), _fn(fn) { }
    void check();
    void read(istream &is);
    Hash hash() { return _hash; }
    string fn() { return _fn; }
    void dump(ostream &os);
};

class Paars : public vector<Paar>
{
public:
    void dump(ostream &os);
    void check(ostream &os);
};

class App
{
    Hasher _hasher;
    Options _options;
    Paars _paars;
    void checkFile2(const char *fn);
public:
    App(int argc, char **argv) : _options(argc, argv) { }
    int run();
};


#endif



