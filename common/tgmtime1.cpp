#if 1
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
    char s[22] = {0};
    strftime(s, sizeof(s), "%F %T", tm1);
    cout << s << "\n";
    MyAssert ma;
    ma.equals(tm1->tm_year, y);
    ma.equals(tm1->tm_yday, yday);
    ma.equals(tm1->tm_mon, mon);
    ma.equals(tm1->tm_mday, day);
    ma.equals(tm1->tm_hour, h);
    ma.equals(tm1->tm_min, min);
}

void
test(time_t tt, uint32_t y, uint32_t yday,
    uint32_t mon, uint32_t day, uint32_t h, uint32_t min, uint32_t sec)
{
    struct tm *tm1 = gmtime(&tt);
    char s[22] = {0};
    strftime(s, sizeof(s), "%F %T", tm1);
    cout << s << "\n";
    MyAssert ma;
    ma.equals(tm1->tm_year, y);
    ma.equals(tm1->tm_yday, yday);
    ma.equals(tm1->tm_mon, mon);
    ma.equals(tm1->tm_mday, day);
    ma.equals(tm1->tm_hour, h);
    ma.equals(tm1->tm_min, min);
    ma.equals(tm1->tm_sec, sec);
}

int main(int argc, char **argv)
{
    test(0, 70, 0, 0, 1, 0, 0, 0);
    test(60, 70, 0, 0, 1, 0, 1, 0);
    test(3600, 70, 0, 0, 1, 1, 0, 0);
    test(86400, 70, 1, 0, 2, 0, 0, 0);
    test(123456, 70, 1, 0, 2, 10, 17, 36);
    test(1234567, 70, 14, 0, 15, 6, 56, 7);
    test(3000000, 70, 34, 1, 4, 17, 20);
    test(6000000, 70, 69, 2, 11, 10, 40);
    test(12345678, 70, 142, 4, 23, 21, 21);
    test(25000000, 70, 289, 9, 17, 8, 26);
    test(75000000, 72, 138, 4, 18, 1, 20);
    test(125000000, 73, 350, 11, 17, 18, 13);
    test(275000000, 78, 260, 8, 18, 20, 53);
    test(946684800, 100, 0, 0, 1, 0, 0);
    test(1262304000, 110, 0, 0, 1, 0, 0);
    test(1426000000, 115, 68, 2, 10, 15, 6, 40);
    cerr << "\e[1;32mTestGMTime1: OK\e[0m\n";
    return 0;
}


