#ifndef _FILESYS_H_
#define _FILESYS_H_
#include "common.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

class File
{
public:
    
};

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
    void chdirx(const char *path);
    void chmkdir(const char *path);
    void chmkdir(FSPath &path);
    void rmdir(const char *path);
    void up();
    void up(int n) { while (n--) up(); }
    void goRoot() { up(pwdir().size() - _root.size()); }
    FSPath pwdir();
    FileSystem() : _root(pwdir()), _pwd(pwdir()) { }
    FSPath root() const { return _root; }
    void ls(vector<string> *v);
};

#endif


