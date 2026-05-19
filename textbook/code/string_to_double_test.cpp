// string_to_double_test.cpp -- EC327 Summer 2026
//
// Convert text to a number with std::stod ("string to double"), then
// use the result in arithmetic.

#include <iostream>
#include <string>

int main() {
  std::string s{"123.4"};

  double d = std::stod(s);

  std::cout << d * 2 << '\n';

  return 0;
}
