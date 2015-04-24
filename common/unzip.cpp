#include "bitinput.h"

class UnzipFile
{
};

class Options
{
    bool _help = true;
    string _fn;
public:
    void parse(int argc, char **argv);
    bool help() const { return _help; }
};

class App
{
    void help(ostream &os);
public:
    int run(int argc, char **argv);
};

void App::help(ostream &os)
{
    os << "unzip ...\n";
}

int App::run(int argc, char **argv)
{
    Options o;
    o.parse(argc, argv);

    if (o.help())
    {
        help(cout);
        return 0;
    }

    return 0;
}

void Options::parse(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
        }
        else
        {
            _fn = argv[++i];
        }
    }
}

int main(int argc, char **argv)
{
    App app;
    int ret = -1;

    try
    {
        ret = app.run(argc, argv);
    }
    catch (const char *e)
    {
        cerr << e << "\n";
    }

    return ret;
}

