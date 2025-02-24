#include <iostream>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <algorithm>

struct A {
  A() {std::cout << "default ctor\n";};
  ~A() { std::cout << "dtor\n";}
  A(const A&) {std::cout << "copy\n";}
  A(A&&) noexcept {std::cout << "move\n";}
  int b;
}A_instance;

struct S {
  S(A& x) 
    : a([&x] -> decltype(auto) {
    if (x.b) return x;
    else return x;
  }()), b(5) {}
  A& a;
  int b;
  
};
//Lambas are anonimus classes with const constexpr operator() overloaded
int main (int argc, char *argv[]) {
  A a;
  S s(a);
  //type of cmp is Closure(замыкание). type of expression is lambda.
  auto cmp = [](int x, int y) -> int {
    if (x < y) 
      return 1;
    else 
      return 1u;

  };
  auto hello = [] {
    std::cout << "hello";
  };
  [] {
    std::cout << "immediate invocation\n";
  }();
  //Usefull case
  int y; 
  // std::cin >> y;
  const int x = [](int a) {
    if (a % 2 == 0)
      return a / 2;
    else 
      return (a + 1) / 2;
  }(y);


  auto p = [](){};
  [](){}();
  []{}();


  //Captures
  std::vector v = {1, 2, 3};

  int center = 3;
  //Mutable just unmark operator() as const
  auto cmpp = [&center](int x, int y) mutable {//capture by reference
    return x + center - y - center;
  };
  int bla = 1;
  int blabla = 2;
  auto cmp2 = [center = std::move(center) + 1 + bla] {};//capture with initialization. Attention - with coping Closure it will copy center. Instead use references
  auto cmp3 = [&c = std::as_const(bla)] {};
  auto cmp4 = [&bla, blabla, c = std::move(3)] {};
  //capture all variables by value.
  //Not recomended. See lambdas in classses.
  auto cmp5 = [=](){};
  //capture all by reference
  //Not recomended
  auto cmp6 = [&](){};
  auto cmp7 = [=, &bla](){};//take all by value, but bla by refernce

  std::sort(v.begin(), v.end(), cmpp);


  static int xx = 0;
  auto f = [] {//Cannot capture static/global variable. Able only with automatic starage.
    ++xx;
    A_instance.b = 5;
  };

  //template lambads. Actually templated operator()
  auto ft = []<typename T>(T&& item) 
                           requires requires(T item) { item.second; }
    {
      ++item.second;
      return item;
    };
  // auto ftt = [](auto& item) {...}; //also possible


  //WHY I LOVE C++--------------------------
  auto fb = []<typename T> requires requires {std::declval<T>().second;} [[nodiscard]]  (T&& item, auto&& item2) 
    mutable noexcept(noexcept(false))//always noexcept since noexcept(false) is true as an operator
    -> decltype(auto) requires requires(T x) {x.second;}
    {
      ++item.second;
      return true;
    };
  // std::map<int, int> mp;
  // std::cout << '\n' <<  noexcept(fb(*mp.begin(), 5)) << '\n';

  //There are recursive lambdas
  return 0;
}

//Pack capture
template <typename...  Strings>
requires (std::is_same_v<Strings, std::string> && ...) 
void test(Strings&... strs) {
  auto cmp = [&strs...]{
    ((strs+="fda"), ...);
  };
  auto cmpt = [...subs = std::move(strs)]{};
}

struct SS {
  int a= 3;
  auto getLam() {
    auto f = [=](){return a + 5;};//Implicitly takes this pointer, not a field
    auto ff = [this](){return a + 5;};//Ub if SS deletes before using lambda
    auto fff = [a = a](){return a + 5;};//Good
    // auto fff = [a]{};//CE
    return fff;
  }
};
