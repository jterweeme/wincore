// addressof example
#include "common.h"

struct unreferenceable {
  int x;
  unreferenceable* operator&() { return nullptr; }
};

void print (unreferenceable* m) {
  if (m) cout << m->x << "\n";
  else cout << "[null pointer]\n";
}

int main () {
  void(*pfn)(unreferenceable*) = addressof(print);

  unreferenceable val {10};
  unreferenceable* foo = &val;
  unreferenceable* bar = addressof(val);

  (*pfn)(foo);   // prints [null pointer]
  (*pfn)(bar);   // prints 10

  return 0;
}


