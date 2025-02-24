// #include <iostream>
// #include <map>
// #include <new>
// #include <iterator>
// #include <set>
// #include <fstream>
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
// #include <type_traits>
// #include <unordered_map>
// #include <vector>

class A {
public:
  int x;
  A()  { std::cout << 0;}
  A(int x) : x(x) {}
  A(const A&) { std::cout << 1;}
  A(A&& other) noexcept : x(other.x)   { std::cout << 2; }
  auto foo() {
    return std::shared_ptr<A>(this);
  }


};

template <typename... Args>
void bar(Args...){ std::cout << "bar ";}
void foo(auto&&... args) {
  std::cout << sizeof...(args);
  bar(args...);
}


template <typename T = int, typename U = double*>
struct B {};

template<typename T, typename U>
struct B<U, T*> {using type = T;};

template <typename T>
void bb() {
   typename T::value_type a;
}

template <typename T>
struct BB {
  using type = const T;
};
int main() {
  BB<const int> a;
  BB<const int>::type x = 5;
  std::add_const_t<int> xx =5;
  // std::map<int, int> mp;
  // for (decltype(mp)::iterator::reference p : mp) {
  //   p.second = 999;
  // }
  // for (auto&& [key, val] : mp) {
  //   std::cout << key << ' ' << val<< '\n';
  // }
}

