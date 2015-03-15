#if 0
#include "common.h"
#else
#include <iostream>
#include <ctime>
using namespace std;
#endif

#include "myassert.h"

void
test(time_t tt, uint32_t y, uint32_t yday, uint32_t mon, uint32_t day, uint32_t h, uint32_t min)
{
    struct tm *tm1 = gmtime(&tt);
    MyAssert ma;
    ma.equals(tm1->tm_year, y);
    ma.equals(tm1->tm_yday, yday);
    ma.equals(tm1->tm_mon, mon);
    ma.equals(tm1->tm_mday, day);
    ma.equals(tm1->tm_hour, h);
    ma.equals(tm1->tm_min, min);
}

int main(int argc, char **argv)
{
    test(0, 70, 0, 0, 1, 0, 0);
    test(60, 70, 0, 0, 1, 0, 1);
    test(3600, 70, 0, 0, 1, 1, 0);
    test(86400, 70, 1, 0, 2, 0, 0);
    test(123456, 70, 1, 0, 2, 10, 17);
    cerr << "\e[1;32mTestGMTime1: OK\e[0m\n";
    return 0;
}


