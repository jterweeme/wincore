#include "common.h"

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

    ifstream3 ifs("onzin.txt");
    
    for (int i = 0; i < 10; i++)
        cout.put(ifs.get());

    cout << "\n" << ifs.tellg() << "\n";
    ifs.close();
    return 0;
}


