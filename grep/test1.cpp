#include <iostream>
#include <regex>
using namespace std;

int main ()
{
    char line[] = "test.cpp";
    regex rx("test");
    
    bool result = regex_search(line, rx);


  return 0;
}


