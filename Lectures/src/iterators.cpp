#include <algorithm>
#include <fstream>
#include <cstddef>
#include <iomanip>
#include <ios>
#include <iostream>
#include <istream>
#include <iterator>
#include <optional>
#include <sstream>
#include <vector>
#include <type_traits>

//sample of implementation of std::distance with/without if constexpr
namespace distance {
  template <typename Iterator>
  typename std::iterator_traits<Iterator>::difference_type
  distance (Iterator first, Iterator last) {

    if constexpr (std::is_base_of_v<
                  std::random_access_iterator_tag,
                  typename std::iterator_traits<Iterator>::iterator_category
                  >) {
      return last - first;
    }
    
    int i = 0;
    for (; first != last; ++i, ++first);
    return i;
  }
  
  //pre c++11 impl:
  template <typename Iterator>
  auto distance_11(Iterator first, Iterator last) {
    return helper(first, last, typename std::iterator_traits<Iterator>::iterator_category());
  }
  
  template <typename Iterator, typename T>
  auto helper(Iterator first, Iterator last, T) {
    int i = 0;
    for (; first != last; ++i, ++first);
    return i;
  }

  template <typename Iterator>
  auto helper(Iterator first, Iterator last, std::random_access_iterator_tag) {
    return last - first;
  }
  
} //distance


//smaple of iterator implementation
template <typename T>
class vector {
  // class iterator {
  // private:
  //   T* ptr_;
  // public:
  //   iterator() : ptr_(nullptr) {}
  //   iterator(T* ptr) : ptr_(ptr) {}
  //   iterator(const iterator&) = default;
  //   iterator& operator=(const iterator&) = default;
  //
  //   T& operator*() const { return *ptr_; }
  //
  //   iterator& operator++() {
  //     ++ptr_;
  //     return *this;
  //   }
  //
  //   iterator operator++(int) {
  //     iterator copy = *this;
  //     ++ptr_;
  //     return copy;
  //   }
  //
  //   T* operator->() const { return ptr_; }
  // };
  //
  // //Copypast isnot desired, so make base iterator
  // class const_iterator {
  // private:
  //   const T* ptr_;
  // public:
  //   const_iterator() : ptr_(nullptr) {}
  //   const_iterator(T* ptr) : ptr_(ptr) {}
  //   const_iterator(const const_iterator&) = default;
  //   const_iterator& operator=(const const_iterator&) = default;
  //
  //   const T& operator*() const { return *ptr_; }
  //   const T* operator->() const { return ptr_; }
  //   const_iterator& operator++() {
  //     ++ptr_;
  //     return *this;
  //   }
  //
  //   const_iterator operator++(int) {
  //     const_iterator copy = *this;
  //     ++ptr_;
  //     return copy;
  //   }
  //
  // };

private:
  template <bool IsConst>
  class BaseIterator {
    friend class vector<T>;
  public:
    using value_type = std::conditional_t<IsConst, const T, T>;
    using reference = std::conditional_t<IsConst, const T&, T&>;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = int;
  

    BaseIterator(const BaseIterator&) = default;
    BaseIterator& operator=(const BaseIterator&) = default;
    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }
  
    BaseIterator& operator++() {
      ++ptr_;
      return *this;
    }
  
    BaseIterator operator++(int) {
      BaseIterator copy = *this;
      ++ptr_;
      return copy;
    }
  
    //want to cast: non-const -> const
    operator BaseIterator<true>() const {
      return ptr_;
    }
    //or:
    //BaseIterator(BaseIterator<false>& other) : ptr_(other.ptr_) {}
    //But there are problems with accesing private members;
  private:
    BaseIterator(pointer* ptr) : ptr_(ptr) {}

    pointer ptr_;
  };

  T* arr_;
  size_t sz_;
  size_t cap_;

