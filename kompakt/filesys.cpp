#include "filesys.h"
#include <sys/stat.h>

void FileSystem::mkdir(const char *name)
{
    if (::mkdir(name, 112) != 0)
        throw "Cannot create directory";
}

void FSPath::dump(ostream &os)
{
    for (iterator it = begin(); it != end(); it++)
        os << "/" << *it;
}



