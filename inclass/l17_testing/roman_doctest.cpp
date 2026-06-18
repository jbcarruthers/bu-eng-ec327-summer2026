// EC327 L17 -- roman_doctest.cpp
// The roman_to_int tests in doctest, run against the buggy roman.hpp (sums
// symbols, ignores subtractive notation).  The point of doing the RED demo in
// doctest specifically: its failure output shows the EXPECTED and ACTUAL
// values, not just "this line failed" --
//
//   ERROR: CHECK( roman_to_int("IV") == 4 ) is NOT correct!
//     values: CHECK( 6 == 4 )
//
// "6 == 4" tells you instantly the function summed I+V instead of subtracting.
// That expected-vs-actual diff is a big part of why you reach for a real
// framework over hand-rolled asserts.
//
//   sudo apt-get install doctest-dev
//   g++ -std=c++20 -Wall -Wextra roman_doctest.cpp -o roman_doctest && ./roman_doctest

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "roman.hpp"

TEST_CASE("single symbols") {
  CHECK(roman_to_int("I") == 1);
  CHECK(roman_to_int("V") == 5);
  CHECK(roman_to_int("X") == 10);
}

TEST_CASE("additive sequences (no subtraction needed) -- these pass") {
  CHECK(roman_to_int("II") == 2);
  CHECK(roman_to_int("VI") == 6);
  CHECK(roman_to_int("XV") == 15);
}

TEST_CASE("subtractive notation -- these FAIL on the buggy impl") {
  CHECK(roman_to_int("IV") == 4);
  CHECK(roman_to_int("IX") == 9);
}

TEST_CASE("mixed -- the year on the slide") {
  CHECK(roman_to_int("XIV") == 14);  // X + IV
  CHECK(roman_to_int("MMXXIV") == 2024);
}
