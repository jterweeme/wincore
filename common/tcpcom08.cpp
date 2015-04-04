// string::cbegin/cend
#include "common.h"

int main ()
{
  string str ("Lorem ipsum");
  for (auto it=str.cbegin(); it!=str.cend(); ++it)
    cout << *it;
  cout << '\n';

  return 0;
}


