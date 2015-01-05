#include "od.h"

int main(int argc, char **argv)
{
#ifdef _WIN32
    _setmode(_fileno(stdin), O_BINARY);
#endif
    HexStream hs;

    if (argc < 2)
    {
        hs.dump(cin);
    }
    else
    {
        ifstream s(argv[1], ifstream::in | ifstream::binary);
        hs.dump(s);
    }

    return 0;
}


