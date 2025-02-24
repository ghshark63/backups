//class templates argument deduction since c++17

#include <initializer_list>
#include <iterator>
#include <vector>
template <typename T>
struct vector {
  template <typename U>
  vector(std::initializer_list<U>) {}
  template <typename Iter>
  vector(Iter, Iter) {}



};

//explicit template deduction guide
template <typename Iter>
vector(Iter, Iter) -> vector<typename std::iterator_traits<Iter>::value_type>;

template <typename U>
vector(std::initializer_list<U>) -> vector<U>;

int main (int argc, char *argv[]) {
  std::vector arr = {1,2};
  std::vector arr2(arr.begin(), arr.end());
}
