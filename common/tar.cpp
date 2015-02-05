#include "tar.h"

void Header::fullInfo(ostream &os, uint8_t width) const
{
    os << mode() << " " << uname() << "/" << gname() << " " << setw(width) << size() << " "
       << "1990-01-01 00-00 " << name() << "\n";
}

void Header::timeStamp(ostream &os) const
{
}

uint8_t Header::numDigits() const
{
    uint8_t digits = 0;
    uint32_t number = size();

    while (number)
        number /= 10, digits++;

    return digits;
}

string Header::_mode(char c) const
{
    switch (c)
    {
        case '7': return "rwx";
        case '6': return "rw-";
        case '5': return "r-x";
        case '4': return "r--";
    }

    return "---";
}

void Header::mode(ostream &os) const
{
    os << "-" << _mode(_h.mode[4]) << _mode(_h.mode[5]) << _mode(_h.mode[6]);
}


