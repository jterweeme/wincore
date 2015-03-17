#include "bunzip2.h"

class App
{
public:
    int run(int argc, char **argv);
};

int App::run(int argc, char **argv)
{
#if 1
    cout << argv[1] << "\n";
    //FILE *fp = fopen(argv[1], "r");
    //BitInputFile bi(fp);
    
    ifstream ifs(argv[1]);
    BitInputStream bi(&ifs);
    DecStream ds(&bi);
    ofstream ofs(argv[2]);
    ds.extractTo(ofs);
    ofs.close();
    ifs.close();
    return 0;
#elif 0
    FILE *fp = fopen(argv[1], "r");
    BitInputFile bi(fp);
    DecStreamBuf dsb(&bi);
    //streambuf *bt = new DecStreamBuf(&bi);
    
    for (int i = 0; i < 190778; i++)
        cout.put(dsb.sbumpc());

    return 0;
#else
    //cout << argv[1] << "\n";
    char buf[264];
    FILE *fp = fopen(argv[1], "r");
    BitInputFile bi(fp);
    DecStream dsb(&bi);
    
    for (int i = 0; i < 20; i++)
    {
        dsb.read(buf, 264);
        
        for (int i = 0; i < 264; i++)
            cout.put(buf[i]);
    }
    return 0;   
#endif
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


