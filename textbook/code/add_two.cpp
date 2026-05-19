// add_two.cpp -- EC327 Summer 2026
//
// The smallest "read input, compute, print" program: add two integers
// typed by the user.

#include <iostream>

int main() {
  int x{};
  int y{};

  std::cin >> x >> y;

  std::cout << x + y << '\n';

  return 0;
}
