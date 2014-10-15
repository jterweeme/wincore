#include "kompakt.h"

template <typename T> void myAssert(T a, T b)
{
    if (a != b)
        throw "error";
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
    myAssert(bitset1.test(7), true);
    myAssert(bitset1.count(), (size_t)8);
    bitset1.reset(2);
    myAssert(bitset1.test(2), false);
    myAssert(bitset1.count(), (size_t)7);
    bitset1.reset();
    myAssert(bitset1.test(4), false);
    myAssert(bitset1.any(), false);
    myAssert(bitset1.count(), (size_t)0);
}

int main()
{
    test();
    testBitset();
    return 0;
}


