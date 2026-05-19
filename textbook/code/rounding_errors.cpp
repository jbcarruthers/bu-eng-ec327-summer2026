// rounding_errors.cpp -- EC327 Summer 2026
//
// Algebraically, 1 - 3 * (i / 3) / i is exactly 0 for every i. In double
// arithmetic it usually is not: 1.0/3 cannot be stored exactly, so the
// rounding error survives the round trip. This loop prints every i from
// 1 to 99 for which the computed result is not 0.

#include <iostream>

int main() {
  double one_third = 1.0 / 3;

  for (int i = 1; i < 100; i++) {
    double zeroish = 1.0 - 3.0 * (i * one_third) / i;
    if (zeroish != 0) {
      std::cout << "i = " << i << "   result = " << zeroish << '\n';
    }
  }

  return 0;
}
