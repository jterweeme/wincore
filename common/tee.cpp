#if 0
#include <iostream>
#include <fstream>
using namespace std;
#else
#include "common.h"
#endif

class teebuf : public streambuf
{
    streambuf *sb1, *sb2;
    virtual int overflow(int c) { return sb1->sputc(c) == EOF || sb2->sputc(c) == EOF ? EOF : c; }
    virtual int sync() { return sb1->pubsync() == 0 && sb2->pubsync() == 0 ? 0 : -1; }
public:
    teebuf(streambuf *sb1, streambuf *sb2) : sb1(sb1), sb2(sb2) { }
};

class teestream : public ostream
{
    teebuf tbuf;
public:
    teestream(ostream &o1, ostream &o2) : ostream(&tbuf), tbuf(o1.rdbuf(), o2.rdbuf()) { }
};

class AppTee
{
public:
    int run(int argc, char **argv);
};

int AppTee::run(int argc, char **argv)
{
    ofstream ofs(argv[1]);
    teestream tee(cout, ofs);
    for (int c; (c = cin.get()) != EOF;) tee.put(c);
    ofs.close();
    return 0;
}

int main(int argc, char **argv)
{
    AppTee app;
    return app.run(argc, argv);
}


