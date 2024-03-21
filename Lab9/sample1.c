#include <math.h>
#include <stdio.h>

void do_nothing() {
  // Intentionally left empty
}

int main() {
  double z;
  int k = 10;
  z = pow(2, k);
  do_nothing();
  printf("At runtime main() is at %p\n", main);
  return 0;
}
