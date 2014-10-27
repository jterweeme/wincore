#include <iostream>
#include <regex>
using namespace std;

int main()
{
    char nummerbord[] = "89-TD-XY";
    regex rx("3");
    bool onzin = regex_search(nummerbord, nummerbord + 8, rx);
    cout << onzin << "\n";
    return 0;
}


