#include <fstream>
#include "bitinput.h"

int main()
{
    ifstream ifs;
    ifs.open("whouse.jpg");
    BitInput b(&ifs);
    
    ifs.close();
    return 0;

}


