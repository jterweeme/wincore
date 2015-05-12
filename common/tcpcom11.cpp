#if 0
// istream::getline example
#include "common.h"     // std::cin, std::cout

int main () {
  char name[256], title[256];

  cout << "Please, enter your name: ";
  cin.getline (name,256);

  cout << "Please, enter your favourite movie: ";
  cin.getline (title,256);

  cout << name << "'s favourite movie is " << title;

  return 0;
}
#else
#include "common.h"

int main()
{
    for (int c = 0; c != EOF;)
    {
        c = cin.get();
        cout << c;
    }
    return 0;
}

#endif

