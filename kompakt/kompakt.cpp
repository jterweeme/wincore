#include "kompakt.h"
#include "filesys.h"

#ifndef DEBUG
#define DEBUG2
#endif

string DirEntry::fn()
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

int ISO::extract(istream &s, const char *outputDir)
{
    FileSystem fs;
    fs.chmkdir(outputDir);
    extract(s);
    fs.goRoot();
    return 0;
}

int ISO::extract(istream &s)
{

    for (Directories::iterator directory = _directories.begin();
            directory != _directories.end();
            directory++)
    {
        for (Directory::iterator file = directory->begin() + 2;
                file != directory->end();
                file++)
        {
            _fileList.push_back(*file);
        }
    }
 
    _fileList.snort();
    FileSystem fs;

    for (Directory::iterator it = _fileList.begin(); it!= _fileList.end(); it++)
    {
        if (it->isDir())
            continue;

        FSPath path = _pathTable.recurse(it->parentLBA());
        path.dump(cout);
        cout << "/" << it->fn() << "\n";
        fs.chmkdir(path);
        ofstream of;
        of.open(it->fn().c_str());
        s.ignore(it->dir().lbaLE * 2048 - s.tellg());
        uint32_t length = it->dir().dataLengthLE;
        char buf[2048] = {0};
        
        while (length >= sizeof(buf))
        {
            s.read(buf, sizeof(buf));
            of.write(buf, sizeof(buf));
            length -= sizeof(buf);
        }

        memset(buf, 0, sizeof(buf));
        s.read(buf, length);
        of.write(buf, length);
        of.close();
    }

    return 0;
}

void Directories::read(istream &is, PathTable &pt)
{
    for (PathTable::iterator it = pt.begin(); it != pt.end(); it++)
    {
        Directory dir;
        dir.read(is, it->offsetLBA());
        push_back(dir);
    }
}

void Directories::list(ostream &os, int mode)
{
    for (iterator it = begin(); it != end(); it++)
        it->list(os, mode);
}

void Directory::list(ostream &os, int mode)
{
    for (iterator it = begin(); it != end(); it++)
    {
        if (mode == 2)
        {
            it->dump(os);
            os << "\n\n";
        }
        else
        {
            os << it->fn() << string(20 - it->fn().length(), ' ')
                << right << setw(9) << it->dir().dataLengthLE << setw(7)
                << it->dir().lbaLE << setw(6) << it->parentLBA() << "\n";
        }
    }

    os << "\n";
}

void PathEntry::read(istream &is)
{
    is.read((char *)&_pe, sizeof(_pe));
    is.read(_name, _pe.length);
    //fprintf(stderr, "%lu\n", is.tellg() % 2);
    is.ignore(is.tellg() % 2);
}

void PathTable::snort()
{
    make_heap(begin(), end(), wayToSort);
    sort_heap(begin(), end(), wayToSort);
}

PathEntry &PathTable::getByLBA(uint32_t lba)
{
    for (iterator it = begin(); it != end(); it++)
        if (it->offsetLBA() == lba)
            return *it;

    throw "onzin";
}

PathEntry &PathTable::getByIndex(int i)
{
    for (iterator it = begin(); it != end(); it++)
        if (it->index() == i)
            return *it;

    throw "onzin";

}

void PathTable::read(istream &is, uint32_t lba, size_t n)
{
    _offsetLBA = lba;
    _size = n;
    //fprintf(stderr, "%lu\n", lba * 2048 - is.tellg());
    is.ignore(lba * 2048 - is.tellg());

    for (int i = 1; is.tellg() < lba * 2048 + n; i++)
    {
        PathEntry pe;
        pe.read(is);
        pe.index(i);
#ifdef DEBUG
        pe.dump(cerr);
        cerr << "\n";
#endif
        push_back(pe);
    }
}

CPrimaryVolumeDesc::CPrimaryVolumeDesc(const CVolumeDescriptor &vd)
{
    memcpy(&_desc, &vd._desc, sizeof(_desc));
}

CVolumeDescriptorSetTerminator::CVolumeDescriptorSetTerminator(const CVolumeDescriptor &vd)
{
    memcpy(&_desc, &vd._desc, sizeof(_desc));
}

