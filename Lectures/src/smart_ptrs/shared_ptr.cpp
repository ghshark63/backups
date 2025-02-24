#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

//Additionally see std::static_pointer_cast, const_pointer_cast etc...


//Должны  поддеражать 2 семантики: создание через make_shared and constructor.
//Недостаток первого - обьект держиться пока не удаляются ввсе weak_ptr. То есть в деструкторе shared_ptr 
//вызывается просто деструктор обьеткта но память не освобождается - минус. Однако при создании экономим вызов к ос через new - плюс
//
//Недостаток второго - 2 вызова new.  Но полностью удаляем обьект при удалении последнего shared_ptr
//
//Идея - в 1 случае хранить указатель на обьект, но занулить контролблок - счетки лежат перед обьектом(контрблок с обьектом)ю
//Во 2 сдучае хранить оба указателя. 
// (для make shared)В декструкторе проверить shared_count == 0 ? если да то вызвать деструктор обьекта.
// Потом проверить weak_count == 0 ? если да то удалить контрол блок 
template <typename T>
class shared_ptr {
  //We need a another template c-tor for case:
  //auto p = std::make_shared<Derived>();
  //shared_ptr<Base> pp = p;

  //Problem - we assign 2 ptrs of the same with different 
  //Deleteers. Question - how to dynamically swap the Deleter??
  //Answer - type_erasure
private:
  struct BaseControlBlock {
    size_t shared_count;
    size_t weak_count;
    virtual ~BaseControlBlock() = default;

  };
  
  template <typename U, typename Deleter, typename Alloc>
  struct RegularControlBlock : BaseControlBlock {
    Alloc alloc;
    Deleter del;
  };

  template <typename U, typename Alloc>
  struct ControlBlockMakeShared : BaseControlBlock {
    //This made for multiple inhertance like mother father and son. 
    //Case: shared<father> p = make_shared<son>();
    //Queastion: how to define shift for shred_cout and weak_count??
    //Ans: shared stores T* while controlblock stores any U(like a real object)
    U value;
    Alloc alloc;
  };

  //May points not to a real object but a parent!!
  T* ptr;
  //stack size_t and static size_t are not applicable(consider assignment operator)
  BaseControlBlock* count;


  template <typename U, typename... Args>
  friend shared_ptr<U> make_shared(Args&&...);

  //for make_shared
  shared_ptr(BaseControlBlock* cp);
public:

  template <typename U>
  class enable_shared_from_this;

  shared_ptr(T* ptr) 
      : ptr(ptr), 
        count(new size_t(1)) {
    if constexpr (std::is_base_of_v<enable_shared_from_this<T>, T>) {
      ptr->sptr = *this;
    }
  }
  shared_ptr(const shared_ptr& other)
      : ptr(other.ptr),
        count(count) {
    ++*count;
  }
  
  ~shared_ptr() {
    if (!count) return;
    --*count;
    if (!*count) {
      delete count;
      delete ptr;
    }
  }
        


};


//make_shared, make_unique
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
template <typename T, typename... Args>
shared_ptr<T> make_shared(Args&&... args) {
  auto* p = new typename shared_ptr<T>::BaseControlBlock{T(std::forward<Args>(args)...), 1};
  // return share;
}


template <typename T, typename Alloc, typename... Args>
shared_ptr<T> allocate_shared(const Alloc& alloc, Args&&... args) {
  using BlockAlloc = std::allocator_traits<Alloc> 
        ::template rebind_alloc<shared_ptr<T>::template ControlBlockMakeShared<T, Alloc>>;
  BlockAlloc ba = alloc;
  auto* ptr = ba.allocate(1);
  //ptr sharedcnt weakcnt params alloc
  ba.construct(ptr, 1, 0, std::forward<Args>(args)..., alloc);
  // return shared_ptr(ptr...); Todo
}
//Weak ptr
//
//

//enable shared from this.
//CRTP

template <typename T>
struct enable_shared_from_this {
  //shared_ptr is not suitable heere - cyclic dependency
  mutable std::weak_ptr<T> sptr;

  enable_shared_from_this() { }

  shared_ptr<T> shared_from_this() const {
    return sptr;
  }


  template <typename U>
  friend class shared_ptr;
};


struct S : private std::enable_shared_from_this<S> {
  std::shared_ptr<S> get() {
    return shared_from_this();
  }
};

int main (int argc, char *argv[]) {
  // auto p = std::shared_ptr<int>(new int(5)); //bad style, we should not operate with c-style pointers. 
  // Also calling a constructor involves 2 new calls(we manually allocate obj + c-tor calls new for counter).
  // Meanwhile make_shared calls only once.
  auto p = std::make_shared<int>(5);
  auto p2 = p;
  auto p3 = p2; // p, p2, p3 ->5


  
  return 0;
}

