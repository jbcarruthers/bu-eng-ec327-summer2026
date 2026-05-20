// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 1 -- walk one whole year.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L2
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 1   ::   NEW FEATURE
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Read the day-of-year (1-365) on which the year's first payday
//    lands. Print every payday in the 365-day year and which month
//    (1-12) each one falls in.
//
// -------------------------------------------------------------------
//  THIS STAGE   (NEW FEATURE -- stage 0 did one month; now: a year)
//    The calendar is deliberately MADE UP -- no real months, no
//    February, no leap years:
//
//        365 days, 12 months.
//        Months 1-7  are 30 days each   (days   1 .. 210)
//        Months 8-12 are 31 days each   (days 211 .. 365)
//        7 * 30  +  5 * 31  =  210 + 155  =  365
//
//    Because it is this regular, a day's month is plain arithmetic --
//    no table of month lengths needed yet. Uses a for-loop, integer
//    division/modulo, and if/else.
//
//    WALL -> the month arithmetic is fiddly and we keep needing it.
//    Stage 2 (REFACTOR) gives that computation a name.
// ===================================================================

#include <iostream>

int main() {
  int anchor{};   // day-of-year (1..365) of the first payday
  std::cout << "Day-of-year of the first payday (1-365): ";
  std::cin >> anchor;

  std::cout << "\nPayday schedule for the year:\n";

  // Step through every payday in the year: anchor, anchor+14, ...
  for (int day = anchor; day <= 365; day += 14) {
    // TODO: given `day` (a day-of-year, 1..365), work out which
    //       month (1..12) it falls in, on the simplified calendar:
    //         - days   1..210  ->  30-day block, months 1..7
    //         - days 211..365  ->  31-day block, months 8..12
    //       Integer division ( / ) turns a day number into a month.
    int month = 0;   // <-- replace 0 with your computed month

    std::cout << "  payday on day " << day
              << "  ->  month " << month << '\n';
  }

  return 0;
}
