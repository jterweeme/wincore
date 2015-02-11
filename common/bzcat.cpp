#include "bunzip2.h"

class App
{
public:
    int run(int argc, char **argv);
};

#if 1
int App::run(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    BitInputStream bi(&ifs);
    DecStream ds(&bi);
    ds.extractTo(cout);
    ifs.close();
    return 0;
}
#else
int App::run(int argc, char **argv)
{
    FILE *fp = fopen(argv[1], "r");
    BitInputFile bif(fp);
    DecStreamBuf dsb(&bif);
    istream is(&dsb);
 
    for (int c; (c = is.get()) != EOF;)
        cout.put(c);

    return 0;
}
#endif

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


