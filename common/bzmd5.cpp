namespace mystl { }
using namespace mystl;
#include "mystl.h"
#include "buftest.h"

int main(int argc, char **argv)
{
    filebuf2 fb;
    FILE *fp = fopen("cat.vcxproj", "r");
    buftest bt(fp);
    fb.open("nl.vcxproj", ios::in);
    //istream is(&fb);
    istream is2(&bt);
    //ifstream is2("od.vcxproj");
    
#if 0
    for (int c = is2.get(); !is2.eof(); c = is2.get())
    {
        cout << is2.tellg() << "\n";
        cout.put(c);
    }
#else
    for (int c, i = 0; (c = is2.get()) != EOF && i < 400; i++)
        cout.put(c);

    cout << is2.tellg() << "\n";
#endif

#if 0
    for (int c; (c = is2.get()) != EOF;)
        cout.put(c);
#endif

    return 0;
}


