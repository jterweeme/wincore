#include "common.h"

void functie()
{
    ifstream ifs("ls.cpp");
    
    for (int i = 0; i < 10; i++)
        cout.put(ifs.get());

    cout << "\n" << ifs.tellg() << "\n";
    ifs.close();
}

int main(int argc, char **argv)
{
#if 0
    vector<uint32_t> v1;
    v1.push_back(5800);
    v1.push_back(63241);
    v1.push_back(5432);
    v1.push_back(900);
    sort(v1.begin(), v1.end());

    for (vector<uint32_t>::iterator it = v1.begin(); it != v1.end(); it++)
        cout << *it << "\n";
#endif
#if 0
    try
    {
        functie();
    }
    catch (const char *e)
    {
        cerr << e << "\n";
    }
#endif

    FILE *fp = fopen("minimal.gif", "r");
    
    int c;
    char buf[64] = {0};
    fread((char *)buf, 64, 1, fp);
    
    while ((c = fgetc(fp)) != EOF)
        printf("%x", c);

    return 0;
}


