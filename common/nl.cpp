#include "common.h"
int main()
{
    char onzin[255] = {0};

    for (int i = 1; !cin.eof();)
    {
        cin.getline(onzin, 255);

        if (cin.eof())
            break;

        if (strlen(onzin) > 0)
            cout << setw(6) << i++ << "\t" << onzin << "\n";
        else
            cout << "       " << onzin << "\n";
    }
    return 0;
}


