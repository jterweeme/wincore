#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>
#include <iostream>
#include <vector>
#include <bitset>

class Options
{
    bool _info;
    bool _list;
    bool _extract;
    bool _help;
    std::string _fn;
public:
    Options() { _info = _list = _extract = _help = false; }
    std::string fn() const { return _fn; }
    int parse(int argc, char **argv);
    bool info() const { return _info; }
    bool help() const { return _help; }
    bool list() const { return _list; }
    bool extract() const { return _extract; }
    bool stdinput() const { return (_fn.length() < 3); }
    std::string toString();
};

class Util
{
public:
    static std::string foo(const char *s, int length) { return std::string(s, s + length); }
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

class CDirectory
{
    std::string _fn;
    SDirectory _dir;
public:
    SDirectory dir() const { return _dir; }
    std::string fn(); //const { return _fn; }
    int read(std::istream &s);
    std::string toString();
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
    const char *typeString();
    static const uint8_t SUPPLEMENTARY_VOLUME_DESCRIPTOR = 2;
    static const uint8_t VOLUME_DESCRIPTOR_SET_TERMINATOR = 255;
    SVolumeDescriptor desc;
    int read(std::istream &s);
    virtual std::string toString();
};

class CPrimaryVolumeDesc : public CVolumeDescriptor
{
public:
    static const uint8_t PRIMARY_VOLUME_DESCRIPTOR = 1;
    CPrimaryVolumeDesc();
    CPrimaryVolumeDesc(const CVolumeDescriptor &vd);
    std::string toString();
};

class CVolumeDescriptorSetTerminator : public CVolumeDescriptor
{
public:
    CVolumeDescriptorSetTerminator();
    CVolumeDescriptorSetTerminator(const CVolumeDescriptor &vd);
    std::string toString();
};

class VolDescFactory
{
public:
    CVolumeDescriptor *create(CVolumeDescriptor &vd);
};

class Descriptors : public std::vector<CVolumeDescriptor *>
{
public:
    int read(std::istream &s);
    std::string toString();
};

class Directories : public std::vector<CDirectory>
{
public:
    int read(std::istream &s, Descriptors &d);
    std::string toString();
    std::string list(int mode = 1);
};

class ISO
{
    Descriptors descriptors;
    Directories directories;
public:
    int read(std::istream &s);
    int extract(std::istream &s);
    std::string list(int mode = 1);
};

class Flags : std::bitset<8>
{
public:
    Flags(uint8_t flags) : std::bitset<8>(flags) { }
    std::string toString();
};

class App
{
    Options options;
public:
    std::string help();
    int run(int argc, char **argv);
};


#endif


