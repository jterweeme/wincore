#include "tar.h"

#if 0
TarStream::TarStream(FILE *fp, uint8_t comp)
{
    //switch (comp)
    {
    //case 1:
        //_is.rdbuf(new DecStreamBuf(fp));
    //default:
        //_is(buftest(fp))
    }
}
#endif

bool TarStream::listFile(bool verbose)
{
    Header h(*_is);
    if (h.empty()) return false;
    _width = max(h.numDigits(), _width);
    if (verbose) h.fullInfo(cout, _width); else cout << h.name() << "\n";
    _is->ignore(512 - _is->tellg() % 512);
    _is->ignore(h.size());
    _is->ignore(512 - _is->tellg() % 512);
    return true;
}

void Header::fullInfo(ostream &os, uint8_t width) const
{
    os << mode() << " " << uname() << "/" << gname() << " " << setw(width) << size() << " "
       << timeStamp() << " " << name() << "\n";
}

void Header::timeStamp(ostream &os) const
{
    for (uint8_t i = 0; i < 12; i++)
        os.put(_h.mtime[i]);
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



