/*
Jasper ter Weeme

TODO: bug: when last read equals 16, another read will be attempted
and another empty line will be displayed.
*/

#include "common.h"
using namespace koe;

class HexStream
{
public:
    void dump(istream &input);
};

void HexStream::dump(istream &input)
{
    char arr[16] = { 0 };
    int pos;

    while (input)
    {
        pos = input.tellg();
        cout << oct << setw(7) << setfill('0') << pos << " ";
        input.read((char *)arr, 16);
        pos += input.gcount();

        for (int i = 0; i < input.gcount(); i++)
            cout << hex << setw(2) << setfill('0') << (int)arr[i] << " ";
        
        for (int i = input.gcount(); i < 16; i++)
            cout << "   ";

        cout << " >";

        for (int i = 0; i < input.gcount(); i++)
        {
            if (arr[i] < 0x20 || arr[i] == 0xff)
                cout << ".";
            else
                cout << arr[i];
        }

        cout << "<\n";
    }

    cout << oct << setw(7) << setfill('0') << pos << "\n";
}

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


