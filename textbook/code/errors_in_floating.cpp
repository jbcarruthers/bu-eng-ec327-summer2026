// errors_in_floating.cpp -- EC327 Summer 2026
//
// Floating-point types do not store most values exactly. This program
// computes 1/3 in float, double, and long double and shows how much
// they differ. setprecision(22) asks cout to print enough digits to
// make the error visible.
//
// Watch num_ld3: 1.0/3 is computed in *double* and only then widened to
// long double, so it carries double's rounding error, not long double's.

#include <iomanip>
#include <iostream>

int main() {
  float num_f = 1.0F / 3;
  double num_d = 1.0 / 3;

  // long double is wider than double on most platforms -- we use it for a
  // third precision level. (clang-tidy's google-runtime-float flags it as
  // non-portable, which is true; here it is deliberate and suppressed.)
  // NOLINTBEGIN(google-runtime-float)
  long double num_ld = 1;
  num_ld /= 3;

  long double num_ld2 = static_cast<long double>(1.0) / 3;
  long double num_ld3 = 1.0 / 3;  // double division, widened afterward
  // NOLINTEND(google-runtime-float)

  std::cout << std::setprecision(22);
  std::cout << "float       1/3 = " << num_f << '\n';
  std::cout << "double      1/3 = " << num_d << '\n';
  std::cout << "long double 1/3 = " << num_ld << '\n';

  std::cout << "double      - float       = " << num_d - num_f << '\n';
  std::cout << "long double - double      = " << num_ld - num_d << '\n';
  std::cout << "long double - num_ld2     = " << num_ld - num_ld2 << '\n';
  std::cout << "long double - num_ld3     = " << num_ld - num_ld3 << '\n';

  return 0;
}
