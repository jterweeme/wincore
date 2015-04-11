#include "od.h"

int main(int argc, char **argv)
{
#ifdef _WIN32
    //_setmode(_fileno(stdin), O_BINARY);
#endif
    AppOd app;
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


