#ifndef _MAIN_H_
#define _MAIN_H_
#include "common.h"

class Options
{
    bool _verbose;
    bool _list;
    bool _extract;
    bool _help;
    bool _cin;
    bool _file;
    bool _desc;
    string _fn;
public:
    Options() { _verbose = _desc = _list = _extract = _cin = _file = _help = false; }
    string fn() const { return _fn; }
    bool file() const { return _file; }
    int parse(int argc, char **argv);
    bool verbose() const { return _verbose; }
    bool help() const { return _help; }
    bool list() const { return _list; }
    bool extract() const { return _extract; }
    bool stdinput() const { return _cin; }
    bool desc() const { return _desc; }
    void dump(ostream &os) { os << "Filename: " << _fn << "\n"; }
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class Util
{
public:
    static string foo(const char *s, int length) { return string(s, s + length); }
};

struct SDirectory
{
    uint8_t length;
    uint8_t extendedLength;
    uint32_t lbaLE;
    uint32_t lbaBE;
    uint32_t dataLengthLE;
    uint32_t dataLengthBE;
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t timezone;
    uint8_t flags;
    uint8_t interleavedFileUnit;
    uint8_t interleavedGapSize;
    uint16_t volSeqNumLE;
    uint16_t volSeqNumBE;
    uint8_t fnLength;
} __attribute__ ((packed));

class Flags : public bitset<8>
{
public:
    Flags(uint8_t flags) : bitset<8>(flags) { }
    void dump(ostream &os);
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class CDirectory
{
    string _fn;
    SDirectory _dir;
public:
    SDirectory dir() const { return _dir; }
    string fn();
    bool isDir() { Flags flags(_dir.flags); return flags.test(1); }
    int read(istream &s);
    uint32_t size() const { return _dir.dataLengthLE; }
    void dump(ostream &os);
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

struct SVolumeDescriptor
{
    uint8_t type;
    char identifier[5];
    uint8_t version;
    union
    {
        struct
        {
            uint8_t unused_1;
            char sysident[32];
            char volident[32];
            uint8_t unused_2[8];
            uint32_t volumeSpaceSizeLSB;
            uint32_t volumeSpaceSizeMSB;
            uint8_t unused_3[32];
            uint16_t volumeSetSizeLSB;
            uint16_t volumeSetSizeMSB;
            uint8_t volumeSequenceSize[4];
            uint16_t logicalBlockSizeLSB;
            uint16_t logicalBlockSizeMSB;
            uint32_t pathTableSizeLSB;
            uint32_t pathTableSizeMSB;
            uint8_t pathTable[4];
            uint8_t optionalTable[4];
            uint8_t pathMTable[4];
            uint8_t optionalPathMTable[4];
            uint8_t directoryLength;
            uint8_t extendedLength;
            uint32_t lbaLSB;
            uint32_t lbaMSB;
            uint32_t dataLengthLSB;
            uint32_t dataLengthMSB;
            uint8_t year;
            uint8_t month;
            uint8_t day;
            uint8_t hour;
            uint8_t minute;
            uint8_t second;
            uint8_t timezone;
            uint8_t directoryEntry[9];
            char volSetIdent[128];
            char pubIdent[128];
            char prepIdent[128];
            uint8_t data[1474];
        } __attribute__ ((packed));
        uint8_t terminator[2041];
    };
} __attribute__ ((packed));

class CVolumeDescriptor
{
public:
    virtual ~CVolumeDescriptor() { }
    const char *typeString();
    static const uint8_t SUPPLEMENTARY_VOLUME_DESCRIPTOR = 2;
    static const uint8_t VOLUME_DESCRIPTOR_SET_TERMINATOR = 255;
    SVolumeDescriptor desc;
    int read(istream &s);
    virtual void dump(ostream &os);
    virtual string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class CPrimaryVolumeDesc : public CVolumeDescriptor
{
public:
    static const uint8_t PRIMARY_VOLUME_DESCRIPTOR = 1;
    CPrimaryVolumeDesc();
    CPrimaryVolumeDesc(const CVolumeDescriptor &vd);
    void dump(ostream &os);
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class CVolumeDescriptorSetTerminator : public CVolumeDescriptor
{
public:
    CVolumeDescriptorSetTerminator();
    CVolumeDescriptorSetTerminator(const CVolumeDescriptor &vd);
    void dump(ostream &os);
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class VolDescFactory
{
public:
    CVolumeDescriptor *create(CVolumeDescriptor &vd);
};

class Descriptors : public vector<CVolumeDescriptor *>
{
public:
    virtual ~Descriptors() { for (iterator it = begin(); it != end(); it++) delete *it; }
    int read(istream &s);
    void dump(ostream &os);
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class Directories : public vector<CDirectory>
{
public:
    int read(istream &s, Descriptors &d);
    string toString();
    void list(ostream &os, int mode);
    string list(int mode = 1) { ostringstream oss; list(oss, mode); return oss.str(); }
};

class ISO
{
    Descriptors _descriptors;
    Directories _directories;
public:
    int read(istream &s);
    int extract(istream &s);
    void list(ostream &os, int mode = 1) { _directories.list(os, mode); }
    string list(int mode = 1) { return _directories.list(mode); }
    //Descriptors descriptors() const { return _descriptors; }
    void dumpDescriptors(ostream &os);
    Directories directories() const { return _directories; }
};

class App
{
    Options options;
    ISO _iso;
public:
    void help(ostream &os);
    string help() { ostringstream oss; help(oss); return oss.str(); }
    int run(int argc, char **argv);
};


#endif


