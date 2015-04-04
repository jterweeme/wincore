#include "common.h"

class UUID
{
    uint16_t _buf[8];
public:
    void generate();
    void dump(ostream &os);
};

void UUID::generate()
{
    for (int i = 0; i < 8; i++)
        _buf[i] = rand();

    _buf[3] = _buf[3] & 0x0fff | 0x4000;
    _buf[4] = _buf[4] & 0x3fff | 0x8000;
}

void UUID::dump(ostream &os)
{
    os << hex << setfill('0') << setw(4) << _buf[0] 
       << setw(4) << _buf[1] << "-" << setw(4) << _buf[2] << "-" << setw(4) << _buf[3]
       << "-" << setw(4) << _buf[4] << "-" << setw(4) <<  _buf[5] << setw(4) << _buf[6]
       << setw(4) << _buf[7];
}

int main()
{
    srand(time(NULL));
    UUID uuid;
    uuid.generate();
    uuid.dump(cout);
    cout << "\n";
    return 0;
}


