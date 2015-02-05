#include "bunzip2.h"
#include <fstream>
#include <algorithm>
#include <cstring>

class App
{
public:
    int run(int argc, char **argv);
};

int App::run(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    BitInputStream bi(&ifs);
    DecStream ds(&bi);
    ds.extractTo(cout);
    ifs.close();
    return 0;
}

int main(int argc, char **argv)
{
    App app;

    try
    {
        return app.run(argc, argv);
    }
    catch (exception &e)
    {
        cerr << e.what() << "\n";
    }
    catch (const char *e)
    {
        cerr << e << "\n";
    }
    catch (...)
    {
        cerr << "Unknown exception\n";
    }

    return -1;
}


