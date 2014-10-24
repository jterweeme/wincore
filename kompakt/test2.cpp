#include "kompakt.h"
#include <sys/stat.h>

template <typename T> void myAssert(T a, T b, const char *err = "error")
{
    if (a != b)
        throw err;
}

void test()
{
    ifstream ifs("cd6.iso");
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

void test2()
{
    FileSystem fs;
    fs.mkdir("data");
    fs.rmdir("data");
}

void testFS()
{
    FileSystem fs;
    fs.root().dump(cout);
    cout << "\n";
    FSPath pwd = fs.pwdir();
    pwd.dump(cout);
    cout << "\n";
}

void testCHMkdir()
{
    FileSystem fs;
    int depth = fs.pwdir().size();
    FSPath fsp;
    fsp.push_back("delta");
    fsp.push_back("echo");
    fsp.push_back("foxtrot");
    fs.chmkdir(fsp);
    myAssert(fs.pwdir().size(), (size_t)(depth + 3));
    fs.up();
    myAssert(fs.pwdir().size(), (size_t)(depth + 2));
    fs.rmdir("foxtrot");
    fs.up();
    fs.rmdir("echo");
    fs.up();
    fs.rmdir("delta");
    fs.up();
    fs.up();
    fs.up();
    myAssert(fs.pwdir().size(), (size_t)depth);
    
}

int main()
{

    try
    {
        test();
        test2();
        testFS();
        testCHMkdir();
    }
    catch (const char *e)
    {
        cerr << "\033[1;31mTest2: " << e << "\033[0m\n";
        return 0;
    }

    cerr << "\033[1;32mTest2: OK\033[0m\n";
    return 0;
}


