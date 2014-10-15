#include "kompakt.h"

string CDirectory::fn()
{
    if (_dir.fnLength == 1)
    {
        return string("..");
    }

    return _fn;
}

void App::help(ostream &os)
{
    os << "kompakt: Usage: kompakt [options]\n";
    os << "Options:\n";
    os << "    -h  Print this help\n";
    os << "    -x  Extract";
}

void Flags::dump(ostream &os)
{
    if (test(0))
        os << "Hidden ";
    if (test(1))
        os << "Dir ";
    if (test(2))
        os << "Associated ";
    if (test(3))
        os << "Ex Attr ";
    if (test(4))
        os << "Permissions ";
    if (test(7))
        os << "Final Dir ";

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

void Directories::list(ostream &os, int mode)
{
    for (iterator it = begin(); it != end(); it++)
    {
        if (mode == 2)
        {
            it->dump(os);
            os << "\n";
        }
        else
        {
            os << it->fn() << string(20 - it->fn().length(), ' ')
                << right << setw(10) << it->dir().dataLengthLE << "\n";
        }
    }
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

void Descriptors::dump(ostream &os)
{
    for (iterator it = begin(); it != end(); it++)
        os << (*it)->toString() << endl << endl;
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

void CDirectory::dump(ostream &os)
{
    os << "[DIRECTORY]\n";
    os << "Length:              " << (int)_dir.length << endl;
    os << "Extended Length:     " << (int)_dir.extendedLength << endl;
    os << "LBA:                 " << (int)_dir.lbaLE << endl;
    os << "Filesize:            " << (int)_dir.dataLengthLE << endl;
    os << "Year:                " << (int)_dir.year + 1900 << endl;
    os << "Month:               " << (int)_dir.month << endl;
    os << "Day:                 " << (int)_dir.day << endl;
    os << "Hour:                " << (int)_dir.hour << endl;
    os << "Minute:              " << (int)_dir.min << endl;
    os << "Second:              " << (int)_dir.sec << endl;
    os << "Timezone:            " << (int)_dir.timezone << endl;
    Flags flags(_dir.flags);
    os << "Flags:               ";
    flags.dump(os);
    os << endl;
    os << "Volume Seq Number:   " << (int)_dir.volSeqNumLE << endl;
    os << "Filename length:     " << (int)_dir.fnLength << endl;
    os << "Name:                " << _fn;
}

void CVolumeDescriptor::dump(ostream &os)
{
    os << "[Volume Descriptor]\n";
    os << "Type:                " << typeString() << endl;
    os << "Identifier:          " << Util::foo(desc.identifier, 5) << endl;
    os << "Version:             " << (int)desc.version << endl;
}

void CPrimaryVolumeDesc::dump(ostream &os)
{
    os << "[Primary Volume Descriptor]\n";
    os << "Type:                " << typeString() << endl;
    os << "Identifier:          " << Util::foo(desc.identifier, 5) << endl;
    os << "Version:             " << (int)desc.version << endl;
    os << "System Identifier:   " << Util::foo(desc.sysident, 32) << endl;
    os << "Volume Identifier:   " << Util::foo(desc.volident, 32) << endl;
    os << "Volume Space Size:   " << desc.volumeSpaceSizeLSB << endl;
    os << "Volume Set Size:     " << (int)desc.volumeSetSizeLSB << endl;
    os << "BlockSize:           " << desc.logicalBlockSizeLSB << endl;
    os << "Path Table Size:     " << desc.pathTableSizeLSB << endl;
    os << "Volume Set Ident:    " << Util::foo(desc.volSetIdent, 128 - 60) << endl;
    os << "Publisher Ident:     " << Util::foo(desc.pubIdent, 128 - 60) << endl;
    os << "Data Prep. Ident:    " << Util::foo(desc.prepIdent, 128 - 60) << endl;
    os << "Directory Length:    " << (int)desc.directoryLength << endl;
    os << "LBA:                 " << (int)desc.lbaLSB << endl;
    os << "Data length:         " << (int)desc.dataLengthLSB << endl;
    os << "Year:                " << (int)desc.year + 1900 << endl;
    os << "Month:               " << (int)desc.month << endl;
    os << "Day:                 " << (int)desc.day << endl;
    os << "Hour:                " << (int)desc.hour << endl;
    os << "Minute:              " << (int)desc.minute << endl;
    os << "Second:              " << (int)desc.second << endl;
    os << "Timezone:            " << (int)desc.timezone;   
}

void CVolumeDescriptorSetTerminator::dump(ostream &os)
{
    os << "[Volume Descriptor Set Terminator]\n";
    os << "Type:                " << typeString() << endl;
    os << "Identifier:          " << Util::foo(desc.identifier, 5) << endl;
    os << "Version:             " << (int)desc.version << endl;
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
        help(cout);
        cout << "\n";
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
            iso.list(cout, 2);
        else
            iso.list(cout);

        cout << "\n";
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




