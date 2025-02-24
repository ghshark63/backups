#include <iostream>
int foo(long long* a, int* b) {
  *b = *a + 5;
  return *a;//with -O2 will not dereference but will remember result from the prev line.
}


void foo() {
  throw 1;
}
int main (int argc, char *argv[]) {
  long long a = 10;
  std::cout << foo(&a, reinterpret_cast<int*>(&a));
  return 0;
}
