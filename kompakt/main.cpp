#include <iostream>
#include <stdint.h>
#include <sstream>
#include <vector>

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
    uint32_t extentLE;
    uint32_t extentBE;
    uint32_t dataLengthLE;
    uint32_t dataLengthBE;
    uint8_t year;
    uint8_t month;
    uint8_t day;
} __attribute__ ((packed));

class CDirectory
{
    SDirectory dir;
public:
    int read(std::istream &s);
    std::string toString();
};

struct Test
{
    uint8_t type;
    char identifier[5];
    uint8_t version;
    union
    {
        struct
        {
            //uint8_t dinges1;
            //uint8_t dinges2;
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
    uint8_t directoryEntry[34];
    char volSetIdent[128];
    char pubIdent[128];
    char prepIdent[128];
    uint8_t data[1474];
        } __attribute__ ((packed));
        //uint8_t onzin1;
        //uint16_t onzin2;
    };
} __attribute__ ((packed));

struct SVolumeDescriptor
{
    uint8_t type;
    char identifier[5];
    uint8_t version;
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
    uint8_t directoryEntry[34];
    char volSetIdent[128];
    char pubIdent[128];
    char prepIdent[128];
    uint8_t data[1474];
} __attribute__ ((packed));

class CVolumeDescriptor
{
    const char *typeString();
public:
    SVolumeDescriptor desc;
//public:
    int read(std::istream &s);
    std::string toString();
};

class ISO
{
};

class App
{
    Options options;
public:
    int run(int argc, char **argv);
};

int CDirectory::read(std::istream &s)
{
    s.read((char *)&dir, sizeof(dir));
    return 0;
}

const char *CVolumeDescriptor::typeString()
{
    switch (desc.type)
    {
    case 1:
        return "Primary Volume Descriptor";
    case 2:
        return "Supplementary Volume Descriptor";
    case 255:
        return "Volume Descriptor Set Terminator";
    }
}

std::string CDirectory::toString()
{
    std::ostringstream ss;
    ss << "Length: " << (int)dir.length << std::endl;
    ss << "Year: " << (int)dir.year << std::endl;
    return ss.str();
}

std::string CVolumeDescriptor::toString()
{
    std::ostringstream ss;
    ss << "Type:               " << typeString() << std::endl;
    ss << "Identifier:         " << Util::foo(desc.identifier, 5) << std::endl;
    ss << "Version:            " << (int)desc.version << std::endl;
    ss << "System Identifier:  " << Util::foo(desc.sysident, 32) << std::endl;
    ss << "Volume Identifier:  " << Util::foo(desc.volident, 32) << std::endl;
    ss << "BlockSize:          " << desc.logicalBlockSizeLSB << std::endl;
    ss << "Volume Space Size:  " << desc.volumeSpaceSizeLSB << std::endl;
    ss << "Volume Set Size:    " << (int)desc.volumeSetSizeLSB << std::endl;
    ss << "Volume Set Ident:   " << Util::foo(desc.volSetIdent, 128) << std::endl;
    ss << "Publisher Ident:    " << Util::foo(desc.pubIdent, 128) << std::endl;
    ss << "Data Prep. Ident:   " << Util::foo(desc.prepIdent, 128);
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
    std::cin.ignore(32768, 0x20);
    std::vector<CVolumeDescriptor> descriptions;
    CVolumeDescriptor desc1, desc2, desc3;

    do
    {
        desc1.read(std::cin);
        std::cout << desc1.toString() << std::endl << std::endl;
    }
    while (desc1.desc.type != 0xff);

    CDirectory dir1;
    dir1.read(std::cin);
    std::cout << dir1.toString() << std::endl;
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


