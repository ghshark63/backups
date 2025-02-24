//constraints and requirements
//replacing for enable_if

#include <concepts>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>

// template <typename T, std::enable_if_t<std::is_class_v<T>, bool> = true>

template <typename T>
requires std::is_class_v<T> && std::is_copy_constructible_v<T>
void foo(const T& v) /* requires std::is_class_v<T>  */{}

template <typename T>
requires std::is_integral_v<T>
void foo(T) {}

// void g(long x) requires (sizeof(x) == 4) {} //can't require from non-template function due to the following problems for linker. Moreover it's seems strange from logcal point of view

template <size_t N>
requires (N <= 100)
struct Array { };



//requires expressions

//see cppreference
//Simple requirements
//Nested requirements
//Compaound requirements
//Type requirements 
template <typename T>
void test() {
  std::cout << requires(int a, T b, T c) {a + b + c;}; //prints either 1 or 0
  std::cout << typeid(requires(int a, T b, T c) {a + b + c;}).name(); //prints bool
}

template <typename T>
requires requires(T x, T y) { x + y; }
T add(T a, T b) requires requires {a + b;}{ return a + b;}

template <typename T, size_t N>
// requires requires { sizeof(T) == N; } //Wrong!!! doesn't check for truth, but compilation
// requires (sizeof(T) == N) // Correct
requires std::is_class_v<T> && (N < 50) && requires {
  requires requires {
    1 + 4 == 4;//Wil be true since this compiles
  };
  requires 1 + 4 == 5; 
  requires sizeof(T) == N; //Nested requirement
  typename T::value_type;
}

void test() {}

template <typename T>
struct A {
  void foo() {
    T a, b;
    a + b;
  }
  void operator()(){}
};

template <typename T>
requires requires (T a) {
// typename std::hash<T>;//Wrong!!! type with T exists!
  std::hash<T>()(a);
}
void test2() {
}


//How does it work??????????????????
//Find out it
struct S {
  bool operator==(const S&) {}
};

template <typename T>
requires requires(T a, T b) {
a == b;
}
void fosd(){}



//Concepts
template <typename T, typename U>
concept same_as = std::is_same_v<T, U>;//type trait
template <typename T>
concept example = std::predicate<T>;//concept

template <typename T>
concept InputIterator = std::is_class_v<T> && requires (T it) {

{++it} noexcept -> std::same_as<T&>;
{*it} -> std::convertible_to<typename std::iterator_traits<T>::value_type>; //Imprtant: first template parameter implicitly substituted
static_cast<T>(std::declval<T>());
};



template <InputIterator T>
void foo(){}

// worse
// template <typename T> 
// requires InputIterator<T> && true
// void foo(){}

template <InputIterator It, std::predicate<decltype(*std::declval<It>())> Pred>
It ffind_if(It begin, It end, Pred pred) { std::cout << 543;} 

//Here beg and end may be different types.
auto ffind_if_2(InputIterator auto beg, InputIterator auto end, std::predicate<decltype(*beg)> auto p) {}
//Fixed, but less readable
auto ffind_if_3(InputIterator auto beg, decltype(beg) end, std::predicate<decltype(*beg)> auto p) {}


//std::advane implementation with concedpts

//such overload works, but only if the compiler is able to prove that
//one concept is a special case of another one.
//How hoes it decides? 
template <std::input_iterator Iter>
void advance(Iter& iter, size_t n) {
  for (size_t i = 0; i < n; i++) ++iter; 
}

template <std::random_access_iterator Iter>
void advance(Iter& iter, size_t n) {
  iter += n;
}
template <typename T>
concept input_iterator = requires(T x) {
*x;
++x;
};
template <typename T>
concept forward_iterator = input_iterator<T> &&
  requires(T x) {
x++;
};
template <typename T>
concept bidir_iterator = forward_iterator<T> &&
  requires(T x) {
--x;
x--;
};
template <typename T>
concept random_iterator = bidir_iterator<T> &&
  requires(T x, T y) {
x - y;
x < y; x > y; //..
x += 1;//...
x + 1;//..
};


template <typename T, typename U>
concept ex1 = std::is_same_v<T, U>;

template <typename T, typename U>
concept ex2 = std::is_same_v<T, U> && std::is_copy_assignable_v<T>;
// template <typename T, typename U>
// concept ex2 = ex1<T, U> && std::is_copy_assignable_v<T>;

template <typename U, ex1<U> T>
void food() {}

template <typename U, ex2<U> T>
void food() {}

template <bidir_iterator T>
void foof(T) {std::cout << 1;}
template <random_iterator T>
void foof(T) {std::cout << 2;}

int main (int argc, char *argv[]) {
  test<int>();
  // std::vector<int> arr;
  // foof(arr.begin());
  //
  // ffind_if_2(arr.begin(), arr.end(), [](int){return 4;});
}
