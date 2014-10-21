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

class Intor : public vector<int>
{
public:
    void dump(ostream &os);
};

void Intor::dump(ostream &os)
{
    for (iterator it = begin(); it != end(); it++)
        os << *it << "\n";
}

void testReverse()
{
    Intor v1;
    v1.push_back(20);
    v1.push_back(5);
    v1.push_back(6);
    v1.push_back(2);
    v1.push_back(4);
    v1.push_back(99);
    myAssert(v1[0], 20);
    myAssert(v1[1], 5);
    myAssert(v1[2], 6);
    myAssert(v1[3], 2);
    v1.dump(cout);
    cout << "\n";
    reverse(v1.begin(), v1.end());
    myAssert(v1[2], 2);
    myAssert(v1[3], 6);
    myAssert(v1[4], 5);
    myAssert(v1[5], 20);
    v1.dump(cout);
    cout << "\n";
    sort(v1.begin(), v1.end());
#if 0
    myAssert(v1[0], 2);
    myAssert(v1[1], 4);
    myAssert(v1[2], 5);
    myAssert(v1[3], 6);
#endif
    v1.dump(cout);
}

bool mypredicate (int i, int j)
{
    return (i==j);
}

void testEqual()
{
    int myints[] = {20,40,60,80,100};
    vector<int> myvector(myints, myints + 5);
#if 0
    myvector.push_back(20);
    myvector.push_back(40);
    myvector.push_back(60);
    myvector.push_back(80);
    myvector.push_back(100);
#endif
    myAssert(equal(myvector.begin(), myvector.end(), myints), true);
    myvector[3] = 81;
    myAssert(equal(myvector.begin(), myvector.end(), myints, mypredicate), false);
}

void testCount()
{
    int myInts[] = {10,20,30,30,20,10,10,20};
    vector<int> myInts2(myInts, myInts + 8);
    int myCount = count(myInts, myInts + 8, 10);
    myAssert(myCount, 3, "Count Failed");
    cout << "10 appears: " << myCount << " times.\n";
    vector<int> v1;
    
    for (int i = 0; i < 8; i++)
        v1.push_back(myInts[i]);

    myCount = count(v1.begin(), v1.end(), 20);
    myAssert(myCount, 3, "Count Failed");
    cout << "20 appears: " << myCount << " times.\n";
}

void testIntersect()
{
    vector<int> v1;
    v1.push_back(5);
    v1.push_back(7);
    vector<int> v2;
    v2.push_back(3);
    v2.push_back(9);

    int first[] = {5,10,15,20,25};
    int second[] = {50,40,30,20,10};
    vector<int> v(10);
    vector<int>::iterator it;

    //std::sort (first,first+5);
    sort(second, second + 5);
    it = set_intersection(first, first+5, second, second+5, v.begin());
    v.resize(it - v.begin());
    myAssert(v.size(), (size_t)2, "Set Intersection Failed");
    cout << "The intersection has " << (v.size()) << " elements:\n";

    for (it=v.begin(); it!=v.end(); ++it)
        cout << ' ' << *it;

    cout << '\n';
}

int main()
{
    try
    {
        testReverse();
        testCount();
        testEqual();
        testIntersect();
    }
    catch (const char *e)
    {
        cerr << "\033[1;31mTest5: " << e << "\033[0m\n";
    }
    catch (...)
    {
        return -1;
    }
    return 0;
}


