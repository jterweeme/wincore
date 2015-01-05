#include "mystlod.h"

namespace mystl
{
    istream cin(stdin);
    ostream cout(stdout);
    ostream cerr(stderr);
    base2 hex(base2::HEX);
    base2 oct(base2::OCT);
    dummy dummy1;
    dummy& setw(int length) { return dummy1; }
    dummy& setfill(char c) { return dummy1; }
}

