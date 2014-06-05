#include <fstream>
#include <iostream>
#include <stdint.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

class Hex
{
public:
    static void dump(std::istream &input);
};

void Hex::dump(std::istream &input)
{
    uint8_t arr[17] = { 0 };

    while (!input.eof())
    {
        input.read((char *)arr, 16);

        if (input.gcount() <= 0)
            break;

        std::cout << std::hex;
        std::cout << (int)arr << std::endl;
    }
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    _setmode(_fileno(stdin), O_BINARY);
#endif
    Hex::dump(std::cin);
    return 0;
}


