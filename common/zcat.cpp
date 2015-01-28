#include "gunzip.h"

class AppZcat
{
public:
    int run(int argc, char **argv);
};

int AppZcat::run(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    BitInput bi(&ifs);
    GzipStream gz(&bi);
    gz.extractTo(cout);
    ifs.close();
    return 0;
}

int main(int argc, char **argv)
{
    AppZcat app;
    return app.run(argc, argv);
}


