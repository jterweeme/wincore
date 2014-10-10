#include "hasher.h"

void test(const char *data, size_t len, const Hash &hash)
{
    Hasher hasher;
    hasher.stream(data, len);
    
    if (!hasher.hash().isEqual(hash))
        throw "error";
}

int main()
{
    try
    {
        test("abc", 3, Hash(0x98500190, 0xb04fd23c, 0x7d3f96d6, 0x727fe128));
        test("jk8ssl", 6, Hash(0x00000000, 0x7a13e618, 0x16abcac2, 0xa6844707));
        test("abcrubbish", 3, Hash(0x98500190, 0xb04fd23c, 0x7d3f96d6, 0x727fe128));
    }
    catch (const char *s)
    {
        cout << s << "\n";
    }

    return 0;
}