int DirEntry::read(istream &s)
{
    _offset = s.tellg();
    uint8_t lengte = s.get();
    length(lengte);
    s.read((char *)&_dir + 1, sizeof(_dir) -1);
    char filename[255] = {0};
    s.read(filename, _dir.fnLength);

    if (filename[_dir.fnLength - 2] == ';')
        filename[_dir.fnLength - 2] = '\0';

    _fn = string(filename);
    s.ignore(_offset + _dir.length - s.tellg());
    return 0;
}

const char *CVolumeDescriptor::typeString()
{
    switch (_desc.type)
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
    switch (vd._desc.type)
    {
    case CPrimaryVolumeDesc::PRIMARY_VOLUME_DESCRIPTOR:
    {
        CPrimaryVolumeDesc *pvd = new CPrimaryVolumeDesc(vd);
        pvd->test();
        return pvd;
    }
    case CVolumeDescriptor::VOLUME_DESCRIPTOR_SET_TERMINATOR:
        return new CVolumeDescriptorSetTerminator(vd);
    default:
        return new CVolumeDescriptor(vd);
    }
}

void Descriptors::dump(ostream &os)
{
    for (iterator it = begin(); it != end(); it++)
    {
        (*it)->dump(os);
        os << "\n\n";
    }
}

uint32_t Descriptors::pathTableOffset()
{
    iterator it = begin();
    CPrimaryVolumeDesc *cpv = dynamic_cast<CPrimaryVolumeDesc*>(*it);
    return cpv->pathTableOffset();
}

size_t Descriptors::pathTableSize()
{
    iterator it = begin();
    CPrimaryVolumeDesc *cpv = dynamic_cast<CPrimaryVolumeDesc*>(*it);
    return cpv->pathTableSize();
}

int Descriptors::read(istream &s)
{
    CVolumeDescriptor desc1;

    do
    {
        desc1.erase();
        desc1.read(s);
        VolDescFactory vdf;
        push_back(vdf.create(desc1));
    }
    while (desc1._desc.type != CVolumeDescriptor::VOLUME_DESCRIPTOR_SET_TERMINATOR);

    return 0;
}

void Directory::read(istream &s, uint32_t offset)
{
    s.ignore(offset * 2048 - s.tellg());

    while (true)
    {
        if (s.peek() != 0)
        {
            DirEntry dir1;
            dir1.read(s);
            dir1.parentLBA(offset);
            push_back(dir1);
            s.ignore(s.tellg() % 2);
        }
        else if (s.tellg() % 2048 > 1900)
        {
            size_t nextLBA = s.tellg() / 2048 + 1;
            s.ignore(nextLBA * 2048 - s.tellg());

            if (s.peek() == 0)
                break;
        }
        else
        {
            break;
        }
    }
}

int ISO::read(istream &s)
{
    s.ignore(32768);
    _descriptors.read(s);
    _pathTable.read(s, _descriptors.pathTableOffset(), _descriptors.pathTableSize());
    _pathTable.snort();
    _directories.read(s, _pathTable);
    return 0;
}

void PathEntry::dump(ostream &os)
{
    os << dec << setw(4) << _index << ":" << setw(5) << (int)_pe.parent << hex << " "
       << (int)_pe.lba << " " << _name;
}

void DirEntry::dump(ostream &os)
{
    os << "[FILE]\n";
    os << "Length:              " << (int)_dir.length << "\n";
    os << "Extended Length:     " << (int)_dir.extendedLength << "\n";
    os << "LBA:                 " << (int)_dir.lbaLE << "\n";
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
    os << "Identifier:          " << string(_desc.identifier, 5) << "\n";
    os << "Version:             " << (int)_desc.version << "\n";
}

void CPrimaryVolumeDesc::test()
{
    if (_desc.fsv != 1)
        throw "error";

    if (_desc.unused != 0)
        throw "error";
}

