#include "gunzip.h"

class Options
{
public:
    void parse(int argc, char **argv);
};

class AppGunzip
{
    Options _options;
public:
    int run(int argc, char **argv);
};

void Options::parse(int argc, char **argv)
{
}

int AppGunzip::run(int argc, char **argv)
{
    _options.parse(argc, argv);
    ifstream ifs(argv[1]);
    ofstream ofs(argv[2]);
    BitInput2Stream bi(&ifs);
    GzipStream gz(&bi);
    gz.extractTo(ofs);
    ofs.close();
    ifs.close();
    return 0;
}

int main(int argc, char **argv)
{
    AppGunzip app;
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


