//since c++17
//This is generally the convenient interface for UNION

//see Lectures for variant and visit impl.
#include <iostream>
#include <string>
#include <variant>


int main (int argc, char *argv[]) {
  std::variant<int, double, std::string> v = 5;
  std::variant<int, int> vv ;
  // vv = 5;

  std::cout << std::get<int>(v) << '\n';

  v = "fdas";

  std::cout << std::get<std::string>(v) << '\n';

  v = 2.14;

  std::cout << std::get<double>(v) << '\n';

  std::variant<int ,int> b;

  std::cout << "\nVisit:\n";
  std::visit([](const auto& item) { std::cout << item; }, v);
  
  return 0;
}

