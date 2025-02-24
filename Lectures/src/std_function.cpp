//See 55'th lecture
//Additionally see SMALL OBJECT optimization 
#include <functional>
#include <memory>
#include <vector>


int multiply(int a, int b) { return a * b; }

struct A {
  int add(int a, int b) { return a + b; }
  int field;
};
struct AddOne {
  int operator()(int, int) { return 0; }

};

struct NotCopy {
std::unique_ptr<int> p;
  int operator()(int, int) {
    return 0;
  }
};

int main (int argc, char *argv[]) {
  //Important: std;:function cann't be used with not copiable types.
  std::function<int(int, int)> f = multiply;

  // decltype(f) f2 = NotCopy();//CE
  f = AddOne();
  f = [](int, int) { return 0; };


  std::function<int(A&, int, int)> x = &A::add;
  std::function<int(A&)> xx = &A::field;


  return 0;
}
