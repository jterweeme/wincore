#include <iostream>
#include <regex>
using namespace std;

int main(const int argc, const char **argv)
{
    while (cin && argc > 1)
    {
        char line[255];
        cin.getline(line, sizeof(line));
        regex rx(argv[1]);

        if (regex_search(line, rx))
            cout << line << "\n";

    }
    return 0;
}


