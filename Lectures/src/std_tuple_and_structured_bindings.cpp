#include <cstdint>
#include <map>
#include <set>
#include <tuple>
#include <utility>
#include <vector>


//tie and make tuple
//interesting things with tie and initialization
//like:
void foo() {
  std::set<int> s;
  bool was_inserted;
  std::set<int>::iterator iter;

  std::tie(iter, was_inserted) = s.insert(5);
  //or
  std::tie(iter, std::ignore) = s.insert(5); //ignore - empty struct with defined template assignment operator that does nothing.
}

template <typename Head, typename... Tail>
struct tuple {
private:
  template<size_t T, typename... Types>
  friend decltype(auto) get(tuple<Types...>&);

  Head head;
  tuple<Tail...> tail;

};

//There are tuple_cat  tuples_size, tuple_element functions 
//
//cann't make get as a method due to in template function we'd need to write t.template get<1>();
template <size_t N, typename... Types>
auto get(tuple<Types...>& t) -> decltype(auto) {
  if constexpr (N == 0) return t.head;
  else return get<N - 1>(t.tail);
}
int main (int argc, char *argv[]) {
  // since c++11
  // std::tuple<int, double, char> t (1, 2.0, 'a');
  //deduction guides
  std::tuple t (1, 2.0, 'a');
  auto& d = std::get<1>(t);

  //structure bindings since c++17
  auto&& [x, y, z] = t;

  std::vector<int> arr;
  std::pair v(1, arr); //due to deduction guides

  //for structure binding define tuple_size
  // for (const auto& [key, value] : map)
  return 0;
}
