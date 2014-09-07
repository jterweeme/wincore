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
} __attribute__ ((packed));

class Group
{
    GroupFile gf;
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
    oss << std::string(gf.magic, gf.magic + 4);
    return oss.str();
}

int Group::read(std::istream &s)
{
    s.read((char *)&gf, sizeof(gf));
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


