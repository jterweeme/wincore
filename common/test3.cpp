#include "bunzip2.h"

class App
{
public:
    int run(int argc, char **argv);
};

int App::run(int argc, char **argv)
{
    FILE *fp = fopen("battery.bz2", "r");
    BitInputFile bif;
    bif.open(fp);
    DecStreamBuf dsb(&bif);
    
    for (int i = 0; i < 10; i++)
        cout.put(dsb.sbumpc());

    fclose(fp);
    return 0;
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


