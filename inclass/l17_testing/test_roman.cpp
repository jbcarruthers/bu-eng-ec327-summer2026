// EC327 L17 -- test_roman.cpp
// Tests for roman_to_int.  Run this against the shipped roman.hpp and watch
// two cases FAIL -- the suite caught the subtractive-notation bug:
//
//   g++ -std=c++20 -Wall -Wextra test_roman.cpp -o test_roman && ./test_roman
//
// The additive cases pass (IorII, VI, MMXXIV); the subtractive cases (IV, IX,
// XIV) fail because the implementation just sums symbols.  This is the live
// "red" in red-green-refactor: the test fails FIRST, then you fix the code
// until it goes green.  Fix is in SOLUTIONS.md.

#include "microtest.hpp"
#include "roman.hpp"

TEST_CASE("single symbols") {
  CHECK_EQ(roman_to_int("I"), 1);
  CHECK_EQ(roman_to_int("V"), 5);
  CHECK_EQ(roman_to_int("X"), 10);
}

TEST_CASE("additive sequences (no subtraction needed)") {
  CHECK_EQ(roman_to_int("II"), 2);
  CHECK_EQ(roman_to_int("VI"), 6);
  CHECK_EQ(roman_to_int("XV"), 15);
}

TEST_CASE("subtractive notation -- IV is 4, not 6") {
  CHECK_EQ(roman_to_int("IV"), 4);
  CHECK_EQ(roman_to_int("IX"), 9);
}

TEST_CASE("mixed -- the year on the slide") {
  CHECK_EQ(roman_to_int("XIV"), 14);  // X + IV
  CHECK_EQ(roman_to_int("MMXXIV"), 2024);
}

MICROTEST_MAIN
