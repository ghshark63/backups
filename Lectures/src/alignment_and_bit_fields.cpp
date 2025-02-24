#include <cstdlib>
#include <iostream>
#include <memory>


void foo(const int&) {
  struct alignas(alignof(long long)) Foo {
    int x[2];
  };

  alignas(32) char c[16];
  std::cout << alignof(decltype(c));

  void* ptr = aligned_alloc(4, 7);
}

//std:align
struct MyAllocator
{
    static constexpr int N = 5;
    char data[N];
    void* p;
    std::size_t sz;

    MyAllocator() : p(data), sz(N) {}

    template<typename T>
    T* aligned_alloc(std::size_t a = alignof(T))
    {
    //std::align - alignes given ptr with given alignment, decreses given heap_size by given sizeof
        if (std::align(a, sizeof(T), p, sz))
        {
            T* result = reinterpret_cast<T*>(p);
            p = (char*)p + sizeof(T);
            sz -= sizeof(T);
            return result;
        }
        return nullptr;
    }
};

struct alignas(2) bit_field {
  unsigned char a : 3;
  unsigned char b : 2;
  unsigned char   : 1;
  unsigned char c : 6;
};

