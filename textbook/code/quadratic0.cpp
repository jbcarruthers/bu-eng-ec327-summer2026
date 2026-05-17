// quadratic0.cpp -- EC327 Summer 2026
//
// Quadratic equation solver, version 0.
//   a x^2 + b x + c = 0
//
// Stage 0: get input and output working. Math will come next.
//
// Topics introduced:
//   - declaring variables of type double
//   - reading values from cin
//   - writing values to cout
//   - the std:: namespace
//   - the int main() shape, return 0

#include <iostream>

int main() {
  double a{};
  double b{};
  double c{};

  std::cout << "Please enter the coefficients a, b, c: ";
  std::cin >> a >> b >> c;

  std::cout << "You typed " << a << " " << b << " " << c << ".\n";
  std::cout << "(The roots are 3 and 5. Probably not, but we will fix that.)\n";

  return 0;
}
