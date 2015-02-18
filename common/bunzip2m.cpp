namespace mystl { }
using namespace std;
#include "bunzip2.h"

class App
{
public:
    int run(int argc, char **argv);
};

int App::run(int argc, char **argv)
{
    cout << argv[1] << "\n";
    mystl::ifstream ifs(argv[1]);
    BitInputStream bi(&ifs);
    DecStream ds(&bi);
    mystl::ofstream ofs(argv[2]);
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


