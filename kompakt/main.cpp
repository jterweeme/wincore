#include <iostream>
#include <stdint.h>
#include <sstream>
#include <vector>
#include <iterator>
#include <string.h>
#include <typeinfo>
#include <fstream>
#include <bitset>

class Options
{
    bool _info;
    bool _list;
    bool _extract;
    std::string _fn;
public:
    Options() : _info(false), _list(false), _extract(false) { }
    std::string fn() const { return _fn; }
    int parse(int argc, char **argv);
    bool info() const { return _info; }
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

std::string CDirectory::fn()
{
    if (_dir.fnLength == 1)
    {
        return std::string("..");
    }

    return Util::foo(_fn.c_str(), (int)_fn.length() - 2);
}

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
    std::string list();
};

class ISO
{
    Descriptors descriptors;
    Directories directories;
public:
    int read(std::istream &s);
    int extract(std::istream &s);
    std::string list();
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
    int run(int argc, char **argv);
};

std::string Flags::toString()
{
    std::ostringstream ss;

    if (test(0))
        ss << "Hidden ";
    if (test(1))
        ss << "Dir ";
    if (test(2))
        ss << "Associated ";
    if (test(3))
        ss << "Ex Attr ";
    if (test(4))
        ss << "Permissions ";
    if (test(7))
        ss << "Final Dir ";

    return ss.str();
}

int ISO::extract(std::istream &s)
{
    for (Directories::iterator it = directories.begin() + 2; it != directories.end(); it++)
    {
        std::ofstream of;
        of.open(it->fn().c_str());
        s.ignore(it->dir().lbaLE * 2048 - s.tellg());
        char *buf = new char[it->dir().dataLengthLE];
        s.read(buf, it->dir().dataLengthLE);
        of.write(buf, it->dir().dataLengthLE);
        of.close();
        delete buf;
    }
    return 0;
}

std::string Directories::list()
{
    std::ostringstream oss;

    for (iterator it = begin(); it != end(); it++)
        oss << it->fn() << " " << it->dir().dataLengthLE << std::endl;

    return oss.str();
}

std::string ISO::list()
{
    return directories.list();
}

CPrimaryVolumeDesc::CPrimaryVolumeDesc(const CVolumeDescriptor &vd)
{
    ::memcpy(&desc, &vd.desc, sizeof(desc));
}

CVolumeDescriptorSetTerminator::CVolumeDescriptorSetTerminator(const CVolumeDescriptor &vd)
{
    ::memcpy(&desc, &vd.desc, sizeof(desc));
}

int CDirectory::read(std::istream &s)
{
    s.read((char *)&_dir, sizeof(_dir));
    char filename[255] = {0};
    s.read(filename, _dir.fnLength);
    _fn = std::string(filename);
    return 0;
}

std::string Options::toString()
{
    std::ostringstream oss;
    oss << "Filename: " << _fn << std::endl;
    return oss.str();
}

const char *CVolumeDescriptor::typeString()
{
    switch (desc.type)
    {
    case CPrimaryVolumeDesc::PRIMARY_VOLUME_DESCRIPTOR:
        return "Primary Volume Descriptor";
    case SUPPLEMENTARY_VOLUME_DESCRIPTOR:
        return "Supplementary Volume Descriptor";
    case VOLUME_DESCRIPTOR_SET_TERMINATOR:
        return "Volume Descriptor Set Terminator";
    default:
        return "Onbekend";
    }
}

CVolumeDescriptor *VolDescFactory::create(CVolumeDescriptor &vd)
{
    switch (vd.desc.type)
    {
    case CPrimaryVolumeDesc::PRIMARY_VOLUME_DESCRIPTOR:
        return new CPrimaryVolumeDesc(vd);
    case CVolumeDescriptor::VOLUME_DESCRIPTOR_SET_TERMINATOR:
        return new CVolumeDescriptorSetTerminator(vd);
    default:
        return new CVolumeDescriptor(vd);
    }
}

std::string Descriptors::toString()
{
    std::ostringstream oss;
 
    for (iterator it = begin(); it != end(); it++)
        oss << (*it)->toString() << std::endl << std::endl;
   
    return oss.str();
}

int Descriptors::read(std::istream &s)
{
    CVolumeDescriptor desc1;

    do
    {
        desc1.read(s);
        VolDescFactory vdf;
        push_back(vdf.create(desc1));
    }
    while (desc1.desc.type != CVolumeDescriptor::VOLUME_DESCRIPTOR_SET_TERMINATOR);

    return 0;
}

int Directories::read(std::istream &s, Descriptors &d)
{
    CDirectory dir1;
    s.ignore(d[0]->desc.lbaLSB * 2048 - s.tellg());

    while (true)
    {
        dir1.read(s);
        
        if (dir1.dir().length > 0)
        {
            push_back(dir1);

            if (s.tellg() % 2 > 0)
                s.ignore(1);

        }
        else
        {
            break;
        }
    }

    return 0;
}

int ISO::read(std::istream &s)
{
    s.ignore(32768, 0x20);
    descriptors.read(s);
    directories.read(s, descriptors);
    return 0;
}

