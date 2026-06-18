// EC327 L17 -- roman.hpp
// A SECOND unit, used for the "a test caught a real bug" beat (and the TDD
// activity).  roman_to_int("XIV") should be 14.
//
// This implementation has a DELIBERATE bug: it simply adds up the value of
// every symbol and never handles SUBTRACTIVE notation (IV = 4, IX = 9,
// XL = 40, ...).  So roman_to_int("IV") returns 6, not 4.
//
// The point of the demo: the bug is invisible to a human skim ("looks
// reasonable") but the test suite in test_roman.cpp catches it on the first
// run.  Tests turn "looks right" into "is right -- here's the proof."
// The fix is in SOLUTIONS.md (instructor-only).

#ifndef ROMAN_HPP
#define ROMAN_HPP

#include <string>

inline int symbol_value(char c) {
  switch (c) {
    case 'I':
      return 1;
    case 'V':
      return 5;
    case 'X':
      return 10;
    case 'L':
      return 50;
    case 'C':
      return 100;
    case 'D':
      return 500;
    case 'M':
      return 1000;
    default:
      return 0;
  }
}

inline int roman_to_int(const std::string& s) {
  int total = 0;
  for (char c : s) {
    total += symbol_value(c);  // BUG: ignores subtractive pairs (IV, IX, ...)
  }
  return total;
}

#endif  // ROMAN_HPP
