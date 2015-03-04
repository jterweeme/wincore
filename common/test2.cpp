#include "common.h"

//void function1(const string2 s) { }
void function1(const char *s) { }
void function1(const uint8_t c) { }
void function1(const uint16_t c) { }
void function1(const char c) { }
void function1(const uint32_t u) { }

int main(int argc, char **argv)
{
#if 0
    vector<uint32_t> v1;
    v1.push_back(5800);
    v1.push_back(63241);
    v1.push_back(5432);
    v1.push_back(900);
    sort(v1.begin(), v1.end());

    for (vector<uint32_t>::iterator it = v1.begin(); it != v1.end(); it++)
        cout << *it << "\n";
#endif
    return 0;
}


