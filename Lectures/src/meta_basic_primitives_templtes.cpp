//metafunctions, sfinae, sfinae friendliness, concepts, constraints

#include <iostream>
#include <type_traits>

template <typename T>
struct type_identity {
  using type = T;
};

template <typename T>
using type_identity_t = type_identity<T>::type;

template <typename T, T x>
struct integral_constant {
  static constexpr T value = x;
};

template <bool b>
using bool_constant = integral_constant<bool, b>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;



template <typename T>
struct is_lvalue_reference : false_type {};

template <typename T>
struct is_lvalue_reference<T&> : true_type {};

template <typename T>
constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;


//No short-circuiting
template <typename... Types>
struct conjunction {
  static constexpr bool value = (Types::value && ...);
};

//conjustion disjunction etc...


//sfinae, std::enable_if 
//Substitution failure is not a an error!

template <bool b, typename = void>//second parametrer since enable_if's used as return type as well as template
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
  using type = T;
};

template <bool B, typename T>
using enable_if_t = enable_if<B, T>::type;

template <typename T, enable_if_t<std::is_integral_v<T>, bool> = true>
void foo(T) {std::cout << 1;}

template <typename T, enable_if_t<!std::is_integral_v<T>, bool> = false>
void foo(T) {std::cout << 2;}

namespace detail {

template <typename T>
std::true_type test(int T::*);

template <typename T>
std::false_type test(...);
}

template <typename T>
struct is_class : decltype(detail::test<T>(nullptr)) {};

template <typename T>
constexpr bool is_class_v = is_class<T>::value;


template <typename T>
T&& declval();

namespace detail {
template <typename T, typename... Args>
std::true_type testmc(decltype(std::declval<T>().construct( std::declval<Args>()...), nullptr));//comma trick with decltype

template <typename...>//important since we manually calls with templates
std::false_type testmc(...);
}

template <typename T, typename... Args>
struct has_method_construct : decltype(detail::testmc<T, Args...>(nullptr)){};



namespace detail {
template <typename T>
std::true_type testc( decltype(T(std::declval<T&>()), nullptr) );

template <typename...>
std::false_type testc(...);
}

template <typename T, typename... Args>
struct is_copy_constructible : decltype(detail::testc<T>(nullptr)){};

namespace detail {
template <typename T>
std::true_type testm( 
  enable_if_t< noexcept(T(std::declval<T&&>())), decltype(nullptr)> 
);
//the same bhaviour:
//auto testm(void*) -> bool_constant<noexcept(T(std::declval<T&&>()))>;

template <typename...>
std::false_type testm(...);
}

template <typename T>
struct is_nothrow_move_constructible : decltype(detail::testm<T>(nullptr)){};


namespace detail {
template <typename Base, typename Derived>
std::true_type testbase(Base*);

template <typename...>
std::false_type testbase(...);

}
//see cppreference. Here there's problem with private and ambigious inheritance. 
//To solve use additional level of redirection
template <typename B, typename D>
struct is_base_of : std::conjunction<
                      std::is_class<B>, 
                      std::is_class<D>, 
                      decltype(detail::testbase<B, D>(static_cast<D*>(nullptr)))>{};


template <typename... Types>
struct common_type;

template <typename T, typename U>
struct common_type<T, U> 
    : std::type_identity<
          decltype(true ? std::declval<T>() : std::declval<U>()) //тернарный оператор ищет общ.тип(но он только 1 из 2 представленных)
// NOT SFINAE FRIENDLINESS 
      > {};

template <typename T, typename... Types>
struct common_type<T, Types...>
    : common_type<T, common_type<Types...>> {};

struct Good {
  Good(Good&&) noexcept;
};
struct Bad {
  Bad(Bad&&); 
};
struct Verybad {
  Verybad(Verybad&&) = delete;
};



int main (int argc, char *argv[]) {
  std::common_type<int,int> x;
  static_assert(is_nothrow_move_constructible<Good>::value, "fdas");
  static_assert(!is_nothrow_move_constructible<Bad>::value, "fdas");
  static_assert(!is_nothrow_move_constructible<Verybad>::value, "fdas");
  return 0;
}
