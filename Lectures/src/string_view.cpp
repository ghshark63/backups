//std::string_view - allows to observe the real string, not owning it. Thus, every operation in view is constant
//std::span - generalization of view for any type(like containter)
//std::ranges::views .....

#include <string_view>
#include <string>
int main (int argc, char *argv[]) {
  std::string_view s = "fdas";
  std::string x;
  return 0;
}

