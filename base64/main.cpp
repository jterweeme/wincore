#include <iostream>
#include <fstream>
#include <stdint.h>
using namespace std;

char set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

class App
{
public:
    App() { }
    void convert(istream &is, ostream &os);
    int run(int argc, char **argv);
};

void App::convert(istream &is, ostream &os)
{
    for (int i = 1; is; i++)
    {
        char buf[3] = {0};
        is.read(buf, sizeof(buf));

        if (is.gcount() <= 0)
            break;

        uint8_t remainder = is.gcount() % 3;
        os << set[buf[0] >> 2];
        os << set[(buf[1] >> 4 | buf[0] << 4) & 0x3f];
        os << (remainder == 1 ? '=' : set[(buf[1] << 2 | buf[2] >> 6) & 0x3f]);
        os << (remainder != 0 ? '=' : set[buf[2] & 0x3f]);

        if (i % 19 == 0)
            os << "\n";
    }

    os << "\n";
}

int App::run(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    convert(ifs, cout);
    ifs.close();
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


