#if 0
#include <iostream>
#include <fstream>
using namespace std;
#else
namespace mystl { }
using namespace mystl;
#include "common.h"
#endif

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        ifstream ifs(argv[i], ifstream::in);
        
        //for (int c = ifs.get(); !ifs.eof(); c = ifs.get())
        for (int c; (c = ifs.get()) != EOF;)
            cout << (char)c;

    }
    return 0;
}


