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
    string s1("nonsens");
    string s2("nonsens2");
    int x = s1.compare(s2);
    cout << x << "\n";
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


#if 0
    ifstream ifs("minimal.gif");
    char buf[43] = {0};
    ifs.read(buf, 43);
    cout << "onzin\n";
#endif

    return 0;
}


