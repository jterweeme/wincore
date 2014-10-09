#ifndef _MAIN_H_
#define _MAIN_H_
#include "hasher.h"

typedef vector<string> Files;

class Options
{
    bool _check;
    Files _files;
    bool _cin;
public:
    Options() : _check(false), _cin(false) { }
    bool check() const { return _check; }
    Files &files() { return _files; }
    void parse(int argc, char **argv);
    void dump(ostream &os);
    bool cin() const { return _cin; }
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
    void checkFile(istream &is);
    void checkFile2(const char *fn);
public:
    int run(int argc, char **argv);
};


#endif



