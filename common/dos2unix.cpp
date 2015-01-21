#include "mystl.h"
using namespace mystl;

int main(int argc, char **argv)
{
    while (cin)
    {
        int c = cin.get();
        
        if (c != '\r' && cin)
            cout.put(c);
    }

    return 0;
}


