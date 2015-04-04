// string::substr
#if 0
#include <iostream>
#include <string>
using namespace std;
#else
#include "common.h"
#endif

int main ()
{
  string str="We think in generalities, but we live in details.";
                                              // (quoting Alfred N. Whitehead)

  string str2 = str.substr (12,12);         // "generalities"

  string::size_type pos = str.find("live"); // position of "live" in str

  string str3 = str.substr (pos);           // get from "live" to the end

  cout << str2 << ' ' << str3 << '\n';

  return 0;
}


