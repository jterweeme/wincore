#include "buftest.h"

int main(int argc, char **argv)
{
    filebuf fb;
    FILE *fp = fopen("cat.vcxproj", "r");
    buftest bt(fp);
    fb.open("nl.vcxproj", std::ios::in);
    //istream is(&fb);
    //istream is2(&bt);
    ifstream is2("od.vcxproj");
    
#if 0
    for (int c = is2.get(); !is2.eof(); c = is2.get())
        cout.put(c);
#endif

    for (int c, i = 0; (c = is2.get()) != EOF && i < 40; i++)
        cout.put(c);

    cout << is2.tellg() << "\n";

#if 0
    for (int c; (c = is2.get()) != EOF;)
        cout.put(c);
#endif

    return 0;
}


