#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        ifstream ifs(argv[i], ifstream::in);
        
        for (int c = ifs.get(); !ifs.eof(); c = ifs.get())
            cout << (char)c;

    }
    return 0;
}


