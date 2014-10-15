#include "kompakt.h"

string CDirectory::fn()
{
    if (_dir.fnLength == 1)
    {
        return string("..");
    }

    return _fn;
}

string App::help()
{
    ostringstream oss;
    oss << "kompakt: Usage: kompakt [options]\n";
    oss << "Options:\n";
    oss << "    -h  Print this help\n";
    oss << "    -x  Extract";
    return oss.str();
}

string Flags::toString()
{
    ostringstream ss;

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

int ISO::extract(istream &s)
{
    for (Directories::iterator it = _directories.begin() + 2; it != _directories.end(); it++)
    {
        ofstream of;
        of.open(it->fn().c_str());
        s.ignore(it->dir().lbaLE * 2048 - s.tellg());
        char *buf = new char[it->dir().dataLengthLE];
        s.read(buf, it->dir().dataLengthLE);
        of.write(buf, it->dir().dataLengthLE);
        of.close();
        delete[] buf;
    }
    return 0;
}

string Directories::list(int mode)
{
    ostringstream oss;

    for (iterator it = begin(); it != end(); it++)
    {
        if (mode == 2)
            oss << it->toString() << endl;
        else
            oss << it->fn() << string(20 - it->fn().length(), ' ')
                << right << setw(10) << it->dir().dataLengthLE << endl;
    }

    return oss.str();
}

bool CDirectory::isDir()
{
    Flags flags(_dir.flags);
    return flags.test(1);
}

string ISO::list(int mode)
{
    return _directories.list(mode);
}

CPrimaryVolumeDesc::CPrimaryVolumeDesc(const CVolumeDescriptor &vd)
{
    memcpy(&desc, &vd.desc, sizeof(desc));
}

CVolumeDescriptorSetTerminator::CVolumeDescriptorSetTerminator(const CVolumeDescriptor &vd)
{
    memcpy(&desc, &vd.desc, sizeof(desc));
}

int CDirectory::read(istream &s)
{
    s.read((char *)&_dir, sizeof(_dir));
    char filename[255] = {0};
    s.read(filename, _dir.fnLength);
    _fn = string(filename);
    return 0;
}

string Options::toString()
{
    ostringstream oss;
    oss << "Filename: " << _fn << endl;
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

string Descriptors::toString()
{
    ostringstream oss;
 
    for (iterator it = begin(); it != end(); it++)
        oss << (*it)->toString() << endl << endl;
   
    return oss.str();
}

int Descriptors::read(istream &s)
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

int Directories::read(istream &s, Descriptors &d)
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

int ISO::read(istream &s)
{
    s.ignore(32768, 0x20);
    _descriptors.read(s);
    _directories.read(s, _descriptors);
    return 0;
}

string CDirectory::toString()
{
    ostringstream ss;
    ss << "[DIRECTORY]\n";
    ss << "Length:              " << (int)_dir.length << endl;
    ss << "Extended Length:     " << (int)_dir.extendedLength << endl;
    ss << "LBA:                 " << (int)_dir.lbaLE << endl;
    ss << "Filesize:            " << (int)_dir.dataLengthLE << endl;
    ss << "Year:                " << (int)_dir.year + 1900 << endl;
    ss << "Month:               " << (int)_dir.month << endl;
    ss << "Day:                 " << (int)_dir.day << endl;
    ss << "Hour:                " << (int)_dir.hour << endl;
    ss << "Minute:              " << (int)_dir.min << endl;
    ss << "Second:              " << (int)_dir.sec << endl;
    ss << "Timezone:            " << (int)_dir.timezone << endl;
    Flags flags(_dir.flags);
    ss << "Flags:               " << flags.toString() << endl;
    ss << "Volume Seq Number:   " << (int)_dir.volSeqNumLE << endl;
    ss << "Filename length:     " << (int)_dir.fnLength << endl;
    ss << "Name:                " << _fn;
    return ss.str();
}

string CVolumeDescriptor::toString()
{
    ostringstream ss;
    ss << "[Volume Descriptor]\n";
    ss << "Type:                " << typeString() << endl;
    ss << "Identifier:          " << Util::foo(desc.identifier, 5) << endl;
    ss << "Version:             " << (int)desc.version << endl;
    return ss.str();
}
string CPrimaryVolumeDesc::toString()
{
    ostringstream ss;
    ss << "[Primary Volume Descriptor]\n";
    ss << "Type:                " << typeString() << endl;
    ss << "Identifier:          " << Util::foo(desc.identifier, 5) << endl;
    ss << "Version:             " << (int)desc.version << endl;
    ss << "System Identifier:   " << Util::foo(desc.sysident, 32) << endl;
    ss << "Volume Identifier:   " << Util::foo(desc.volident, 32) << endl;
    ss << "Volume Space Size:   " << desc.volumeSpaceSizeLSB << endl;
    ss << "Volume Set Size:     " << (int)desc.volumeSetSizeLSB << endl;
    ss << "BlockSize:           " << desc.logicalBlockSizeLSB << endl;
    ss << "Path Table Size:     " << desc.pathTableSizeLSB << endl;
    ss << "Volume Set Ident:    " << Util::foo(desc.volSetIdent, 128 - 60) << endl;
    ss << "Publisher Ident:     " << Util::foo(desc.pubIdent, 128 - 60) << endl;
    ss << "Data Prep. Ident:    " << Util::foo(desc.prepIdent, 128 - 60) << endl;
    ss << "Directory Length:    " << (int)desc.directoryLength << endl;
    ss << "LBA:                 " << (int)desc.lbaLSB << endl;
    ss << "Data length:         " << (int)desc.dataLengthLSB << endl;
    ss << "Year:                " << (int)desc.year + 1900 << endl;
    ss << "Month:               " << (int)desc.month << endl;
    ss << "Day:                 " << (int)desc.day << endl;
    ss << "Hour:                " << (int)desc.hour << endl;
    ss << "Minute:              " << (int)desc.minute << endl;
    ss << "Second:              " << (int)desc.second << endl;
    ss << "Timezone:            " << (int)desc.timezone;
    return ss.str();
}

string CVolumeDescriptorSetTerminator::toString()
{
    ostringstream ss;
    ss << "[Volume Descriptor Set Terminator]\n";
    ss << "Type:                " << typeString() << endl;
    ss << "Identifier:          " << Util::foo(desc.identifier, 5) << endl;
    ss << "Version:             " << (int)desc.version << endl;
    return ss.str();
}

int CVolumeDescriptor::read(istream &s)
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
            _fn = string(opt);
        }
    }

    return 0;
}

int App::run(int argc, char **argv)
{
    options.parse(argc, argv);

    if (options.help())
    {
        cout << help() << "\n";
        return 0;
    }

    ISO iso;
    ifstream fs;

    if (options.stdinput())
    {
        iso.read(cin);
    }
    else
    {
        fs.open(options.fn().c_str(), fstream::in);
        iso.read(fs);
    }

    if (options.list())
    {
        if (options.info())
            cout << iso.list(2) << endl;
        else
            cout << iso.list() << endl;
    }

    if (options.extract())
    {
        if (options.stdinput())
            iso.extract(cin);
        else
            iso.extract(fs);
    }
        
    return 0;
}




