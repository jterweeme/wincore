#include "fector.h"

void Fugt::dump(ostream &os) const
{
    os << "Fugt, size: " << size() << "\n";

    for (uint32_t i = 0; i < size(); i++)
        os << (int)at(i) << " ";
}


