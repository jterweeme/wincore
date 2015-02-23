//#include <fstream>
#include "bitinput.h"

int main()
{
    ifstream ifs;
    ifs.open("whouse.jpg");
    BitInputStream b(&ifs);
    cerr << b.readBool() << "\n";
    cerr << hex << b.readBits(24) << "\n";
    ifs.close();
    return 0;

}


