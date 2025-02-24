#include <memory>
#include <cstddef>
#include <utility>


template <typename T, typename Alloc = std::allocator<T>>
class vector {

  T* arr_;
  size_t size_;
  size_t capacity_;
  Alloc alloc_;//takes 8 bytes, insterad of 0
  //since c++ 20  instead of empty base optimization
  // [[no_unique_address]] Alloc alloc;
  
  using AllocTraits = std::allocator_traits<Alloc>;
public:
  //Awfull reserve
  void res(int newcap) {
    T* newarr = new T[newcap];
    memcpy(newarr, arr_, size_);
    delete[] arr_;
    arr_ = newarr;
    capacity_ = newcap;
  }

  //good reserve
  void reserve(size_t new_capacity) {
    //Attention. Consider v.push_back(v[5]) - firt we move objects, when copy from dangling reference. 
    //That is why we must: first - push element, then move others

    if (new_capacity <= capacity_) return;

    
    //T* new_arr = new T[new_capacity]; 1 - T may not be able to default construct
    T* new_arr = nullptr;
    size_t index = 0;

    //Strong exception gurantee; just add here try - catch
    try {

      // new_arr = reinterpret_cast<T*>(new char[new_capacity * sizeof(T)]);
      new_arr = AllocTraits::allocate(alloc_, new_capacity);

      //memcpy(new_arr, arr_, size_); Can not memcpy due to the class may contain the reference or pointer to it's member:)
      for (; index < size_; ++index) {
        //new_arr[index] = arr_[index]; 2 - Try to call assign operator of not constructed obj(Now there are only raw bytes)

        // new (new_arr + index) T(arr_[index]); //placement new
        // AllocTraits::construct(alloc_, new_arr + index, arr_[index]); //bad - there is a coping here
        AllocTraits::construct(alloc_, new_arr + index, std::move_if_noexcept(arr_[index]));//with simple std::move - move 
        //constructor may throw and we miss exception safety
      }
    } catch(...) {
      for (size_t newindex = 0; newindex < index; ++newindex) {
        // (new_arr + newindex)->~T();
        AllocTraits::destroy(alloc_, new_arr + newindex);
      }
      AllocTraits::dealocate(alloc_, new_arr, new_capacity);
      // delete[] reinterpret_cast<char*>(new_arr);
      throw;
    }

    //delete[] arr_; properly dealocates neccessery memory, but the problem with calling correct number of destructors

    for (size_t index = 0; index < size_; ++index) {
      // (arr_ + index)->~T();
      alloc_.destroy(arr_ + index);
    }
    // delete[] reinterpret_cast<char*>(arr_);
    alloc_.dealocate(arr_, capacity_);

    arr_ = new_arr;
    capacity_ = new_capacity;
  }




  // void push_back(const T& value) {
  //   if (size_ == capacity_) {
  //     //The last element may throw, So there is should be reserve_and_push instead of reserve
  //     reserve(capacity_ > 0 ? capacity_ * 2 : 1);
  //   }
  //   AllocTraits::construct(alloc_, arr_ + size_, value);
  //   or AllocTraits::construct(alloc_, arr_ + size_, std::move(value));  in the overload push_back(T&&)
  //   ++size_;
  // }

  void push_back(const T& value) {
    emplace_back(value);
  }
  void push_back(T&& value) {
    emplace_back(std::move(value));
  }

  //Idea: pass by value and move.
  //void push_back(T value) {
  //emplace_back(std::move(value));
  //}

  template <typename... Args>
  void emplace_back(Args&&... args) {
    if (size_ == capacity_) {
      //The last element may throw, So there is should be reserve_and_push instead of reserve
      reserve(capacity_ > 0 ? capacity_ * 2 : 1);
    }
    AllocTraits::construct(alloc_, arr_ + size_, std::forward<Args>(args)...);
    ++size_;

  }

  //the same semantics since c++20
  // void emplace_back(auto&&... args) {
  //   if (size_ == capacity_) {
  //     //The last element may throw, So there is should be reserve_and_push instead of reserve
  //     reserve(capacity_ > 0 ? capacity_ * 2 : 1);
  //   }
  //
  //   forward<decltype(args)> - reference collapsing && = &&
  //   AllocTraits::construct(alloc_, arr_ + size_, std::forward<decltype(args)>(args)...);  
  //
  //   ++size_;
  //
  // }



  //some constructors with allocators
  vector() noexcept(noexcept(Alloc()));
  explicit vector(const Alloc&) noexcept;

  vector& operator=(const vector& other) {
    if constexpr (AllocTraits::propogate_on_container_copy_assignment::value) {

    }
    
    Alloc newalloc = AllocTraits::propogate_on_container_copy_assignment::value
      ? other.alloc_ : alloc_;

    T* newarr = AllocTraits::allocate(newalloc, other.capacity_);
    size_t i = 0;
    try {
      for (; i < other.size_; ++i) {
        AllocTraits::construct(newalloc, newarr + i, other[i]);
      }
    } catch(...) {
      for (size_t j = 0; j < i; ++j) {
         AllocTraits::destroy(newalloc, newarr + j);
      }
      AllocTraits::deallocate(newalloc, newarr, other.capacity_);
      throw;
    }

    for (size_t i = 0; i < size_; ++i) {
      AllocTraits::destroy(alloc_, arr_ + i);
    }
    AllocTraits::deallocate(alloc_, arr_, capacity_);

    arr_ = newarr;
    size_ = other.size_;
    capacity_ = other.capacity_;
    alloc_ = newalloc; //nothrow
  }
};

