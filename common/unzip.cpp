#include "bitinput.h"

class UnzipFile
{
};

class Options
{
    string _fn;
public:
    void parse(int argc, char **argv);
};

class App
{
public:
    int run(int argc, char **argv);
};

int App::run(int argc, char **argv)
{
    Options o;
    o.parse(argc, argv);
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

