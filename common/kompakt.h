#ifndef _MAIN_H_
#define _MAIN_H_
#include "filesys.h"

/*
todo: skip/ignore functie maken
duplicate bestanden oplossen
*/

class mistream
{   
protected:
    size_t _pos;
    FILE *_fp;
    size_t _lastRead;
    bool _eof;
public:
    mistream() : _pos(0), _lastRead(0), _eof(false) { }
    mistream(FILE *fp) : _pos(0), _fp(fp), _lastRead(0), _eof(false) { }
    virtual ~mistream() { }
    virtual mistream &ignore(size_t n = 1, int d = '\n') { while (n--) get(); return *this; }
    virtual int get() { _pos++; return fgetc(_fp); }
    int tellg() { return _pos; } 
    int gcount() { return _lastRead; }
    operator void * () const { return (void *)!_eof; }
    virtual void getline(char *dest, size_t size);
    virtual void read(char *s, size_t length);
};

class mifstream : public mistream
{
    bool _open;
public:
    bool is_open() const { return _open; }
    static const uint8_t in = 1;
    static const uint8_t binary = 2;
    mifstream() : _open(false) { }
    mifstream(const char *s, int m = 1) : mistream(fopen(s, "rb")) { _open = _fp; }
    void open(const char *fn, int mode = 1) { _open = _fp = fopen(fn, "rb"); }
    void close() { if (_open) { fclose(_fp); _open = false; } }
    virtual ~mifstream() { close(); }
};

class Options
{
    bool _verbose;
    bool _list;
    bool _extract;
    bool _help;
    bool _cin;
    bool _file;
    bool _desc;
    bool _pathTable;
    bool _outputDir;
    string _fn;
    string _outputDirName;
public:
    Options()
      :
        _verbose(false),
        _list(false),
        _extract(false),
        _help(false),
        _cin(false),
        _file(false),
        _desc(false),
        _pathTable(false),
        _outputDir(false)
    { }

    string fn() const { return _fn; }
    bool file() const { return _file; }
    int parse(int argc, char **argv);
    bool verbose() const { return _verbose; }
    bool help() const { return _help; }
    bool list() const { return _list; }
    bool extract() const { return _extract; }
    bool stdinput() const { return _cin; }
    bool pathTable() const { return _pathTable; }
    bool desc() const { return _desc; }
    bool outputDir() const { return _outputDir; }
    string outputDirName() const { return _outputDirName; }
    void dump(ostream &os) { os << "Filename: " << _fn << "\n"; }
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

struct SFile
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
} PACKED;

class Flags : public bitset<8>
{
public:
    Flags(uint8_t flags) : bitset<8>(flags) { }
    void dump(ostream &os);
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class DirEntry
{
    string _fn;
    SFile _dir;
    uint32_t _offset;
    uint32_t _parentLBA;
public:
    DirEntry() : _dir(), _offset(0), _parentLBA(0) { }
    void length(uint8_t l) { _dir.length = l; }
    uint8_t length() const { return _dir.length; }
    SFile dir() const { return _dir; }
    string fn();
    bool isDir() { Flags flags(_dir.flags); return flags.test(1); }
    uint8_t read(mistream &s);
    void parentLBA(uint32_t n) { _parentLBA = n; }
    uint32_t parentLBA() { return _parentLBA; }
    uint32_t size() const { return _dir.dataLengthLE; }
    void dump(ostream &os);
    bool compare(DirEntry &de) { return de.dir().lbaLE == _dir.lbaLE; }
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
            uint32_t pathTable;
            uint32_t optionalTable;
            uint32_t pathMTable;
            uint32_t optionalPathMTable;
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
            char appIdent[128];
            char copyrightIdent[38];
            char abstractIdent[36];
            char bibIdent[37];
            char creationTime[17];
            char modTime[17];
            char expTime[17];
            char effectTime[17];
            uint8_t fsv;
            uint8_t unused;
            uint8_t data[1165];
        } PACKED;
        uint8_t terminator[2041];
    };
} PACKED;

