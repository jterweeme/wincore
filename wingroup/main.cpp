#include <stdint.h>
#include <fstream>
#include <iostream>
#include <sstream>

class Options
{
public:
};

struct GroupFile
{
    uint8_t magic[4];
    uint8_t unknown[18];
    uint8_t stringPointer;
} __attribute__ ((packed));

class Group
{
    GroupFile gf;
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

std::string Group::toString()
{
    std::ostringstream oss;
    oss << "Magic:          " << std::string(gf.magic, gf.magic + 4) << std::endl;
    oss << "Description:    " << description << std::endl;
    return oss.str();
}

int Group::read(std::istream &s)
{
    s.read((char *)&gf, sizeof(gf));
    s.ignore(gf.stringPointer - s.tellg());
    char foo[255] = {0};

    for (size_t i = 0; i < sizeof(foo); i++)
    {
        s.read(&foo[i], 1);
        
        if (foo[i] == 0)
            break;
    }

    description = std::string(foo);

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


