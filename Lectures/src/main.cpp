#include <algorithm>
#include <functional>
#include <iostream>
#include <locale>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;

struct B {};

struct A {
  int x;
  A(int) : x(0) { cout << "ctor\n"; }
  A(const A&) { cout << "copy\n"; }
  A& operator= (const A&) { cout << "assignment\n"; return *this;}
  A(const char*) {cout << "puuuuuuuuuuuuu";}

  ~A() { cout << "dtor\n"; }

  // operator int() {return 0;}
  // iter foo() {iter xx;(void)xx;}
private:
  using XXX = int;
  struct iter { void bar();};
};  

int main() {
  A a("fdas");
  
  std::hash<std::string> s;
  A b = "fdas";

} 