void CPrimaryVolumeDesc::dump(ostream &os)
{
    os << "[Primary Volume Descriptor]\n";
    os << "Type:                " << typeString() << "\n";
    os << "Identifier:          " << string(_desc.identifier, 5) << "\n";
    os << "Version:             " << (int)_desc.version << "\n";
    os << "System Identifier:   " << string(_desc.sysident, 32) << "\n";
    os << "Volume Identifier:   " << string(_desc.volident, 32) << "\n";
    os << "Volume Space Size:   " << _desc.volumeSpaceSizeLSB << "\n";
    os << "Volume Set Size:     " << (int)_desc.volumeSetSizeLSB << "\n";
    os << "BlockSize:           " << _desc.logicalBlockSizeLSB << "\n";
    os << "Path Table Size:     " << _desc.pathTableSizeLSB << "\n";
    os << "L-Path:              " << _desc.pathTable << "\n";
    os << "M-Path:              " << _desc.pathMTable << "\n";
    os << "Volume Set Ident:    " << string(_desc.volSetIdent, 128 - 60) << "\n";
    os << "Publisher Ident:     " << string(_desc.pubIdent, 128 - 60) << "\n";
    os << "Data Prep. Ident:    " << string(_desc.prepIdent, 128 - 60) << "\n";
    os << "Application Ident:   " << string(_desc.appIdent, 128 - 60) << "\n";
    os << "Copyright Ident:     " << string(_desc.copyrightIdent, 38) << "\n";
    os << "Abstract File Ident: " << string(_desc.abstractIdent, 36) << "\n";
    os << "Bibliographic Ident: " << string(_desc.bibIdent, 37) << "\n";
    os << "Creation Time:       " << string(_desc.creationTime, 17) << "\n";
    os << "Modification Time:   " << string(_desc.modTime, 17) << "\n";
    os << "Expiration Time:     " << string(_desc.expTime, 17) << "\n";
    os << "Effective Time:      " << string(_desc.effectTime, 17) << "\n";
    os << "File Struct Version: " << (int)_desc.fsv << "\n";
    os << "Unused:              " << (int)_desc.unused << "\n";
    os << "Directory Length:    " << (int)_desc.directoryLength << "\n";
    os << "LBA:                 " << (int)_desc.lbaLSB << "\n";
    os << "Data length:         " << (int)_desc.dataLengthLSB << "\n";
    os << "Year:                " << (int)_desc.year + 1900 << "\n";
    os << "Month:               " << (int)_desc.month << "\n";
    os << "Day:                 " << (int)_desc.day << "\n";
    os << "Hour:                " << (int)_desc.hour << "\n";
    os << "Minute:              " << (int)_desc.minute << "\n";
    os << "Second:              " << (int)_desc.second << "\n";
    os << "Timezone:            " << (int)_desc.timezone;   
}

bool PathTable::wayToSort(const PathEntry &p1, const PathEntry &p2)
{
    return p1.offsetLBA() < p2.offsetLBA();
}

void PathTable::dump(ostream &os)
{
    os << "Path table starts at block " << _offsetLBA << ", size " << _size << "\n";

    for (iterator it = begin(); it != end(); it++)
    {
        it->dump(os);
        os << "\n";
    }
}

void CVolumeDescriptorSetTerminator::dump(ostream &os)
{
    os << "[Volume Descriptor Set Terminator]\n";
    os << "Type:                " << typeString() << endl;
    os << "Identifier:          " << string(_desc.identifier, 5) << endl;
    os << "Version:             " << (int)_desc.version << endl;
}

int Options::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
            case 'h':
                _help = true;
                return -1;
            case 'l':
                _list = true;
                break;
            case 'f':
                _file = true;
                _fn = string(argv[++i]);
                break;
            case 'p':
                _pathTable = true;
                break;
            case 'v':
                _verbose = true;
                break;
            case 's':
                _cin = true;
                break;
            case 'd':
                _desc = true;
                break;
            case 'x':
                _extract = true;
                break;
            case 'o':
                _outputDir = true;
                _outputDirName = string(argv[++i]);
                break;
            }
        }
    }

    return 0;
}

int App::run(int argc, char **argv)
{
    _options.parse(argc, argv);

    if (_options.help())
    {
        help(cout);
        cout << "\n";
        return 0;
    }

    istream *is;

    if (_options.stdinput())
    {
        is = &cin;
    }
    else if (_options.file())
    {
        _ifs.open(_options.fn().c_str(), fstream::in);

        if (!_ifs.is_open())
            throw "File not open";

        is = &_ifs;
    }
    else
    {
        help(cout);
        cout << "\n";
        return 0;
    }

    _iso.read(*is);

    if (_options.desc())
    {
        _iso.dumpDescriptors(cout);
        cout << "\n";
    }

    if (_options.pathTable())
    {
        _iso.dumpPathTable(cout);
        cout << "\n";
    }

    if (_options.list())
    {
        if (_options.verbose())
            _iso.list(cout, 2);
        else
            _iso.list(cout);

        cout << "\n";
    }

    if (_options.extract())
    {
        if (_options.outputDir())
            _iso.extract(*is, _options.outputDirName().c_str());
        else
            _iso.extract(*is);
    }
        
    return 0;
}




