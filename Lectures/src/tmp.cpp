#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <ostream>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <stddef.h>

struct B {
  int x;
  B() = default;
  B(const B&) {std::cout << "copy";}
  B(B&&) {std::cout << "move";}
  virtual ~B();
};
struct A : B{
  //some 
  int b;
  A() = default;
  A& operator=(const A&) = delete;
  // A(const A&) = delete;
  // A(A&&) noexcept {std::cout << "move\n";}
  ~A() {}
  
};
auto foo() {
  auto func = []() {};

  auto& p = func;
  return p;
}

void bar();
int main()
{
  int some = 1;
}
