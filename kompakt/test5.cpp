#if 1
#include "mystl.h"
using namespace mystl;
#else
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;
#endif

template <typename T> void myAssert(T a, T b, const char *err = "error")
{
    if (a != b)
        throw err;
}

void testReverse()
{
    vector<int> v1;
    v1.push_back(20);
    v1.push_back(5);
    v1.push_back(6);
    v1.push_back(2);
    myAssert(v1[0], 20);
    myAssert(v1[1], 5);
    myAssert(v1[2], 6);
    myAssert(v1[3], 2);

    for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
        cout << *it << "\n";

    cout << "\n";
    reverse(v1.begin(), v1.end());
    myAssert(v1[0], 2);
    myAssert(v1[1], 6);
    myAssert(v1[2], 5);
    myAssert(v1[3], 20);

    for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
        cout << *it << "\n";   

//    sort(v1.begin(), v1.end());

    for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
        cout << *it << "\n";
}

void testCount()
{
    int myInts[] = {10,20,30,30,20,10,10,20};
    int myCount = count(myInts, myInts + 8, 10);
    myAssert(myCount, 3);
    cout << "10 appears: " << myCount << " times.\n";
    vector<int> v1;
    
    for (int i = 0; i < 8; i++)
        v1.push_back(myInts[i]);

    myCount = count(v1.begin(), v1.end(), 20);
    myAssert(myCount, 3);
    cout << "20 appears: " << myCount << " times.\n";
}

int main()
{
    testReverse();
    testCount();
    return 0;
}


