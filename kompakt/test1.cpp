#include "kompakt.h"
#include <string.h>
using namespace std;

class TestArguments
{
    int _argc;
    char **_argv;
public:
    TestArguments() : _argc(0) { _argv = new char*[10]; }
    ~TestArguments();
    void push(const char *s) { _argv[_argc] = new char[10]; strcpy(_argv[_argc++], s); }
    int argc() const { return _argc; }
    char **argv() const { return _argv; }
};

TestArguments::~TestArguments()
{
    for (int i = 0; i < _argc; i++)
        delete[] _argv[i];

    delete[] _argv;
}

void test1()
{
    Options options;
    TestArguments ta;
    ta.push("kompakt");
    ta.push("-i");
    options.parse(ta.argc(), ta.argv());
    
    if (!options.info())
        throw "Info option";

    if (!options.stdinput())
        throw "Standard input option";

    if (options.help())
        throw "Help option";
}

int main()
{
    try
    {
        test1();
    }
    catch (const char *s)
    {
        cerr << "Failed! " << s << "\n";
        return -1;
    }
/*
    Options options;
    TestArguments ta;
    ta.push("kompakt");
    ta.push("-i");
    options.parse(ta.argc(), ta.argv());
    cout << options.info() << options.list() << "\n";
*/
    return 0;
}


