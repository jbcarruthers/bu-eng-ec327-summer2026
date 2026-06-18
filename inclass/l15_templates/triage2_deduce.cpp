// EC327 L15 activity -- TRIAGE SNIPPET 2 of 3.  THIS IS SUPPOSED TO FAIL.
// Build: g++ -std=c++20 -Wall -Wextra triage2_deduce.cpp -o t2
//
// Same triage routine. This one is a DEDUCTION error, not a missing operator:
// you called a template in a way that leaves the compiler unable to pick one T.
//
// Hint to check yourself: the wall says "deduced conflicting types for 'T'"
// (it sees an int for one argument and a double for the other).

#include <iostream>

template <typename T>
T max_of(T a, T b) {       // ONE type slot, shared by both parameters
  return (a > b) ? a : b;
}

int main() {
  int    a = 3;
  double b = 4.5;
  std::cout << max_of(a, b) << '\n';  // <-- T can't be both int AND double
  return 0;
}
