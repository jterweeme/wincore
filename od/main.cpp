#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdint.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

class HexStream
{
public:
    void dump(std::istream &input);
};

void HexStream::dump(std::istream &input)
{
    uint8_t arr[16] = { 0 };

    while (input)
    {
        input.read((char *)arr, 16);

        for (size_t i = 0; i < input.gcount(); i++)
            std::cout << std::hex << std::setw(2) << std::setfill('o') << (int)arr[i] << " ";

        std::cout << " >";

        for (size_t i = 0; i < input.gcount(); i++)
        {
            if (arr[i] < 0x20 || arr[i] == 0xff)
                std::cout << ".";
            else
                std::cout << arr[i];
        }

        std::cout << "<" << std::endl;
    }
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    _setmode(_fileno(stdin), O_BINARY);
#endif
    HexStream hs;

    if (argc < 2)
    {
        hs.dump(std::cin);
    }
    else
    {
        std::ifstream s(argv[1], std::ifstream::in | std::ifstream::binary);
        hs.dump(s);
    }

    return 0;
}


