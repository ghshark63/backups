#include <memory>

template <typename T>
struct default_deleter {
  void operator()(T* ptr) const { delete ptr; }
};

//There is Deleter, since we should not call delete ptr(examle: malloc - free, file_open - file_close)
template <typename T, typename Deleter = std::default_delete<T>>
class unique_ptr {
public:
  unique_ptr(T* ptr, Deleter del) : ptr(ptr), del(del) {};
  unique_ptr(T* ptr) : ptr(ptr) {}

  ~unique_ptr() {
    del(ptr);
  }

  unique_ptr(const unique_ptr&) = delete;
  unique_ptr& operator=(const unique_ptr&) = delete;

  unique_ptr(unique_ptr&& other) : ptr(other.ptr), del(std::move(other.del)) {
    other.ptr = nullptr;
  }

  unique_ptr& operator=(unique_ptr&& other) {
    if (this == &other) return *this;

    del(ptr);
    ptr = other.ptr;
    del = std::move(other.del);
    other.ptr = nullptr;

    return *this;
  }
  T& operator*() const { return *ptr; }
  T* operator->() const { return ptr; }
  T* get() const { return ptr; }
  void reset() {
    del(ptr);
    ptr = nullptr;
  }
private:
  T* ptr;
  [[no_unique_address]] Deleter del;
};

