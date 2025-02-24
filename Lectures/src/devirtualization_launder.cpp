#include <iostream>
#include <utility>

struct A {
  virtual int foo();
};

struct B : A {
  int foo() override { new (this) A; return 1; }
};

int A::foo() { new (this) B; return 2; }

void run() {
  //problem - untill c++17 every stl container was ub if a struct it holds contained a const field
  //because of placement new on the same piece of memory were ub in case not compatible types or const field.
  A a;
  int n = 0, m = 0;

  n = a.foo();
  m = a.foo();
  std::cout << n + m << '\n'; //4

  n = a.foo();
  m = (&a)->foo();
  std::cout << n + m << '\n'; // 4

  n = a.foo();
  m = std::launder(&a)->foo(); // means "forgot everything about pointer a"
  std::cout << n + m << '\n'; // 3


}

int main (int argc, char *argv[]) {
  run();
  return 0;
}
