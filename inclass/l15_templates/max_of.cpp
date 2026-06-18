// EC327 L15 demo -- one template, every type.
// Build: g++ -std=c++20 -Wall -Wextra max_of.cpp -o max_of && ./max_of
//
// The whole idea of a function template in five lines: declare a type slot
// `T`, use it like any type, and let the compiler stamp out a separate
// function for each T you actually call with.  The ONLY requirement on T here
// is that it supports `>` -- that implicit requirement is the seed of every
// template error you'll triage in the activity.

#include <iostream>
#include <string>

template <typename T>
T max_of(T a, T b) {
  return (a > b) ? a : b;
}

int main() {
  std::cout << max_of(3, 9) << '\n';                              // T = int
  std::cout << max_of(2.5, 1.5) << '\n';                          // T = double
  std::cout << max_of(std::string{"apple"},
                      std::string{"banana"}) << '\n';             // T = string
  // No max_of<char*> etc. is generated -- only the three above exist.
}
