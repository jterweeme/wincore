#include "common.h"

int main(int argc, const char **argv)
{
    switch (argc)
    {
    case 1:
        while (true)
            cout << "y\n";
    case 2:
        while (true)
            cout << argv[1] << "\n";
    default:
        while (true)
        {
            cout << argv[1];

            for (int i = 2; i <= argc - 1; i++)
                cout << " " << argv[i];
            
            cout << "\n";
        }
    }

    return 0;
}

