#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

//operator new in realitity firstly calls function new and then calls constuctor. 
//You may overload only function new, so constuctor always will be called
[[nodiscard]] void* operator new(size_t n) {
  std::cout << n << " bytes allocated\n";
  return malloc(n);
}

void operator delete(void* ptr) noexcept { 
  free(ptr); 
}

[[nodiscard]] void* operator new[](size_t n) {
  std::cout << n << "bytes[] allocated\n";
  return malloc(n);
}

void operator delete[](void* ptr) noexcept { 
  std::cout << ptr << '\n';
  free(ptr); 
}


//any params
//call: int* ptr = new (5, "fdas") int(5);
void* operator new(size_t n, int a, const std::string& b) {
  std::cout << a << b << '\n';
  return malloc(n);
}


//you cannot new[] and delete[] with Base* ptr = new Derived[3]; - delete just doesn't know the size of  Derived
//So with polymorphic objects use just new and delete
struct S {
  void* operator new(size_t n) {
    std::cout << n << " operator new for s\n";
    return malloc(n);
  }
  void operator delete(void* ptr) {
    std::cout << "operator delete for s\n";
    free(ptr);
  }   
  void* operator new[](size_t n) {
    std::cout << n << " operator new[] for s\n";
    return malloc(n);
  }
  void operator delete[](void* ptr) {
    std::cout << "operator delete[] for s\n";
    free(ptr);
  }   
  S() {} 
  ~S() {}

private:
};


struct A {
  void* operator new(size_t n) {//hides ALL standard overloads
    return malloc(n);
  }
  void* operator new(size_t n, void* ptr) {}//...
  void operator delete(void* ptr) {
    // static_cast<A*>(ptr)->~A(); //Why did i put it here? - probably i meant destoring delete
    free(ptr);
  }
  
private:
  ~A() {std::cout << "dtor call";};
};

int main (int argc, char *argv[]) {
}
