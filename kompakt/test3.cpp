#if 1
#include "mystl.h"
#include "color.h"
using namespace mystl;
#else
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
#endif

template <typename T> void myAssert(T a, T b, const char *err = "error")
{
    if (a != b)
        throw err;
}

template <> void myAssert(const char *a, const char *b, const char *err)
{
    if (strcmp(a, b) != 0)
        throw err;
}

void testVector()
{
    vector<int> v1(4, 99);
    myAssert(v1[0], 99);
    myAssert(v1[1], 99);
    myAssert(v1[2], 99);
    myAssert(v1[3], 99);
    
    for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
        cout << *it << " ";

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

void testStrtok()
{
    char str[] ="- This, a sample string.";
    char *pch;
    printf ("Splitting string \"%s\" into tokens:\n", str);
    pch = strtok(str," ,.-");

    while (pch != NULL)
    {
        printf ("%s\n",pch);
        pch = strtok (NULL, " ,.-");
    }
}

int main()
{
    Color ccerr;

    try
    {
        testVector();
        testBitset();
        testStrtok();
    }
    catch (const char *e)
    {
        ccerr.color(2);
        ccerr << "Test3: " << e << "\n";
        return 0;
    }
    //string s(9, ' ');
    //cout << s;
    ccerr.color(1);
    ccerr << "Test3: OK\n";
    return 0;
}


