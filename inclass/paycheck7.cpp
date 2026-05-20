// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 7 -- introduce std::vector.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L4
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 7   ::   NEW FEATURE  (introduces: vector)
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Read a year. For each of the 14 possible biweekly pay schedules
//    (the first payday can fall on any of days 1..14), print which
//    months get three paychecks.
//
// -------------------------------------------------------------------
//  THIS STAGE   (NEW FEATURE -- one year, but all 14 schedules)
//    The set of three-paycheck months has a length we do NOT know in
//    advance: usually two, sometimes three. A plain array needs its
//    size fixed up front. We need a list that grows on demand.
//
//    NEW C++ FEATURE -- std::vector:
//        std::vector<int> hits;   // an empty, growable list of ints
//        hits.push_back(8);       // append a value
//        hits.size()              // how many it holds
//        for (int m : hits) ...   // range-based for: visit each one
//
//    WALL -> 14 schedules for one year is already a wall of numbers.
//    The real question is statistical: across many years and all
//    schedules, how often is a three-paycheck month a 31-day month?
//    Stage 8 (NEW FEATURE) tallies that -- keyed by month length.
// ===================================================================

#include <iostream>
#include <vector>

// --- brought forward from stage 6 (paste your working versions) ---
bool is_leap_year(int year) { return false; }
int month_length(int month, int year) { return 0; }
int month_of_day(int day, int year) { return 0; }

// Return the months (1..12) that get 3+ paychecks in `year` when the
// year's first payday falls on day-of-year `anchor`.
std::vector<int> three_paycheck_months(int year, int anchor) {
  std::vector<int> hits;
  // TODO: count paydays per month for this year and anchor (the work
  //       of stage 6), then push_back every month with 3 or more
  //       onto `hits`.
  return hits;
}

int main() {
  int year{};
  std::cout << "Year: ";
  std::cin >> year;

  // The 14 phases: the year's first payday on each of days 1..14.
  for (int anchor = 1; anchor <= 14; anchor += 1) {
    std::vector<int> hits = three_paycheck_months(year, anchor);

    std::cout << "first payday on day " << anchor << ": ";
    // TODO: print every month number in `hits` with a range-based for.
    std::cout << '\n';
  }

  return 0;
}
