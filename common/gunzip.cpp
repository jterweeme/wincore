#include <iostream>
#include <fstream>
using namespace std;

class ByteBitInputStream
{
};

class App
{
    void _extract(istream &is, ostream &os);
public:
    int run(int argc, char **argv);
};

void App::_extract(istream &is, ostream &os)
{
}

int App::run(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    ofstream ofs(argv[2]);
    ofs.close();
    ifs.close();
    return 0;
}

int main(int argc, char **argv)
{
    App app;
    return app.run(argc, argv);
}


