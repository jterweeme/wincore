#include "kompakt.h"
#include <sys/stat.h>

template <typename T> void myAssert(T a, T b, const char *err = "error")
{
    if (a != b)
        throw err;
}

void test()
{
#if 0
    ifstream ifs("cd.iso");
    ISO iso;
    iso.read(ifs);
#endif
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

int main()
{
    try
    {
        test();
        testBitset();
        test2();
    }
    catch (const char *e)
    {
        cerr << e << "\n";
        return -1;
    }

    cerr << "\033[1;32mTest2: OK\033[0m\n";
    return 0;
}


