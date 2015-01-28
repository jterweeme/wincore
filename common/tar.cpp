#include "tar.h"

void Header::fullInfo(ostream &os) const
{
    os << mode() << " " << uname() << "/" << gname() << " " << setw(5) << size() << " "
       << "1990-01-01 00-00 " << name() << "\n";
}

void Header::timeStamp(ostream &os) const
{
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