struct SPathEntry
{
    uint8_t length;
    uint8_t extLength;
    uint32_t lba;
    uint16_t parent;
} PACKED;

class PathEntry
{
    SPathEntry _pe;
    char _name[255];
    int _index;
public:
    PathEntry() { memset(_name, 0, sizeof(_name)); }
    uint32_t offsetLBA() const { return _pe.lba; }
    uint32_t offset() const { return offsetLBA() * 2048; }
    void read(mistream &is);
    void dump(ostream &os);
    char *name() { return _name; }
    void index(int i) { _index = i; }
    int index() const { return _index; }
    uint16_t parent() const { return _pe.parent; }
    bool compare(PathEntry &p) { return offsetLBA() == p.offsetLBA(); }
};

class PathTable : public vector<PathEntry>
{
    static bool wayToSort(const PathEntry &p1, const PathEntry &p2);
    uint32_t _offsetLBA;
    size_t _size;
public:
    void read(mistream &is, int lba, size_t n);
    void snort() { sort(begin(), end(), wayToSort); }
    void dump(ostream &os);
    PathEntry &getByLBA(uint32_t lba);
    PathEntry &getByIndex(int i);
    bool isRoot(PathEntry &pe) { return pe.compare(root()); }
    PathEntry &root() { iterator it = begin(); return *it; }
    uint32_t offsetLBA() const { return _offsetLBA; }
    size_t size() const { return _size; }
    FSPath recurse(uint32_t lba);
};

class CVolumeDescriptor
{
public:
    SVolumeDescriptor _desc;
public:
    virtual ~CVolumeDescriptor() { }
    const char *typeString();
    static const uint8_t SUPPLEMENTARY_VOLUME_DESCRIPTOR = 2;
    static const uint8_t VOLUME_DESCRIPTOR_SET_TERMINATOR = 255;
    int read(mistream &s) { s.read((char *)&_desc, sizeof(_desc)); return 0; }
    void erase() { memset((void *)&_desc, 0, sizeof(_desc)); }
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
    uint32_t pathTableOffset() const { return _desc.pathTable; }
    size_t pathTableSize() const { return _desc.pathTableSizeLSB; }
    void test();
    uint32_t offsetRootDir() const { return _desc.lbaLSB; }
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
    int read(mistream &s);
    void dump(ostream &os);
    uint32_t pathTableOffset();
    size_t pathTableSize();
    uint32_t offsetRootDir() { return 0; }
    string toString() { ostringstream oss; dump(oss); return oss.str(); }
};

class Directory : public vector<DirEntry>
{
    static bool sortLBA(const DirEntry &p1, const DirEntry &p2)
    { return p1.dir().lbaLE < p2.dir().lbaLE; }
public:
    void read(mistream &s, uint32_t offset);
    string toString();
    void list(ostream &os, int mode = 1);
    string list(int mode = 1) { ostringstream oss; list(oss, mode); return oss.str(); }
    void snort() { sort(begin(), end(), sortLBA); }
};

class Directories : public vector<Directory>
{
public:
    void read(mistream &is, PathTable &pt);
    void list(ostream &os, int mode = 1);
};

class ISO
{
    Descriptors _descriptors;
    PathTable _pathTable;
    Directories _directories;
    Directory _fileList;
public:
    int read(mistream &s);
    int extract(mistream &s);
    int extract(mistream &s, const char *outputDir);
    void list(ostream &os, int mode = 1) { _directories.list(os, mode); }
    void dumpDescriptors(ostream &os) { _descriptors.dump(os); }
    void dumpPathTable(ostream &os) { _pathTable.dump(os); }
    Descriptors descriptors() const { return _descriptors; }
    Directories directories() const { return _directories; }
    PathTable pathTable() const { return _pathTable; }
};

class App
{
    Options _options;
    ISO _iso;
    mifstream _ifs;
public:
    void help(ostream &os) const;
    string help() { ostringstream oss; help(oss); return oss.str(); }
    int run(int argc, char **argv);
};

#endif


