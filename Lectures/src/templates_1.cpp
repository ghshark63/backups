
#include <vector>
template <typename T, typename U>
struct IsSame { 
  static constexpr bool value = false;
};

template <typename T>
struct IsSame<T, T> {
  static constexpr bool value = true;
};

template <typename T, typename U>
constexpr bool IsSame_v = IsSame<T, U>::value;

template <typename T>
using vec = std::vector<T>;

template <typename T>
struct isptr { 
  using type = T;
};

template <typename T>
struct isptr<T*> {
  using type = T;
};

template <typename T>
using isptr_t = typename isptr<T>::type; 

template <void()>
bool ff;
