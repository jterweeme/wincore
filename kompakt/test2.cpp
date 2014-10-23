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

void testBitset()
{
    bitset<8> bitset1(8);
    myAssert(bitset1.size(), (size_t)8);
    myAssert(bitset1.test(3), true);
    myAssert(bitset1.test(4), false);
    myAssert(bitset1.any(), true);
    myAssert(bitset1.count(), (size_t)1);
    bitset1.set();
    myAssert(bitset1.test(7), true, "Test 7");
    myAssert(bitset1.count(), (size_t)8);
    bitset1.reset(2);
    myAssert(bitset1.test(2), false);
    myAssert(bitset1.count(), (size_t)7);
    bitset1.reset();
    myAssert(bitset1.test(4), false);
    myAssert(bitset1.any(), false);
    myAssert(bitset1.count(), (size_t)0);
}

void test2()
{
    FileSystem fs;
    fs.mkdir("data");
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

int main()
{
    try
    {
        test();
        testBitset();
        //test2();
        testFS();
    }
    catch (const char *e)
    {
        cerr << e << "\n";
        return -1;
    }

    cerr << "\033[1;32mTest2: OK\033[0m\n";
    return 0;
}


