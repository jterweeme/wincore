#include <stdint.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

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

struct Item
{
    uint8_t unknown[17];
    uint16_t command;
    uint16_t name;
};

class CItem
{
    std::string _name;
    std::string _command;
public:
    CItem(std::string name, std::string command) : _name(name), _command(command) { }
    std::string name() const { return _name; }
    std::string command() const { return _command; }
    std::string toString();
};

class Items : public std::vector<int>
{
public:
    std::string toString();
};

class CItems : public std::vector<CItem>
{
public:
    std::string toString();
};

class Group
{
    GroupFile gf;
    Items items;                // offsets
    CItems cItems;
    std::string description;
public:
    int read(std::istream &s);
    std::string toString();
};

class App
{
    Group group;
public:
    int run(int argc, char **argv);
};

std::string Items::toString()
{
    std::ostringstream oss;
    
    for (iterator it = begin(); it != end(); it++)
        oss << *it << std::endl;

    return oss.str();
}

std::string CItem::toString()
{
    std::ostringstream oss;
    oss << "[Item]" << std::endl;
    oss << "Name:           " << name() << std::endl;
    oss << "Command:        " << command() << std::endl;
    return oss.str();
}

std::string CItems::toString()
{
    std::ostringstream oss;
    
    for (iterator it = begin(); it != end(); it++)
        oss << it->toString() << std::endl;

    return oss.str();
}

std::string Group::toString()
{
    std::ostringstream oss;
    oss << "[" << description << "]" << std::endl;
    oss << "Magic:          " << std::string(gf.magic, gf.magic + 4) << std::endl;
    oss << "Description:    " << description << std::endl;
    oss << "Icon size:      " << gf.iconWidth << "x" << gf.iconHeight << std::endl;
    oss << "Icon format:    " << gf.iconFormat << std::endl;
    oss << "Items:          " << gf.itemCount << std::endl << std::endl;
    oss << cItems.toString() << std::endl;
    return oss.str();
}

int Group::read(std::istream &s)
{
    s.read((char *)&gf, sizeof(gf));
    
    for (int i = 0, foo = 0; i < gf.itemCount; i++)
    {
        s.read((char *)&foo, 2);
        items.push_back(foo);
    }

    std::sort(items.begin(), items.end());

    s.ignore(gf.stringPointer - s.tellg());
    char foo[255] = {0};

    for (size_t i = 0; i < sizeof(foo); i++)
    {
        s.read(&foo[i], 1);
        
        if (foo[i] == 0)
            break;
    }

    description = std::string(foo);

    Item item1;
    char item1name[255] = {0};
    char item1cmd[255] = {0};
    s.ignore(items[0] - s.tellg());
    s.read((char *)&item1, sizeof(item1));
    s.ignore(item1.name - s.tellg());
    
    for (size_t i = 0; i < sizeof(item1name); i++)
    {
        s.read(&item1name[i], 1);

        if (item1name[i] == 0)
            break;
    }

    s.ignore(item1.command - s.tellg());
    
    for (size_t i = 0; i < sizeof(item1name); i++)
    {
        s.read(&item1cmd[i], 1);

        if (item1cmd[i] == 0)
            break;
    }

    CItem item1c(item1name, item1cmd);
    cItems.push_back(item1c);
    //std::cout << item1name << std::endl;
    return 0;
}

int App::run(int argc, char **argv)
{
    group.read(std::cin);
    std::cout << group.toString() << std::endl;
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


