#if 1
#include "mystl.h"
using namespace mystl;
#else
#include <iostream>
#include <algorithm>
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

class MyVector : public vector<int>
{
public:
    MyVector() : vector<int>() { }
    MyVector(size_t first, int last) : vector<int>(first, last) { }
    MyVector(int *first, int *last) : vector<int>(first, last) { }
    void dump(ostream &os);
};

void MyVector::dump(ostream &os)
{
    for (iterator it = begin(); it != end(); it++)
        os << " " << *it;
}

void testIterSwap()
{
    int myints[] = {10,20,30,40,50 };
    MyVector v1(4, 99);
    myAssert(v1[0], 99);
    myAssert(v1[1], 99);
    myAssert(v1[2], 99);
    myAssert(v1[3], 99);
    cout << "myvector contains:";
    v1.dump(cout);
    cout << "\n";
    iter_swap(myints, v1.begin());
    myAssert(v1[0], 10);
    myAssert(v1[1], 99);
    myAssert(v1[2], 99);
    myAssert(v1[3], 99);
    cout << "myvector contains:";
    v1.dump(cout);
    cout << "\n";
    iter_swap(myints + 3, v1.begin()+2);
    myAssert(v1[0], 10);
    myAssert(v1[1], 99);
    myAssert(v1[2], 40);
    myAssert(v1[3], 99);
    cout << "myvector contains:";
    v1.dump(cout);
    cout << "\n";
}

#if 0
void testSortHeap()
{
    int myints[] = {8, 50, 4, 45, 22, 99, 2000, 144, 38};
    MyVector v(myints, myints + 9);
    make_heap(v.begin(), v.end());
    sort_heap(v.begin(), v.end());
    myAssert(v[0], 4);
    myAssert(v[8], 2000);
    v.dump(cout);
    cout << "\n\n";
}

void testHeapPush()
{
    int myints[] = {10,20,30,5,15};
    MyVector v(myints,myints + 5);
    myAssert(v[0], 10);
    myAssert(v[1], 20);
    myAssert(v[2], 30);
    myAssert(v[3], 5);
    myAssert(v[4], 15);
    make_heap (v.begin(),v.end());
    myAssert(v.front(), 30);
    cout << "initial max heap   : " << v.front() << "\n";
    pop_heap (v.begin(),v.end()); v.pop_back();
    myAssert(v.front(), 20);
    cout << "max heap after pop : " << v.front() << "\n";
    v.push_back(99); push_heap (v.begin(),v.end());
    myAssert(v.front(), 99);
    cout << "max heap after push: " << v.front() << "\n";
    sort_heap(v.begin(),v.end());
    myAssert(v[0], 5);
    myAssert(v[1], 10);
    myAssert(v[2], 15);
    myAssert(v[3], 20);
    myAssert(v[4], 99);   
    cout << "final sorted range :";
    v.dump(cout);
    cout << "\n";
}
#endif

int main()
{
    try
    {
        testIterSwap();
        //testHeapPush();
        //testSortHeap();
    }
    catch (const char *e)
    {
        cerr << "\033[1;31mTest6: " << e << "\033[0m\n";
        return 0;
    }
    cerr << "\033[1;32mTest6: OK\033[0m\n";
    return 0;
}



