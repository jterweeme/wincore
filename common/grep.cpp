#include "common.h"

void grep(istream &is, const regex rx)
{
    while (is)
    {
        char line[255] = {0};
        is.getline(line, sizeof(line));

        if (regex_search(line, rx))
            cout << line << "\n";
    }
}

void grep(istream &is, const char *rgx)
{
    regex rx(rgx);
    grep(is, rx);
}

int main(const int argc, const char **argv)
{
    if (argc < 2)
    {
        cerr << "Usage: \n";
        return -1;
    }

    if (argc == 2)
        grep(cin, argv[1]);

    if (argc == 3)
    {
        ifstream ifs(argv[2]);
        grep(ifs, argv[1]);
        ifs.close();
    }
    return 0;
}


