// compare_two.cpp -- EC327 Summer 2026
//
// Read two integers and report how they compare, using three
// independent if-statements -- one per relational operator.

#include <iostream>

int main() {
  int j{};
  int k{};

  std::cout << "Type in two values and I will compare them\n";
  std::cin >> j >> k;

  if (j == k) {
    std::cout << "first equals second\n";
  }
  if (j < k) {
    std::cout << "first less than second\n";
  }
  if (j > k) {
    std::cout << "first greater than second\n";
  }

  return 0;
}
