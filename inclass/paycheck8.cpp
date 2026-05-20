// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 8 -- introduce std::map.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L12
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 8   ::   NEW FEATURE  (introduces: map)
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Read a first year and a last year. Across every year in that
//    range and all 14 pay schedules, count how many three-paycheck
//    months were 28-, 29-, 30-, or 31-day months. Print the tally
//    and the share that were 31-day months.
//
// -------------------------------------------------------------------
//  THIS STAGE   (NEW FEATURE -- the statistical question, at last)
//    We want counts grouped by month LENGTH. The natural label for a
//    bucket is a length -- 28, 29, 30, 31 -- not a slot number 0..11.
//    An array is indexed by position; here we want to index by value.
//
//    NEW C++ FEATURE -- std::map<Key, Value>:
//        std::map<int, int> tally;   // length -> count
//        tally[31] += 1;             // a bucket appears when first used
//        for (auto entry : tally)    // entry.first, entry.second
//
//    WALL -> the share of 31-day months you measure here is far above
//    a naive guess that treats months as independent. Is the real
//    calendar's STRUCTURE causing that, or is it chance? Stage 9
//    (NEW FEATURE) settles it by shuffling the calendar.
// ===================================================================

#include <iostream>
#include <map>
#include <vector>

// --- brought forward from stages 6-7 (paste your working versions) -
bool is_leap_year(int year) { return false; }
int month_length(int month, int year) { return 0; }
int month_of_day(int day, int year) { return 0; }
std::vector<int> three_paycheck_months(int year, int anchor) {
  return {};
}

int main() {
  int first_year{};
  int last_year{};
  std::cout << "First year: ";
  std::cin >> first_year;
  std::cout << "Last year: ";
  std::cin >> last_year;

  // tally[length] = how many three-paycheck months had that length.
  std::map<int, int> tally;

  for (int year = first_year; year <= last_year; year += 1) {
    for (int anchor = 1; anchor <= 14; anchor += 1) {
      std::vector<int> hits = three_paycheck_months(year, anchor);
      for (int m : hits) {
        int len = month_length(m, year);
        // TODO: add 1 to tally[len]  -- the map makes this one line.
      }
    }
  }

  // TODO: print each length bucket and its count.
  // TODO: print the share of all three-paycheck months that were 31
  //       days long (the 31-day count divided by the grand total).

  return 0;
}
