#include "main.h"
#include <sstream>
#include <iterator>
#include <string.h>
#include <typeinfo>
#include <fstream>
#include <iomanip>

std::string CDirectory::fn()
{
    if (_dir.fnLength == 1)
    {
        return std::string("..");
    }

    return _fn;
}

std::string App::help()
{
    std::ostringstream oss;
    oss << "kompakt: Usage: kompakt [options]" << std::endl;
    oss << "Options:" << std::endl;
    oss << "    -h  Print this help" << std::endl;
    oss << "    -x  Extract";
    return oss.str();
}

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

std::string Directories::list(int mode)
{
    std::ostringstream oss;

    for (iterator it = begin(); it != end(); it++)
    {
        if (mode == 2)
            oss << it->toString() << std::endl;
        else
            oss << it->fn() << std::string(20 - it->fn().length(), ' ')
                << std::right << std::setw(10) << it->dir().dataLengthLE << std::endl;
    }

    return oss.str();
}

bool CDirectory::isDir()
{
    Flags flags(_dir.flags);
    return flags.test(1);
}

std::string ISO::list(int mode)
{
    return directories.list(mode);
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

    for (iterator it = begin() + 2; it != end(); it++)
    {
        if (!it->isDir())
            continue;

        s.ignore(it->dir().lbaLE * 2048 - s.tellg());
        
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
            case 'h':
                _help = true;
                return -1;
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

    if (options.help())
        return 0;

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
    {
        if (options.info())
            std::cout << iso.list(2) << std::endl;
        else
            std::cout << iso.list() << std::endl;
    }

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


