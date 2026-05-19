// add_bools.cpp -- EC327 Summer 2026
//
// What happens when you add bool values? Each bool promotes to int
// (false -> 0, true -> 1), so bool + bool is ordinary integer addition.

#include <iostream>

int main() {
  bool a{true};
  bool b{false};

  std::cout << b + b << '\n';  // 0
  std::cout << a + b << '\n';  // 1
  std::cout << a + a << '\n';  // 2

  return 0;
}
