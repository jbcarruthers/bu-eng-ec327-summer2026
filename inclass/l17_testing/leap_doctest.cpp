// EC327 L17 -- leap_doctest.cpp
// The SAME is_leap_year tests as test_leap.cpp, but written against the real
// doctest framework instead of our 60-line microtest.hpp.  Compare the two
// files side by side: the ideas are identical (named cases, CHECK macros, a
// runner) -- doctest just adds nicer output, SUBCASE grouping, command-line
// filtering, and thousands of lines of polish you don't have to maintain.
//
// doctest is HEADER-ONLY: install the header, no library to link.
//   sudo apt-get install doctest-dev      # Ubuntu (the Multipass box)
//   g++ -std=c++20 -Wall -Wextra leap_doctest.cpp -o leap_doctest
//   ./leap_doctest                         # add --help to see the options
//
// DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN tells doctest to generate main() for us.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "leap.hpp"

TEST_CASE("the Gregorian leap-year rule") {
  SUBCASE("ordinary divisible-by-4 years are leap") {
    CHECK(is_leap_year(2024));
    CHECK(is_leap_year(2028));
  }
  SUBCASE("non-multiples of 4 are not leap") {
    CHECK_FALSE(is_leap_year(2023));
    CHECK_FALSE(is_leap_year(2025));
  }
  SUBCASE("century years are NOT leap -- the famous trap") {
    CHECK_FALSE(is_leap_year(1900));
    CHECK_FALSE(is_leap_year(2100));
  }
  SUBCASE("400-divisible centuries ARE leap -- the trap's trap") {
    CHECK(is_leap_year(2000));
    CHECK(is_leap_year(1600));
  }
  SUBCASE("boundary / odd inputs follow the rule and don't crash") {
    CHECK(is_leap_year(0));        // divisible by 400
    CHECK_FALSE(is_leap_year(1));  // smallest non-leap
    CHECK(is_leap_year(-4));       // negatives still follow the % rule
  }
}
