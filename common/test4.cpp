#include "common.h"

int main()
{
    ifstream ifs("data.md5");
    
    char onzin[255];
    ifs.getline(onzin, sizeof(onzin));
    cout << onzin << "\n";
    ifs.getline(onzin, sizeof(onzin));
    cout << onzin << "\n";
    ifs.close();
    return 0;
}


