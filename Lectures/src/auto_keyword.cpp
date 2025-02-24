//since c++11 auto keyword

#include <algorithm>
#include <iostream>
#include <type_traits>

template <typename T>
auto g(T x) {
  return ++x;
}

//trailing return type since c++11
template <typename T>
auto move(T&& value) -> std::remove_reference_t<T>&& {}

//since c++20
//just a shorten for template <typename T> void foo(T) {}
// auto f(auto&& x) {}
// auto f(auto... x) {}
// auto f(auto&&... x) {}

template <auto N>
void foo() {}

int main (int argc, char *argv[]) {
  auto a = g(1);

  //almost the same as for templates type deduction
  auto x = 5;//int
  auto& y = x; //int&
  const auto& z = y; //const int&
  
  auto&& t = x; //REFERENCE COLLAPSING -> int&
  auto&& r = std::move(x); // -> int&&

}


