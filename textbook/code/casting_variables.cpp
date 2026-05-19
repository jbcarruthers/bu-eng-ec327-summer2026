// casting_variables.cpp -- EC327 Summer 2026
//
// A catalog of ways to convert between numeric types, and the classic
// integer-division trap. EC327 prefers static_cast: it is searchable,
// explicit, and the one form clang-tidy does not flag -- the other three
// conversions below carry a NOLINT only because we show them on purpose.
//
// Adapted from a casting example by D. Densmore.

#include <iostream>

int main() {
  double d{1.7};

  // Four ways to narrow a double to an int. All four discard the 0.7.
  // NOLINTNEXTLINE(bugprone-narrowing-conversions) -- implicit, easy to miss
  int implicit = d;
  // NOLINTNEXTLINE(google-readability-casting) -- C-style cast, discouraged
  int c_style = (int)d;
  // NOLINTNEXTLINE(google-readability-casting) -- function-style cast
  int functional = int(d);
  int checked = static_cast<int>(d);  // preferred

  std::cout << implicit << ' ' << c_style << ' ' << functional << ' ' << checked
            << '\n';

  // Widening (int -> double) is always safe; being explicit shows intent.
  int n{4};
  double widened = static_cast<double>(n);
  std::cout << widened << '\n';

  // The integer-division trap: a / b is computed in int *first*.
  int a{1};
  int b{2};
  // NOLINTNEXTLINE(bugprone-integer-division) -- a/b is 0, shown on purpose
  std::cout << 1.0 * (a / b) << '\n';
  std::cout << 1.0 * a / b << '\n';                 // 0.5: a is double first
  std::cout << static_cast<double>(a) / b << '\n';  // 0.5: clearest

  return 0;
}
