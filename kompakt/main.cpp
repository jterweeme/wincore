#include <iostream>
#include <stdint.h>
#include <sstream>
#include <vector>
#include <iterator>
#include <string.h>
#include <typeinfo>

class Options
{
    bool info;
    bool list;
    bool extract;
public:
    int parse(int argc, char **argv);
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
    uint8_t interleaved;
    uint16_t volSeqNumLE;
    uint16_t volSeqNumBE;
} __attribute__ ((packed));

class CDirectory
{
public:
    SDirectory dir;
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
    static CVolumeDescriptor *create(CVolumeDescriptor &vd);
};

class ISO
{
    std::vector<CVolumeDescriptor> descriptions;
    std::vector<CVolumeDescriptor *> descriptors;
    std::vector<CDirectory> directories;
public:
    int read(std::istream &s);
};

class App
{
    Options options;
public:
    int run(int argc, char **argv);
};

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
    s.read((char *)&dir, sizeof(dir));
    return 0;
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

int ISO::read(std::istream &s)
{
    s.ignore(32768, 0x20);
    CVolumeDescriptor desc1;

    do
    {
        desc1.read(s);
        descriptors.push_back(VolDescFactory::create(desc1));
    }
    while (desc1.desc.type != CVolumeDescriptor::VOLUME_DESCRIPTOR_SET_TERMINATOR);

    for (std::vector<CVolumeDescriptor *>::iterator it = descriptors.begin();
        it != descriptors.end();
        it++)
    {
        std::cout << (*it)->toString() << std::endl << std::endl;
    }

    CDirectory dir1;
    s.ignore(descriptors[0]->desc.lbaLSB *2048 - s.tellg());
    dir1.read(s);
    std::cout << dir1.toString() << std::endl;
    return 0;
}

std::string CDirectory::toString()
{
    std::ostringstream ss;
    ss << "Length:              " << (int)dir.length << std::endl;
    ss << "Extended Length:     " << (int)dir.extendedLength << std::endl;
    ss << "LBA:                 " << (int)dir.lbaLE << std::endl;
    ss << "Data length:         " << (int)dir.dataLengthLE << std::endl;
    ss << "Year:                " << (int)dir.year + 1900 << std::endl;
    ss << "Month:               " << (int)dir.month << std::endl;
    ss << "Day:                 " << (int)dir.day << std::endl;
    ss << "Hour:                " << (int)dir.hour << std::endl;
    ss << "Minute:              " << (int)dir.min << std::endl;
    ss << "Second:              " << (int)dir.sec << std::endl;
    ss << "Timezone:            " << (int)dir.timezone << std::endl;
    ss << "Volume Seq Number:   " << (int)dir.volSeqNumLE;
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
        info = true;
        return 0;
    }

    for (int i = 0; i < argc; i++)
    {
        char *opt = argv[i];

        if (opt[0] == '-')
        {
            switch (opt[1])
            {
            case 'l':
                list = true;
                break;
            case 'i':
                info = true;
                break;
            }
        }
    }
}

int App::run(int argc, char **argv)
{
    options.parse(argc, argv);
    ISO iso;
    iso.read(std::cin);
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


