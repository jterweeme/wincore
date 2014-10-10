#include <stdint.h>
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;

struct Header
{
    uint16_t magic;
    uint8_t method;
    uint8_t flags;
    uint32_t time;
} __attribute__ ((packed));

class App
{
    void read(istream &is);
    Header _header;
    const int _argc;
    const char **_argv;
    void dump(ostream &os);
public:
    App(const int argc, const char **argv) : _argc(argc), _argv(argv) { }
    int run();
};

void App::dump(ostream &os)
{
    time_t foo = (time_t)_header.time;
    struct tm *ptm = localtime(&foo);
    char buf[30] = {0};
    strftime(buf, 30, "%a,%F %H:%M:%S",  ptm);
    os << buf;
}

void App::read(istream &is)
{
    is.read((char *)&_header, sizeof(_header));
}

int App::run()
{
    fstream fs(_argv[1]);
    read(fs);
    fs.close();
    dump(cout);
    cout << "\n";
    return 0;
}

int main(const int argc, const char **argv)
{
    App app(argc, argv);
    return app.run();
}


