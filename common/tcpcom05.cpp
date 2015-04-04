// array::fill example
//#include <iostream>
//#include <array>
#include "common.h"

int main () {
  array<int,6> myarray;

  myarray.fill(5);

  cout << "myarray contains:";
  for ( int& x : myarray) { cout << ' ' << x; }

  cout << '\n';

  return 0;
}


