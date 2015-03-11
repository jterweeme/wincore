namespace mystl { }
using namespace mystl;
#include "mystl.h"

void assertEquals(uint32_t a, uint32_t b)
{
    if (a != b)
        throw "error";
}

void teststrlen1(const char *s, uint32_t len)
{
    uint32_t x = strlen(s);
    cout << "Length of " << s << ": " << x << "\n";
    
    if (strlen(s) != len)
        throw;
}

void testisdigit()
{
    char c = '9';
    cout << isdigit(c) << "\n";
}

void testctoi(char c)
{
    Util2 u;
    uint32_t i = u.ctoi(c);
    cout << i << "\n";
}

void testupow(uint32_t base, uint32_t exp)
{
    Util2 u;
    cout << u.upow(base, exp) << "\n";
}

int main()
{
    Util2 u;
    teststrlen1("Hello", 5);
    testisdigit();
    testctoi('5');
    testupow(2, 8);
    assertEquals(u.upow(2, 8), 256);
    assertEquals(u.upow(10, 3), 1000);
    assertEquals(u.upow(10, 1), 10);
    assertEquals(u.upow(10, 0), 1);
    cout << strtol("123", "123", 10) << "\n";
    cerr << "\e[1;32mTest Mystl1: OK\e[0m\n";
    return 0;
}


