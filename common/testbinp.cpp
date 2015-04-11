//#include <fstream>
#include "bitinput.h"
#include "myassert.h"

class App
{
public:
    int run();
};

int App::run()
{
    MyAssert ma;
    ifstream ifs("minimal.gif");
#if 0
    BitInputStream b(&ifs);
    ma.equals(false, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(false, b.readBool());
#else
    //BitInput2 b(&ifs);
    BitInputStreamRev b(&ifs);
    ma.equals(true, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(true, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(false, b.readBool());
    ma.equals(true, b.readBool());
#endif
    ifs.close();
    cerr << "\e[1;32mTestBitinput: OK\e[0m\n";
    return 0;
}

int main()
{
    App app;
    int ret = -1;

    try
    {
        ret = app.run();
    }
    catch (const char *e)
    {
        cerr << e << "\n";
    }

    return ret;
}


