#include <functional>
#include <iostream>
int sum(int x, int y) {
  return x + y;
}

int diff(int x, int y) {
  return x - y;
}

int main (int argc, char *argv[]) {
  auto f = std::bind(sum, 2, std::placeholders::_1);
  auto g = std::bind(diff, std::placeholders::_1, 5);
  auto gg = std::bind(diff, 5, std::placeholders::_1);


  int x = 1;
  auto h = std::bind(diff, std::placeholders::_1, x);
  auto hh = std::bind(diff, std::placeholders::_1, std::ref(x));


  std::cout << f(5) << ' ' << g(1) << ' ' << gg(1);
  return 0;
}
