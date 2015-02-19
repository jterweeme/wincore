namespace mystl { };
using namespace mystl;
#include "mystl.h"

int main(int argc, char **argv)
{
    while (cin)
    {
        char c = cin.get();

        if (c == 0x0a)
            cout << "\r\n";
        else
            cout.put(c);
    }
    return 0;
}


