#include "common.h"

int main(int argc, char **argv)
{
    ifstream ifs(argv[1]);
    ofstream ofs(argv[2]);
    ofs << ifs.rdbuf();
    ofs.close();
    ifs.close();
    return 0;
}


