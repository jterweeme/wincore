#include "buftest.h"

int main(int argc, char **argv)
{
    filebuf fb;
    FILE *fp = fopen("cat.vcxproj", "r");
    buftest bt(fp);
    fb.open("nl.vcxproj", std::ios::in);
    istream is(&fb);
    istream is2(&bt);
    
#if 0
    for (int c; (c = is.get()) != EOF;)
        cout.put(c);
#endif

    for (int c; (c = is2.get()) != EOF;)
        cout.put(c);

    return 0;
}


