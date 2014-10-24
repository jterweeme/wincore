#ifndef _FILESYS_H_
#define _FILESYS_H_
#include "common.h"
#include <unistd.h>

class FSPath : public vector<string>
{
public:
    void dump(ostream &os);
};

class FileSystem
{
    FSPath _root;
    FSPath _pwd;
public:
    void mkdir(const char *name);

    int chdir(const char *path)
    {
        return ::chdir(path);
    }

    int chmkdir(const char *path);

    void chmkdir(uint32_t lba, FSPath path)
    {
        for (FSPath::iterator it = path.begin(); it != path.end(); it++)
        {
            int x = chdir(it->c_str());

        }
    }

    FSPath pwdir()
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
    FileSystem() : _root(pwdir()), _pwd(pwdir()) { }
    FSPath root() const { return _root; }
};

#endif


