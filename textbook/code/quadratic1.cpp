// quadratic1.cpp -- EC327 Summer 2026
//
// Quadratic equation solver, version 1.
//
// Stage 1: actually compute real roots. Ignore the complex case for now;
// if the discriminant is negative we will get NaN from std::sqrt and the
// output will look wrong. That is on purpose -- we will fix it in stage 2.
//
// New topics introduced:
//   - #include <cmath> and std::sqrt
//   - arithmetic operators and operator precedence
//   - intermediate variables to avoid re-computing an expression

#include <iostream>
#include <cmath>

int main() {
  double a{};
  double b{};
  double c{};

  std::cout << "Please enter the coefficients a, b, c: ";
  std::cin >> a >> b >> c;

  double disc = b * b - 4 * a * c;

  double root1 = (-b + std::sqrt(disc)) / (2 * a);
  double root2 = (-b - std::sqrt(disc)) / (2 * a);

  std::cout << "The roots are " << root1 << " and " << root2 << ".\n";

  return 0;
}
