#if 1
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
#else
#include "common.h"
#endif

class Options
{
public:
};

struct GroupFile
{
    uint8_t magic[4];
    uint8_t unknown[18];
    uint16_t stringPointer;
    uint16_t iconWidth;
    uint16_t iconHeight;
    uint16_t iconFormat;
    uint16_t reserved;
    uint16_t itemCount;
} __attribute__ ((packed));

class CItem
{
    string _name;
    string _command;
    uint16_t _command2;
public:
    CItem(string name, string command) : _name(name), _command(command) { }
    void command2(uint16_t n) { _command2 = n; }
    string name() const { return _name; }
    string command() const { return _command; }
    string toString();
};

class Items : public vector<int>
{
public:
    string toString();
};

class CItems : public vector<CItem>
{
public:
    string toString();
};

class Group
{
    GroupFile gf;
    Items items;                // offsets
    CItems cItems;
    string _description;
public:
    int read(istream &s);
    string toString();
};

class App
{
    Group group;
public:
    int run(int argc, char **argv);
};

string Items::toString()
{
    ostringstream oss;
    
    for (iterator it = begin(); it != end(); it++)
        oss << *it << "\n";

    return oss.str();
}

string CItem::toString()
{
    ostringstream oss;
    oss << "[Item]\n";
    oss << "Command:        " << _command2 << "\n";
    oss << "Name:           " << name() << "\n";
    oss << "Command:        " << command() << "\n";
    return oss.str();
}

string CItems::toString()
{
    ostringstream oss;
    
    for (iterator it = begin(); it != end(); it++)
        oss << it->toString() << endl;

    return oss.str();
}

string Group::toString()
{
    ostringstream oss;
    oss << "[" << _description << "]" << endl;
    oss << "Magic:          " << string(gf.magic, gf.magic + 4) << endl;
    oss << "StringPointer   " << gf.stringPointer << "\n";
    oss << "Description:    " << _description << endl;
    oss << "Icon size:      " << gf.iconWidth << "x" << gf.iconHeight << endl;
    oss << "Icon format:    " << gf.iconFormat << endl;
    oss << "Items:          " << gf.itemCount << endl << endl;
    oss << cItems.toString() << endl;
    return oss.str();
}

int Group::read(istream &s)
{
    s.read((char *)&gf, sizeof(gf));
    
    for (int i = 0, foo = 0; i < gf.itemCount; i++)
    {
        s.read((char *)&foo, 2);
        items.push_back(foo);
    }

    sort(items.begin(), items.end());
    s.ignore(gf.stringPointer - s.tellg());
    char desc[255] = {0};
    char cmd[255] = {0};
    char path[255] = {0};
    char name[255] = {0};
    for (int i = 0, c = 0; (c = s.get()) != 0; i++) desc[i] = c;
    _description = string(desc);
    uint16_t cmdPtr;
    uint16_t namePtr;
    uint16_t barPtr;
    s.ignore(18);
    s.read((char *)&cmdPtr, 2);
    s.read((char *)&namePtr, 2);
    s.read((char *)&barPtr, 2);
    for (int i = 0, c = 0; (c = s.get()) != 0; i++) name[i] = c;
    cout << name << "\n";
    s.ignore(namePtr - s.tellg());
    for (int i = 0, c = 0; (c = s.get()) != 0; i++) cmd[i] = c;
    cout << cmd << "\n";
    s.ignore(cmdPtr - s.tellg());
    for (int i = 0, c = 0; (c = s.get()) != 0; i++) path[i] = c;
    cout << path << "\n";
    CItem item1c(cmd, path);
    item1c.command2(cmdPtr);
    //cItems.push_back(item1c);
    return 0;
}

int App::run(int argc, char **argv)
{
    group.read(cin);
    cout << group.toString() << "\n";
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


