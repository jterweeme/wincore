#include "kompakt.h"

template <typename T> void myAssert(T a, T b, const char *err = "Error")
{
    if (a != b)
        throw err;
}

void myAssert(const char *a, const char *b, size_t n, const char *err = "Error")
{
    if (strncmp(a, b, n) != 0)
        throw err;
}

template <> void myAssert(const char *a, const char *b, const char *err)
{
    cout << a << "\n";
    cout << b << "\n";

    if (strcmp(a, b) != 0)
        throw err;
}

int main()
{
    ifstream ifs;
    ifs.open("cd5.iso");

    if (!ifs.is_open())
        throw "File does not exist";

    ifs.ignore(32768);
    Descriptors descriptors;
    descriptors.read(ifs);
    myAssert(descriptors[0]->_desc.version, (uint8_t)1);
    myAssert((const char *)descriptors[0]->_desc.identifier, "CD001", 5);
    descriptors.dump(cout);
    cout << "\n";
    ifs.ignore(descriptors[0]->_desc.lbaLSB * 2048 - ifs.tellg());
    DirEntry dir1;
    dir1.read(ifs);
    dir1.dump(cout);
    cout << "\n";
    DirEntry dir2;
    dir2.read(ifs);
    dir2.dump(cout);
    cout << "\n";
    DirEntry dir3;
    dir3.read(ifs);
    dir3.dump(cout);
    cout << "\n";
    DirEntry dir4;
    dir4.read(ifs);
    dir4.dump(cout);
    ifs.close();
    cerr << "\033[1;32mTest4: OK\033[0m\n";
    return 0;
}


