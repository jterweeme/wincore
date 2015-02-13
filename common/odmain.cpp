namespace mystl { }
using namespace mystl;
#include "od.h"

int main(int argc, char **argv)
{
#ifdef _WIN32
    _setmode(_fileno(stdin), O_BINARY);
#endif
    AppOd app;
    return app.run(argc, argv);
}


