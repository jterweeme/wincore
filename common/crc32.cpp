#include <stdint.h>
#include <iostream>
#include <iomanip>
using namespace std;

class CRC
{
    unsigned _crc = 0xffffffff;
public:
    unsigned reverse(unsigned x) const;
    unsigned reverse() { return _crc = reverse(~_crc); }
    void crc32a(unsigned byte);
    void crc32a(istream &is);
};

unsigned CRC::reverse(unsigned x) const
{
   x = (x & 0x55555555) << 1 | x >> 1 & 0x55555555;
   x = (x & 0x33333333) << 2 | x >> 2 & 0x33333333;
   x = (x & 0x0F0F0F0F) << 4 | x >> 4 & 0x0F0F0F0F;
   x = x << 24 | x & 0xFF00 << 8 | x >> 8 & 0xFF00 | x >> 24;
   return x;
}

void CRC::crc32a(unsigned byte)
{
    byte = reverse(byte);

    for (int j = 0; j <= 7; j++)
    {
        _crc = (int)(_crc ^ byte) < 0 ? _crc << 1 ^ 0x04C11DB7 : _crc << 1;
        byte <<= 1;
    }
}

void CRC::crc32a(istream &is)
{
    for (int c = is.get(); !is.eof(); c = is.get())
        crc32a(c);
}

int main(int argc, char **argv)
{
    CRC crc;
    crc.crc32a(cin);
    cout << hex << crc.reverse() << "\n";
    return 0;
}


