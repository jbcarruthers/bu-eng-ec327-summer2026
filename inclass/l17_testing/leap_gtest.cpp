// EC327 L17 -- leap_gtest.cpp
// The same leap tests in GoogleTest -- the heavyweight you'll meet in big C++
// codebases.  Different surface syntax (TEST(Suite, Name) and EXPECT_*/ASSERT_*
// instead of TEST_CASE/CHECK), same three ideas underneath.
//
//   sudo apt-get install libgtest-dev    # Ubuntu (the Multipass box)
//   g++ -std=c++20 -Wall -Wextra leap_gtest.cpp -o leap_gtest -lgtest -lgtest_main -pthread
//   ./leap_gtest
//
// EXPECT_* records a failure and keeps going; ASSERT_* aborts the current test
// (the CHECK vs REQUIRE distinction again).  gtest_main supplies main().

#include <gtest/gtest.h>

#include "leap.hpp"

TEST(LeapYear, OrdinaryDivisibleBy4AreLeap) {
  EXPECT_TRUE(is_leap_year(2024));
  EXPECT_TRUE(is_leap_year(2028));
}

TEST(LeapYear, NonMultiplesOf4AreNotLeap) {
  EXPECT_FALSE(is_leap_year(2023));
  EXPECT_FALSE(is_leap_year(2025));
}

TEST(LeapYear, CenturyYearsAreNotLeap) {  // the famous trap
  EXPECT_FALSE(is_leap_year(1900));
  EXPECT_FALSE(is_leap_year(2100));
}

TEST(LeapYear, FourHundredDivisibleCenturiesAreLeap) {  // the trap's trap
  EXPECT_TRUE(is_leap_year(2000));
  EXPECT_TRUE(is_leap_year(1600));
}

TEST(LeapYear, BoundaryAndOddInputs) {
  EXPECT_TRUE(is_leap_year(0));   // divisible by 400
  EXPECT_FALSE(is_leap_year(1));  // smallest non-leap
  EXPECT_TRUE(is_leap_year(-4));  // negatives still follow the % rule
}
