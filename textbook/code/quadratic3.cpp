// quadratic3.cpp -- EC327 Summer 2026
//
// Quadratic equation solver, version 3.
//
// Stage 3: correctly handle all three cases:
//   - discriminant > 0 : two real roots
//   - discriminant = 0 : one repeated real root
//   - discriminant < 0 : two complex-conjugate roots
//
// New topics introduced:
//   - if / else if / else (three-way selection)
//   - the and / or / not keyword forms of the logical operators
//     (this course writes the keyword spellings as the primary form;
//     && || ! mean the same thing, and you must be able to read both)
//   - more careful output formatting

#include <iostream>
#include <cmath>

int main() {
  double a{};
  double b{};
  double c{};

  std::cout << "Please enter the coefficients a, b, c: ";
  std::cin >> a >> b >> c;

  if (a == 0 and b == 0) {
    std::cout << "a and b are both zero; this is not a quadratic.\n";
    return 1;
  }

  if (a == 0) {
    // Degenerate linear case: bx + c = 0  =>  x = -c/b.
    std::cout << "Linear (a is zero). The root is " << -c / b << ".\n";
    return 0;
  }

  double disc = b * b - 4 * a * c;

  if (disc > 0) {
    double root1 = (-b + std::sqrt(disc)) / (2 * a);
    double root2 = (-b - std::sqrt(disc)) / (2 * a);
    std::cout << "Two real roots: " << root1 << " and " << root2 << ".\n";
  } else if (disc == 0) {
    double root = -b / (2 * a);
    std::cout << "One repeated real root: " << root << ".\n";
  } else {
    double real_part = -b / (2 * a);
    double imag_part = std::sqrt(-disc) / (2 * a);
    std::cout << "Two complex roots: "
              << real_part << " + " << imag_part << "i and "
              << real_part << " - " << imag_part << "i.\n";
  }

  return 0;
}
