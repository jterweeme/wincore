#include "kompakt.h"
using namespace std;

template <typename T> void myAssert(T a, T b, const char *err = "error")
{
    if (a != b)
        throw err;
}

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
    ta.push("-s");
    ta.push("-v");
    options.parse(ta.argc(), ta.argv());
 
    if (!options.verbose())
        throw "Verbose";   

    if (!options.stdinput())
        throw "Standard input option";

    if (options.help())
        throw "Help option";
}

void test2()
{
    ISO iso;
    ifstream ifs;
    ifs.open("cd1.iso");
    
    if (!ifs.is_open())
        throw "File does not exist";

    iso.read(ifs);


    if (iso.directories()[0][2].dir().dataLengthLE != 1679)
        throw "Bestand moet 1679 bytes groot zijn";


    ifs.close();
}

void testCD6()
{
    ifstream ifs("cd6.iso");

    if (!ifs.is_open())
        throw "File does not exist";

    ISO iso;
    iso.read(ifs);
    PathTable pt = iso.pathTable();
    myAssert(pt.offsetLBA(), (uint32_t)19);
    myAssert(pt.size(), (size_t)54);
    PathEntry root = pt.root();
    myAssert(root.offsetLBA(), (uint32_t)23);
    FSPath fsp = pt.recurse(26);
    fsp.dump(cout);
    cout << "\n";
}

int main()
{

    try
    {
        test1();
        test2();
        testCD6();
    }
    catch (const char *s)
    {
        cerr << "\033[1;31mTest1: Failed! " << s << "\033[0m\n";
        return 0;
    }
/*
    Options options;
    TestArguments ta;
    ta.push("kompakt");
    ta.push("-i");
    options.parse(ta.argc(), ta.argv());
    cout << options.info() << options.list() << "\n";
*/

    cerr << "\033[1;32mTest1: OK\033[0m\n";
    return 0;
}