std::string CDirectory::toString()
{
    std::ostringstream ss;
    ss << "[DIRECTORY]" << std::endl;
    ss << "Length:              " << (int)_dir.length << std::endl;
    ss << "Extended Length:     " << (int)_dir.extendedLength << std::endl;
    ss << "LBA:                 " << (int)_dir.lbaLE << std::endl;
    ss << "Filesize:            " << (int)_dir.dataLengthLE << std::endl;
    ss << "Year:                " << (int)_dir.year + 1900 << std::endl;
    ss << "Month:               " << (int)_dir.month << std::endl;
    ss << "Day:                 " << (int)_dir.day << std::endl;
    ss << "Hour:                " << (int)_dir.hour << std::endl;
    ss << "Minute:              " << (int)_dir.min << std::endl;
    ss << "Second:              " << (int)_dir.sec << std::endl;
    ss << "Timezone:            " << (int)_dir.timezone << std::endl;
    Flags flags(_dir.flags);
    ss << "Flags:               " << flags.toString() << std::endl;
    ss << "Volume Seq Number:   " << (int)_dir.volSeqNumLE << std::endl;
    ss << "Filename length:     " << (int)_dir.fnLength << std::endl;
    ss << "Name:                " << _fn;
    return ss.str();
}

std::string CVolumeDescriptor::toString()
{
    std::ostringstream ss;
    ss << "[Volume Descriptor]" << std::endl;
    ss << "Type:                " << typeString() << std::endl;
    ss << "Identifier:          " << Util::foo(desc.identifier, 5) << std::endl;
    ss << "Version:             " << (int)desc.version << std::endl;
    return ss.str();
}
std::string CPrimaryVolumeDesc::toString()
{
    std::ostringstream ss;
    ss << "[Primary Volume Descriptor]" << std::endl;
    ss << "Type:                " << typeString() << std::endl;
    ss << "Identifier:          " << Util::foo(desc.identifier, 5) << std::endl;
    ss << "Version:             " << (int)desc.version << std::endl;
    ss << "System Identifier:   " << Util::foo(desc.sysident, 32) << std::endl;
    ss << "Volume Identifier:   " << Util::foo(desc.volident, 32) << std::endl;
    ss << "Volume Space Size:   " << desc.volumeSpaceSizeLSB << std::endl;
    ss << "Volume Set Size:     " << (int)desc.volumeSetSizeLSB << std::endl;
    ss << "BlockSize:           " << desc.logicalBlockSizeLSB << std::endl;
    ss << "Path Table Size:     " << desc.pathTableSizeLSB << std::endl;
    ss << "Volume Set Ident:    " << Util::foo(desc.volSetIdent, 128 - 60) << std::endl;
    ss << "Publisher Ident:     " << Util::foo(desc.pubIdent, 128 - 60) << std::endl;
    ss << "Data Prep. Ident:    " << Util::foo(desc.prepIdent, 128 - 60) << std::endl;

    ss << "Directory Length:    " << (int)desc.directoryLength << std::endl;
    ss << "LBA:                 " << (int)desc.lbaLSB << std::endl;
    ss << "Data length:         " << (int)desc.dataLengthLSB << std::endl;
    ss << "Year:                " << (int)desc.year + 1900 << std::endl;
    ss << "Month:               " << (int)desc.month << std::endl;
    ss << "Day:                 " << (int)desc.day << std::endl;
    ss << "Hour:                " << (int)desc.hour << std::endl;
    ss << "Minute:              " << (int)desc.minute << std::endl;
    ss << "Second:              " << (int)desc.second << std::endl;
    ss << "Timezone:            " << (int)desc.timezone;
    return ss.str();
}

std::string CVolumeDescriptorSetTerminator::toString()
{
    std::ostringstream ss;
    ss << "[Volume Descriptor Set Terminator]" << std::endl;
    ss << "Type:                " << typeString() << std::endl;
    ss << "Identifier:          " << Util::foo(desc.identifier, 5) << std::endl;
    ss << "Version:             " << (int)desc.version << std::endl;
    return ss.str();
}

int CVolumeDescriptor::read(std::istream &s)
{
    s.read((char *)&desc, sizeof(desc));
    return 0;
}

int Options::parse(int argc, char **argv)
{
    if (argc <= 1)
    {
        _info = true;
        return 0;
    }

    for (int i = 1; i < argc; i++)
    {
        char *opt = argv[i];

        if (opt[0] == '-')
        {
            switch (opt[1])
            {
            case 'l':
                _list = true;
                break;
            case 'i':
                _info = true;
                break;
            case 'x':
                _extract = true;
                break;
            }
        }
        else
        {
            _fn = std::string(opt);
        }
    }

    return 0;
}

int App::run(int argc, char **argv)
{
    options.parse(argc, argv);
    ISO iso;
    std::ifstream fs;

    if (options.stdinput())
    {
        iso.read(std::cin);
        
    }
    else
    {
        fs.open(options.fn().c_str(), std::fstream::in);
        iso.read(fs);
        
    }

    if (options.list())
        std::cout << iso.list() << std::endl;

    if (options.extract())
    {
        if (options.stdinput())
            iso.extract(std::cin);
        else
            iso.extract(fs);
    }
        
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


