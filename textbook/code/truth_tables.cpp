// truth_tables.cpp -- EC327 Summer 2026
//
// Truth tables for the boolean operators, built by looping over every
// combination of operands. EC327 distributes code with the spelled-out
// keywords `and`, `or`, `not` (not && || !). std::boolalpha prints the
// results as true/false instead of 1/0.

#include <iostream>

int main() {
  std::cout << std::boolalpha;

  for (bool p : {false, true}) {
    for (bool q : {false, true}) {
      std::cout << p << " or " << q << " is " << (p or q) << '\n';
    }
  }

  for (bool p : {false, true}) {
    for (bool q : {false, true}) {
      std::cout << p << " and " << q << " is " << (p and q) << '\n';
    }
  }

  for (bool p : {false, true}) {
    std::cout << "not " << p << " is " << (not p) << '\n';
  }

  return 0;
}
