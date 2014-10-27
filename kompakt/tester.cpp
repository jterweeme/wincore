#include <sys/stat.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv)
{
    struct stat buf;
    int i = stat(argv[1], &buf);
    
    if (i)
    {
        cout << "\033[1;33mSkipping " << argv[1] << "\033[0m\n";
        return 0;
    }

    system("./kompakt -x -f battery.iso -o battery");

    return 0;
}


