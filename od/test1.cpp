#include "mystl.h"
using namespace mystl;

void test1()
{
    vector2<int> nummers;
    nummers.push_back(2);
    nummers.push_back(5);
    nummers.push_back(7);
    nummers.push_back(11);
    
    for (vector2<int>::iterator it = nummers.begin(); it != nummers.end(); it++)
        cout << *it << "\n";
}

int main()
{
    test1();
    return 0;

}


