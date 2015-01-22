#include "bzip2.h"
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
    cout << argv[1] << "\n";
    ifstream ifs(argv[1]);
    BitInput bi(&ifs);
    DecStream ds(&bi);
    ofstream ofs(argv[2]);
    ds.extractTo(ofs);
    ofs.close();
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
    catch (const char *e)
    {
        cout << e << "\n";
    }
    catch (...)
    {
        cout << "Unknown exception\n";
    }

    return -1;
}


