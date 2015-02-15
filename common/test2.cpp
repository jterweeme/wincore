namespace mystl { }
using namespace mystl;
#include "common.h"

int main(int argc, char **argv)
{
    ifstream ifs("yes.cpp");
    char buf[10];
    ifs.read(buf, sizeof(buf));
    buf[9] = 0;
    cout << buf << "\n";
    ifs.close();
    return 0;
}


