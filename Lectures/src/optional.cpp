//sinc c++17
#include <optional>
template <typename T>
class optional {
public:
  optional() = default;
  optional(const T& newval) : is_initialized(true) {
    new (value) T(newval);
  }
  ~optional() {
    if (is_initialized) reinterpret_cast<T*>(value)->~T();
  }
  bool has_value() const { return is_initialized; }
  operator bool() const { return is_initialized; }

  T& operator*() & {
    return reinterpret_cast<T&>(*value);
  }
  const T& operator*() const &{
    return reinterpret_cast<const T&>(*value);
  }
  T&& operator*() &&{
    return reinterpret_cast<T&&>(*value);
  }
  const T&& operator*() const && {
    return reinterpret_cast<const T&&>(*value);
  }

  //4 versions overloads
  T& Value() & {
    if (!is_initialized) throw std::bad_optional_access();
    return reinterpret_cast<T&>(*value);
  }

  //since c++23
  //Deducing this: wrtie one function instead of 4 the identical
  // template <typename Self>
  // decltype(auto) Value(this Self&& self) {
  //   if (!is_initialized) throw std::bad_optional_access();
  //   //forward_like since c++23. Quite easy to implement
  //
  //   using DesiredType = dectype(std::forward_like<decltype(self)>(std::declval<T>()));
  //   return reinterpret_cast<DesiredType>(*self.value);
  // }
  //
  // Even more funny:
  // decltype(auto) Value(this auto&& self) {....}

  T& value_or(T& other) {
    return is_initialized ? reinterpret_cast<T&>(*value) : other;
  }
private:
  char value[sizeof(T)];
  bool is_initialized = false;
};

void foo() {
  std::optional<int> g;
}
