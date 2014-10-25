#ifndef _FILESYS_H_
#define _FILESYS_H_
#include "common.h"

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
    void chdir(const char *path);
    void chmkdir(const char *path);
    void chmkdir(FSPath &path);
    void rmdir(const char *path);
    void up();
    void up(int n) { while (n--) up(); }
    void goRoot() { up(_pwd.size() - _root.size()); }
    void unlink(const char *pathname);
    void destroy(const char *pathname);
    FSPath pwdir();
    FileSystem() : _root(pwdir()), _pwd(pwdir()) { }
    FSPath root() const { return _root; }
};

#endif


