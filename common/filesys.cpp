#include <unistd.h>
#include <sys/stat.h>
#include "filesys.h"

void FileSystem::chdirx(const char *path)
{
    if (chdir(path) != 0)
        throw "Cannot change directory";

    _pwd.push_back(path);
}

void FileSystem::mkdir(const char *name)
{
    if (::mkdir(name, 500) != 0)
        throw "Cannot create directory";
}

void FileSystem::chmkdir(FSPath &path)
{
    goRoot();

    for (FSPath::iterator it = path.begin(); it != path.end(); it++)
        chmkdir(it->c_str());

}

void FileSystem::up()
{
    if (_root.size() >= _pwd.size())
        return;

    if (::chdir("..") != 0)
        throw "Cannot go up directory";

    _pwd.pop_back();
}

void FileSystem::chmkdir(const char *path)
{
    try
    {
        chdir(path);
        return;
    }
    catch (...)
    {
    }
    
    mkdir(path);
    chdir(path);
}

FSPath FileSystem::pwdir()
{
    FSPath pwd;
    char path[255] = {0};
    getcwd(path, sizeof(path));

    for (char *token = strtok(path, "/"); token != NULL; )
    {
        pwd.push_back(token);
        token = strtok(NULL, "/");
    }

    return pwd;
}

void FSPath::dump(ostream &os)
{
    for (iterator it = begin(); it != end(); it++)
        os << "/" << *it;
}

void FileSystem::rmdir(const char *path)
{
    if (::rmdir(path) != 0)
        throw "Cannot remove directory";
}


