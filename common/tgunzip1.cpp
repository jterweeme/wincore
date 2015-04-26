#include "gunzip.h"

int main()
{
    vector<int> llcodelens(288);
    fill_n(llcodelens.begin(), 144, 8);
    fill_n(llcodelens.begin() + 144, 112, 9);
    fill_n(llcodelens.begin() + 256, 24, 7);
    fill_n(llcodelens.begin() + 280, 8, 8);
    Nau llcodeLens2(llcodelens);
    llcodeLens2.dump(cout);
    cout << "\n";
    cout << llcodeLens2.max() << "\n";
    cerr << "\e[1;32mTest Gunzip1 OK\e[0m\n";
    return 0;
}


