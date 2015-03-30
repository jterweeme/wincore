//using namespace std;
#include "gunzip.h"

int main()
{
    Vint llcodelens(288);
    std::fill_n(llcodelens.begin(), 144, 8);
    std::fill_n(llcodelens.begin() + 144, 112, 9);
    std::fill_n(llcodelens.begin() + 256, 24, 7);
    std::fill_n(llcodelens.begin() + 280, 8, 8);
    Nau llcodeLens2(llcodelens);
    llcodeLens2.dump(cout);
    cout << "\n";
    cout << llcodeLens2.max() << "\n";
    cerr << "\e[1;32mTest Gunzip1 OK\e[0m\n";
    return 0;
}