public:
  using iterator = BaseIterator<false>;
  using const_iterator = BaseIterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() {
    return iterator(arr_);
  }

  iterator end() {
    return iterator(arr_ + sz_);
  }

  const_iterator begin() const {
    return const_iterator(arr_);
  }

  const_iterator end() const {
    return const_iterator(arr_ + sz_);
  }

  const_iterator cbegin() const {
    return const_iterator(arr_);
  }

  const_iterator cend() const {
    return const_iterator(arr_ + sz_);
  }
  
  reverse_iterator rbegin() {
    return reverse_iterator(end());
  }

  reverse_iterator rend() {
    return reverse_iterator(begin());
  }
  void reserve();
  void push_back(); 
  
};


void sstreams() {
  std::istringstream in("a b c");
  int x;
  in >> x;
  in.str();
  std::ostringstream out("fda");
  out << x;

  std::cout << out.str();
}

//output iterators
void back_insert() {
  int a[10] = {1, 2, 3};
  std::vector<int> arr;
  int* b = nullptr;
  // std::copy(a, a + 10, std::back_insert_iterator<std::vector<int>>(arr));
  //better:
  std::copy(a, a + 10, back_inserter(arr));

}

//There are also front_insert_iterator/front_inserter.
//Also there is just inserter
template <typename Container>
class back_insert_iterator {
private:
  Container& container;
public:
  back_insert_iterator(Container& cont) : container(cont) {}
  back_insert_iterator& operator=(const typename Container::value_type& value) {
    container.push_back(value);
    return *this;
  }
  back_insert_iterator& operator++() {
    return *this;
  }
  back_insert_iterator operator++(int) {
    return *this;
  }
  back_insert_iterator& operator*() {
    return *this;
  }
};

template <typename Container>
back_insert_iterator<Container> back_inserter(Container& container) {
  return container;
}



//since c++17
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
  T& operator*() {
    return reinterpret_cast<T&>(*value);
  }
  T& value_or(T& other) {
    return is_initialized ? reinterpret_cast<T&>(*value) : other;
  }
private:
  char value[sizeof(T)];
  bool is_initialized = false;
};
//empty optional
//https://stackoverflow.com/questions/49617785/why-is-stdnullopt-t-part-of-the-c-standard
struct nullopt_t {};
nullopt_t nullopt;

//stream iterators
//Istream_iterator is a "iterator" like wrapper for std::istream
//The same about std::ostream_iterator and std::ostream(ostream is implemented like back_insert_iterator)
//poor general impl
template <typename T>
class istream_iterator {
public:
  using pointer = T*;
  using reference = T&;
  using value_type = T;
  using difference_type = int;
  using iterator_category = std::input_iterator_tag;
  istream_iterator() = default;
  istream_iterator(std::istream& in) : in_(&in) {
    in >> *value;
  }
  istream_iterator& operator++() {
    if (!(*in_ >> value)) {
      *this = istream_iterator();
    }
    return *this;
  }
  //since this is an input iterator nor forward, so copying is prohibited
  istream_iterator& operator++(int) = delete;

  reference operator*() { return *value;}
private:
  std::istream* in_ = nullptr;
  //seems optional is unneccessary
  std::optional<T> value;
};

void stream_iter() {
  istream_iterator<int> it(std::cin);
  //Ways to read a vector from istream_iterator 
  std::vector<int> v(10);
  std::copy(it, istream_iterator<int>(), v.begin());

  std::vector<int> vv;
  std::copy(it, istream_iterator<int>(), std::back_inserter(vv));

  std::vector<int> vvv(it, istream_iterator<int>());

  //input from file and output to stdout
  std::ifstream infile("input.txt");
  std::istream_iterator<int> itt(infile);
  std::copy(itt, std::istream_iterator<int>(), 
            std::ostream_iterator<int>(std::cout, " "));
  //print squares
  std::transform(itt, std::istream_iterator<int>(), 
                 std::ostream_iterator<int>(std::cout, " "), 
                 [](int x) { return x * x;});
}

//std::move_iterator adaptor - dereferencing returns a rvalue reference

//stream manipulators
void stream_manipulators() {
  std::cout << std::hex;

  std::cin >> std::noskipws;
  std::cout << std::boolalpha;
  std::cout << std::setprecision(5);
}
