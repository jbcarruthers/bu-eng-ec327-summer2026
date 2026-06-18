// EC327 L17 -- test_leap.cpp
// Unit tests for is_leap_year, written against our teaching framework.
// Build & run:
//   g++ -std=c++20 -Wall -Wextra test_leap.cpp -o test_leap && ./test_leap
//
// Every test case is the same three-beat shape -- Arrange, Act, Assert (AAA):
//   arrange: pick an input
//   act:     call the function
//   assert:  CHECK the claim about the result
// Here arrange+act collapse into the CHECK argument because the function is
// pure and tiny.  Notice the cases ARE the spec: read them top to bottom and
// you have the leap-year rule, executably.

#include "leap.hpp"
#include "microtest.hpp"

TEST_CASE("ordinary divisible-by-4 years are leap") {
  CHECK(is_leap_year(2024));
  CHECK(is_leap_year(2028));
}

TEST_CASE("non-multiples of 4 are not leap") {
  CHECK(!is_leap_year(2023));
  CHECK(!is_leap_year(2025));
}

TEST_CASE("century years are NOT leap -- the famous trap") {
  CHECK(!is_leap_year(1900));
  CHECK(!is_leap_year(2100));
}

TEST_CASE("400-divisible centuries ARE leap -- the trap's trap") {
  CHECK(is_leap_year(2000));
  CHECK(is_leap_year(1600));
}

TEST_CASE("boundary / odd inputs don't crash and follow the rule") {
  CHECK(is_leap_year(0));   // year 0 is divisible by 400
  CHECK(!is_leap_year(1));  // smallest non-leap
  CHECK(is_leap_year(-4));  // negative years still follow the % rule
}

MICROTEST_MAIN
