#ifndef _MAIN_H_
#define _MAIN_H_
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


#endif



