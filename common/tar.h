#ifndef _TAR_H_
#define _TAR_H_
#if 0
#include <iostream>
#include <fstream>
#include <stdint.h>
#endif
//#include <cstdlib>
//#include <cstring>
#include <vector>
//#include <iomanip>
//#include <sstream>
#include "bunzip2.h"
#include "buftest.h"

#ifdef __GNUC__
#define PACKED __attribute__ ((packed))
#else
#define PACKED
#endif

typedef vector<string> Vest;

struct SHeader
{
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char chksum[8];
    char typeflag;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
} __attribute__ ((packed));


class Header
{
    SHeader _h;
    string _mode(char c) const;
public:
    Header() { }
    Header(SHeader &h) : _h(h) { }
    Header(istream &is) { read(is); }
    void read(istream &is) { is.read((char *)&_h, sizeof(_h)); }
    uint32_t size() const { return strtol(_h.size, 0, 8); }
    uint8_t numDigits() const;
    string name() const { return string(_h.name); }
    string uname() const { return string(_h.uname); }
    string gname() const { return string(_h.gname); }
    bool empty() const { return _h.name[0] == 0; }
    SHeader raw() const { return _h; }
    void fullInfo(ostream &os, uint8_t width) const;
    void mode(ostream &os) const;
    void timeStamp(ostream &os) const;
    string mode() const { ostringstream s; mode(s); return s.str(); }
    string fullInfo(uint8_t w) const { ostringstream s; fullInfo(s, w); return s.str(); }
};

class TarStream
{
protected:
    istream *_is;
    uint8_t _width = 5;
public:
    TarStream(istream *is) : _is(is) { }
    bool listFile(bool verbose);
    void list(bool verbose) { while (listFile(verbose)); }
    bool extractFile(bool verbose);
    void extract(bool verbose) { while (extractFile(verbose)); }
};

#endif


