#include <iostream>
struct A {
  // If you uncomment it - the code will not print anything. 
  // The reason is that there is actualy "complete object destuctor" and 
  // "deleting object destuctor". 
  // Why? Because the class may override delete operator, so the 
  // compiler must call the appropriate one when holding the pointer
  // to base while the actual type is derived. 
  // Deleting destuctor does the same as the complete one(most likely calls the complete one) but in 
  // the end it calls proper delete operator.
  //
  // virtual ~A() {}
  virtual void bar() {std::cout << "Hello from A bar!\n";}
  virtual void foo() {std::cout << "Hello from A foo!\n";}


};
struct B : A {
  void bar() override {std::cout << "Hello from B bar!\n";}
  void foo() override {std::cout << "Hello from B foo!\n"; }
};

void callBar(A* obj) {
  using func_type = void(*)();

  long long vptr_address = *(long long*) obj;
  auto vptr = (long long*) vptr_address;

  long long func_address = *vptr;
  auto func = (func_type)func_address;

  func();
}
void callFoo(A* obj) {
  using func_type = void(*)();

  long long vptr_address = *(long long*) obj;
  auto vptr = (long long*) vptr_address;

  long long func_address = *(vptr + 1);
  auto func = (func_type)func_address;

  func();
}

int main (int argc, char *argv[]) {
  A* a = new A;
  callBar(a);
  callFoo(a);
  std::cout << "\n";
  a = new B;
  callBar(a);
  callFoo(a);

}
