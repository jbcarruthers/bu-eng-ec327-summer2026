// divide_remain.cpp -- EC327 Summer 2026
//
// Integer division (/) and remainder (%). The key identity, for any
// integers i and j with j != 0:
//   (i / j) * j + (i % j) == i

#include <iostream>

int main() {
  int i{};
  int j{};

  std::cin >> i >> j;

  int divres = i / j;
  int modres = i % j;

  std::cout << i << ' ' << j << '\n';
  std::cout << divres << ' ' << modres << ' ' << divres * j + modres << '\n';

  return 0;
}
