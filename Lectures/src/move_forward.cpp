#include <iostream>
#include <type_traits>
#include <utility>

//See: perfect forwarding/universal refernces. And refernce collapsing.

//Consider difference cases with lvalue and rvalue and their type
//
//Due to remove reference here, the compiler is unable to deduce type T, 
//so we must excplicitly write template like: std::forward<Args>(args)...
template <typename T>
T&& forward(std::remove_reference_t<T>& value) noexcept {
  return static_cast<T&&>(value);
}

//May be usefull when we forward result of function like std::foward<Args>(f(args...))...
template <typename T>
T&& forward(std::remove_reference_t<T>&& value) noexcept {
  static_assert(!std::is_lvalue_reference_v<T>, "only rvalue ref are allowed");
  return static_cast<T&&>(value);
}

//Get T&& value since move must always works for both rvalue and lvalue and must return rvalue
template <typename T>
std::remove_reference_t<T>&& move(T&& value) noexcept {
  return static_cast<std::remove_reference_t<T>&&>(value);
}


class A {
public:
  A() {std::cout << 1;}
  // A(A &&) {std::cout << 2;}
  A(const A &) {std::cout << 3;} 
  ~A() = default;

private:
  
};

template <typename T>
void foo(T&&x){std::forward<T>(x);}
int main(){
}
