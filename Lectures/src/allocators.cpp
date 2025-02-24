//container -> allocator_traits -> allocator -> new -> malloc -> os
#include <cstddef>
#include <memory>
#include <scoped_allocator>
#include <string>
#include <utility>
#include <vector>

//allocators_traits
template <typename Alloc>
struct allocator_traits {

  template <typename U, typename... Args>
  static void construct(Alloc& alloc, U* ptr, Args&&... args) {
    if constexpr (true /*Alloc has method construct*/) {
      alloc.construct(ptr, std::forward<Args>(args)...);
    } else { 
      new (ptr) U(std::forward<Args>(args)...);
    }
  }
  //destory function
  //
  //allocate + deallocate (for unifying)
  //
  //rebind_alloc metafunction
  //
  //propagete....
};



//simplified std::allocator implementation
template <typename T>
struct allocator {
  allocator() = default;
  template <typename U>
  allocator(allocator<U>) {}

  T* allocate(size_t count) {
    // return reinterpret_cast<T*>(new char[count * sizeof(T)]);
    return static_cast<T*>(operator new(count * sizeof(T)));
  }
  void deallocate(T* ptr, size_t) {
    // delete[] reinterpret_cast<char*>(ptr);
    operator delete(ptr);
  }

  //until c++ 20, predefined in allocators_traits
  // template <typename U, typename... Args>
  // void construct(U* ptr, const Args&... args) {
  //   new (ptr) U(args...);
  // }
  //
  // template<typename U>
  // void destroy(U* ptr) {
  //   ptr->~U();
  // }


  template <typename U>
  struct rebind {
    using other = allocator<U>;
  };
};

//example with list
template <typename T, typename Alloc = std::allocator<T>>
class list {
  struct BaseNode {
    BaseNode* prev;
    BaseNode* next;
  };

  struct Node : BaseNode {
    T value;
  };

  BaseNode fake_node;
  size_t count;
  typename Alloc::template rebind<Node>::other alloc;

  list(const Alloc& alloc) : fake_node(), count(), alloc(alloc) {}
};


//PoolAllocator - allocates large amount of memory and alloc from it, so there's only 1 call to new operator
//StackAllocator - the same as pool one, but takes a stack array from user and allocates from it
//FreeListAllocator


//Allocator-aware containers 
//
//Motivation: what should container do with allocator while copying(like create new pull or not, create or copy allocator)
//std::allocator_traits<allocator_type>::select_on_container_copy_construction
//std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value




//scoped allocator

template <typename T>
using MyAlloc = std::allocator<T>;

void scoped_alloc_problem() {
  using MyString = std::basic_string<char, std::char_traits<char>, MyAlloc<char>>;

  MyAlloc<MyString> alloc;
  
  //Problem - every string has its own istance of allocator, meanwhile we want they share the same one
  // std::vector<MyString, MyAlloc<MyString>> v(alloc);

  std::vector<MyString, std::scoped_allocator_adaptor<MyAlloc<MyString>>> v(alloc);

  v.push_back("fda");
  v.push_back("aaa");
}

//partial scoped allocator idea
template <typename Alloc>
struct scoped_allocator_adaptor {
  Alloc alloc;

  template <typename T, typename... Args>
  void construct(T* ptr, const Args&... args) {
    if constexpr (std::uses_allocator_v<T, Alloc>) {
      using inner_alloc = typename T::allocator_type;
      alloc.construct(ptr, args..., inner_alloc(alloc));
    } else {
      alloc.construct(ptr,args...);
    }
  }

  template <typename T>
  void destroy(T* ptr) {
    alloc.destroy(ptr);
  }
};


