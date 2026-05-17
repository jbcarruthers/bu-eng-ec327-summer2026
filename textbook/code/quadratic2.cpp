// quadratic2.cpp -- EC327 Summer 2026
//
// Quadratic equation solver, version 2.
//
// Stage 2: detect the complex-root case and refuse to compute it.
// We do not yet *handle* it; we just notice it and exit with a
// non-zero status code so the caller knows we did not produce roots.
//
// New topics introduced:
//   - comparison operators (<)
//   - if-statements (selection)
//   - meaningful exit codes (return 1 vs return 0)

#include <iostream>
#include <cmath>

int main() {
  double a{};
  double b{};
  double c{};

  std::cout << "Please enter the coefficients a, b, c: ";
  std::cin >> a >> b >> c;

  double disc = b * b - 4 * a * c;

  if (disc < 0) {
    std::cout << "Sorry, the roots are complex. Try stage 3 of this program.\n";
    return 1;
  }

  double root1 = (-b + std::sqrt(disc)) / (2 * a);
  double root2 = (-b - std::sqrt(disc)) / (2 * a);

  std::cout << "The roots are " << root1 << " and " << root2 << ".\n";

  return 0;
}
