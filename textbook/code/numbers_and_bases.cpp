// numbers_and_bases.cpp -- EC327 Summer 2026
//
// Integer division and modulo, and how C++ writes integer literals in
// other bases: 0x... for hexadecimal, 0b... for binary. The final loop
// peels off the base-16 digits of a number using % and /.

#include <iostream>

int main() {
  int x{123};
  int d{10};

  std::cout << "x = " << x << '\n';
  std::cout << "d = " << d << '\n';
  std::cout << "x / d = " << x / d << '\n';  // integer division
  std::cout << "x % d = " << x % d << '\n';  // remainder (modulo)

  double z = 1.0 * x / d;  // multiply by 1.0 to force real division
  std::cout << "1.0 * x / d = " << z << '\n';

  // The same kind of value, written in two other bases.
  int hex_literal{0x12345};
  int bin_literal{0b1011101111};
  std::cout << "0x12345      = " << hex_literal << '\n';
  std::cout << "0b1011101111 = " << bin_literal << '\n';

  // Peel off the hexadecimal digits of a number, least significant first.
  x = bin_literal;
  while (x > 0) {
    d = x % 16;
    x = x / 16;
    std::cout << "digit " << d << ", remaining " << x << '\n';
  }

  return 0;
}
