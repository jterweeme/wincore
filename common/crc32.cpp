#if 0
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
using namespace std;
#else
#include "common.h"
#endif

typedef vector<string> Files;

class Options
{
    bool _cin = false;
    Files _files;
public:
    Files files() const { return _files; }
    bool cin() const { return _cin; }
    void parse(int argc, char **argv);
};

class CRC
{
    unsigned _crc = 0xffffffff;
public:
    unsigned reverse(unsigned x) const;
    unsigned reverse() { return _crc = reverse(~_crc); }
    void crc32a(unsigned byte);
    void crc32a(istream &is);
    void reset() { _crc = 0xffffffff; }
};

void Options::parse(int argc, char **argv)
{
    if (argc == 1)
    {
        _cin = true;
        return;
    }

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
        }
        else
        {
            _files.push_back(argv[i]);
        }
    }
}

unsigned CRC::reverse(unsigned x) const
{
   x = ((x & 0x55555555) << 1) | x >> 1 & 0x55555555;
   x = ((x & 0x33333333) << 2) | x >> 2 & 0x33333333;
   x = ((x & 0x0F0F0F0F) << 4) | x >> 4 & 0x0F0F0F0F;
   x = x << 24 | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | (x >> 24);
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
    Options o;
    o.parse(argc, argv);
    CRC crc;
    ifstream ifs;
    
    if (o.cin())
    {
        crc.crc32a(cin);
        cout << hex << crc.reverse() << "\n";
    }

    if (o.files().size() > 0)
    {
        Files files = o.files();
        
        for (Files::const_iterator it = files.cbegin(); it != files.cend(); it++)
        {
            crc.reset();
            ifs.open(it->c_str());
            crc.crc32a(ifs);
            cout << hex << crc.reverse() << "\n";
            ifs.close();
        }
    }
    
    return 0;
}


