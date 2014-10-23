#if 1
#include "mystl.h"
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

int main()
{
    try
    {
        testVector();
    }
    catch (const char *e)
    {
        cerr << "\033[1;31mTest3: " << e << "\033[0m\n";
        return 0;
    }
    //string s(9, ' ');
    //cout << s;
    cerr << "\033[1;32mTest3: OK\033[0m\n";
    return 0;
}


