
#include <any>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>


//type erasure(dynamicaly can swap types of underlying container)
class any {
  //Honeslty, type erasuyre in std::any is made through unions and static pointers like in funciton(similar to our 
  //implementation of vtabel) 
private:

  template <typename U>
  friend U& any_cast(any& a);

  struct Base {
    virtual Base* getCopy() const = 0;
    virtual ~Base() = default;
  };

  template <typename T>
  struct Derived : public Base{
    T value;
    Derived(const T& value): value(value) {}
    Derived(T&& value): value(std::move(value)) {}
    Base* getCopy() const override   { return new Derived(value); }
  };

  Base* ptr;
public:
  template <typename T>
  any(const T& value): ptr(new Derived<T>(value)) {}

  any(const any& other) : ptr(other.ptr->getCopy()) {}


  ~any() {
    delete ptr;
  }
  
};

template <typename T>
T any_cast(any& a) {
  auto* p = dynamic_cast<any::Derived<std::remove_reference_t<T>>*>(a.ptr);

  if (!p) {
    throw std::bad_any_cast();
  }

  return p->value;
}

int main (int argc, char *argv[]) {
  std::vector<int> arr = {1, 2};

  std::any a = 5;
  std::cout << std::any_cast<int&>(a);

  a = "fa\n";
  std::cout << std::any_cast<const char*>(a);

  a = std::string("fdas\n");
  std::cout << std::any_cast<std::string>(a);
  //std::bad_any_cast exception
  // std::cout << std::any_cast<int>(a);


  a = arr;//copy
  std::cout << std::any_cast<decltype(arr)&>(a)[0];

  a = 3.4;
  //cast to copy
  std::cout << std::any_cast<double>(a);

  return 0;
}
