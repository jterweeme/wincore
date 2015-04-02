#include "common.h"
#include "myassert.h"

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

string retString()
{
    return string("nonsens");
}

void testString()
{
    string s1("Hello world\n");
    string s2 = retString();
    cout << s2 << "\n";
}

void testFileBuf()
{
    filebuf fb;
    fb.open("gzip.cpp", ios::in);
    
    for (int i = 0; i < 10; i++)
        cout.put(fb.sbumpc());

    cout.put('\n');
    fb.pubseekpos(100, ios::in | ios::out);

    for (int i = 0; i < 10; i++)
        cout.put(fb.sbumpc());

    cout.put('\n');
}

int main()
{
    Util2 u;
    teststrlen1("Hello", 5);
    testisdigit();
    testctoi('5');
    testupow(2, 8);
    MyAssert ma;
    ma.equals(u.upow(2, 8), 256);
    ma.equals(u.upow(10, 3), 1000);
    ma.equals(u.upow(10, 1), 10);
    ma.equals(u.upow(10, 0), 1);
    //cout << strtol("123", "123", 10) << "\n";
    testFileBuf();
    testString();
    cerr << "\e[1;32mTest Mystl1: OK\e[0m\n";
    return 0;
}


