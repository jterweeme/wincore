#include "tar.h"

bool TarStream::listFile(bool verbose)
{
    Header h(*_is);
    if (h.empty()) return false;

    if (h.uname().length() + h.gname().length() == 9)
        _width = max((uint8_t)7, _width);

    _width = max(h.numDigits(), _width);
    if (verbose) h.fullInfo(cout, _width); else cout << h.name() << "\n";
    _is->ignore((512 - _is->tellg() % 512) % 512);
    _is->ignore(h.size());
    _is->ignore((512 - _is->tellg() % 512) % 512);
    return true;
}

void MyDateTime::dump(ostream &os) const
{
    os << year << "-" << setfill('0') << setw(2)
       << (int)month << "-" << setw(2) << (int)day << " " << (int)hour << ":" << (int)minute;
}

void Header::fullInfo(ostream &os, uint8_t width) const
{
    os << mode() << " " << uname() << "/" << gname();

    switch (uname().length() + gname().length())
    {
    case 9:
        os << "  ";
        break;
    case 3:
        os << "      ";
        break;
    default:
        os << " ";
    }

    os << setw(width) << size() << " " << timeStamp() << " " << name() << "\n";
}

void Header::timeStamp(ostream &os) const
{
    time_t onzin = strtol(_h.mtime, 0, 8);
    struct tm *ptm = localtime(&onzin);
    char s[18] = {0};
    strftime(s, sizeof(s), "%F %R", ptm);
    os << s;
}

uint8_t Header::numDigits() const
{
    uint8_t digits = 0;
    uint32_t number = size();
    while (number) number /= 10, digits++;
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
    os << type() << _mode(_h.mode[4]) << _mode(_h.mode[5]) << _mode(_h.mode[6]);
}

bool TarStream::extractFile(bool verbose)
{
    Header h(*_is);
    if (h.empty()) return false;
    if (verbose) cout << h.name() << "\n";
    _is->ignore(512 - _is->tellg() % 512);
    ofstream ofs(h.name().c_str());
    for (uint32_t i = 0; i < h.size(); i++) ofs.put(_is->get());
    ofs.close();
    _is->ignore(512 - _is->tellg() % 512);
    return true;
}



