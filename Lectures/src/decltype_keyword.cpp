#include <iostream>
#include <type_traits>
#include <vector>
template <typename T>
struct S {
  S() = delete;
};

//c++11 style
template <typename Cont>
auto getElem(Cont& cont, size_t index) -> decltype(cont[index]) {

}
//c++14 style 
template <typename Cont>
decltype(auto) getElem(Cont& cont, size_t index) {
}
//BRAINFUCK:
template <typename Cont>
decltype(auto) getelem(Cont& cont, size_t index) {
  //see vector<bool>[] return type
  decltype(auto) elem = cont[index];
  // returns && or & or just type properly
  return elem;
}
template <typename Cont>
decltype(auto) geTelem(Cont& cont, size_t index) {
  //see vector<bool>[] return type
  

  decltype(auto) elem = cont[index];
  //Expression of name id -> always lvalue -> according to decltype from expression, returns type& -> here possibly dangling reference
  return (elem);
}

int main (int argc, char *argv[]) {
  // std::declval<int>();

  int x = 0;
  //At compile time. In the contrary to typid at runtime.
  decltype(x)& y = x;

  //DECLTYPE REMAIN REFERENCES, SO HERE THERE'S vector<int&>
  // std::vector<decltype(z)> arr;

  // useless const, since cann't be const reference, so typeof(z) = int&
  // const decltype(y)  z = x;
  decltype(x)& z = x;
  decltype(x)&& t = std::move(z);

  //reference collapsing - typeof(k) = int&
  decltype(y)&& k = x;
  //with simple auto - int, with decltype(auto) - int&
  decltype(auto) h = k;

  // По сути деклтайп от имени и деклтайп от экспрешина
  // - это 2 разных маханизма
}

