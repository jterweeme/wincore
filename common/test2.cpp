#include "gunzip.h"

int main(int argc, char **argv)
{
    FILE *fp = fopen("znew.gz", "r");
    BitInput2File bif2;
    bif2.open(fp);
    GzipStream gs(&bif2);

    for (int i = 0; i < 10; i++)
        cout.put(gs.read());

    cout.put('\n');

    return 0;
}


