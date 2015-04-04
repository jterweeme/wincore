// string::cbegin/cend
#if 0
#include <iostream>
#include <string>
using namespace std;
#else
#include "common.h"
#endif

int main ()
{
  string str ("Lorem ipsum");
  for (auto it=str.cbegin(); it!=str.cend(); ++it)
    cout << *it;
  cout << '\n';

  return 0;
}


