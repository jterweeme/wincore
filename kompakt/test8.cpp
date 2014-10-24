#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

bool wayToSort(int i, int j) { return i > j; }

int main()
{
    vector<int> vector1;
    vector1.push_back(5);
    vector1.push_back(1);
    vector1.push_back(2000);
    vector1.push_back(89);
    vector1.push_back(89);
    vector1.push_back(89);
    vector1.push_back(47);
    
    for (vector<int>::iterator it = vector1.begin(); it != vector1.end(); it++)
        cout << *it << "\n";

    cout << "\n";
    sort(vector1.begin(), vector1.end(), wayToSort);

    for (vector<int>::iterator it = vector1.begin(); it != vector1.end(); it++)
        cout << *it << "\n";

    return 0;
}


