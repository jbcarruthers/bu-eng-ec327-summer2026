// EC327 L17 -- leap_catch2.cpp
// The same leap tests in Catch2 v3.  Note how little changes from doctest --
// TEST_CASE / SECTION / CHECK / REQUIRE.  That's the whole point of the
// "pick one, they're all the same idea" slide.
//
// Catch2 v3 is NOT header-only the way v2 was -- you link its library, which
// also supplies main():
//   sudo apt-get install catch2          # Ubuntu (the Multipass box)
//   g++ -std=c++20 -Wall -Wextra leap_catch2.cpp -o leap_catch2 -lCatch2Main -lCatch2
//   ./leap_catch2
//
// CHECK keeps going after a failure; REQUIRE aborts the test case (use it when
// the rest of the case makes no sense once this fails).

#include <catch2/catch_test_macros.hpp>

#include "leap.hpp"

TEST_CASE("the Gregorian leap-year rule", "[leap]") {
  SECTION("ordinary divisible-by-4 years are leap") {
    CHECK(is_leap_year(2024));
    CHECK(is_leap_year(2028));
  }
  SECTION("non-multiples of 4 are not leap") {
    CHECK_FALSE(is_leap_year(2023));
    CHECK_FALSE(is_leap_year(2025));
  }
  SECTION("century years are NOT leap -- the famous trap") {
    CHECK_FALSE(is_leap_year(1900));
    CHECK_FALSE(is_leap_year(2100));
  }
  SECTION("400-divisible centuries ARE leap -- the trap's trap") {
    REQUIRE(is_leap_year(2000));  // REQUIRE: if this is wrong, stop the case
    CHECK(is_leap_year(1600));
  }
}
