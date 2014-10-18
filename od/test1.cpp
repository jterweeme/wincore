#include "mystl.h"
#include <iostream>
using namespace std;


class Nester
{
    vector2<std::string> foo;
};

#if 0
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
#endif

void test2()
{
    vector2<Nester> bar;
    Nester nester1;
    bar.push_back(nester1);
}

int main()
{
    //test1();
    test2();
    return 0;

}


