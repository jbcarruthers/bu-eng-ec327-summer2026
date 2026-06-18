// EC327 L17 -- leap.hpp
// The "unit under test": one small, pure function with real edge cases.
//
// Gregorian leap-year rule (the classic test-case-generation example):
//   - divisible by 4    -> leap          (2024)
//   - EXCEPT divisible by 100 -> not leap (1900)   <- the trap
//   - EXCEPT divisible by 400 -> leap      (2000)  <- the trap's trap
//
// Pure (no I/O, no globals): same input always gives same output.  That is
// exactly what makes a function easy to unit-test -- arrange an input, call,
// assert on the return value.  Nothing to mock.

#ifndef LEAP_HPP
#define LEAP_HPP

inline bool is_leap_year(int year) {
  if (year % 400 == 0)
    return true;
  if (year % 100 == 0)
    return false;
  return year % 4 == 0;
}

#endif  // LEAP_HPP
