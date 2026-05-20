// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 2 -- introduce functions.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L4
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 2   ::   REFACTOR  (introduces: functions)
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Read the day-of-year of the year's first payday; print every
//    payday with its month. EXACTLY the same result as stage 1 --
//    the day->month lookup is just now a function you write and call.
//
// -------------------------------------------------------------------
//  THIS STAGE   (REFACTOR -- same behaviour as stage 1, cleaner code)
//    A refactor changes the code, not what it does: run stage 1 and
//    stage 2 on the same input, get identical output. What changed is
//    that the fiddly month arithmetic now lives in ONE named place.
//
//    NEW C++ FEATURE -- the function: a computation with a NAME,
//    written ONCE, CALLED anywhere, taking parameters and handing
//    back a return value.
//
//        int month_of_day(int day) { ...; return some_month; }
//            ^^^          ^^^                ^^^^^^^^^^^^^^^
//         return type    parameter           return value
//
//    WALL -> we can LIST paydays, but the puzzle asks HOW MANY land
//    in each month. Stage 3 (NEW FEATURE) counts them -- and its only
//    tool, for now, is twelve separate counter variables.
// ===================================================================

#include <iostream>

// Return which month (1..12) a day-of-year (1..365) falls in, on the
// simplified calendar (months 1-7: 30 days; months 8-12: 31 days).
int month_of_day(int day) {
  // TODO: move your month-finding arithmetic from stage 1 into here,
  //       and `return` the month instead of printing it.
  return 0;   // <-- placeholder so the file still compiles
}

// Return the length in days (30 or 31) of a month (1..12) on the
// simplified calendar.
int month_length(int month) {
  // TODO: months 1-7 are 30 days; months 8-12 are 31 days.
  return 0;   // <-- placeholder so the file still compiles
}

int main() {
  int anchor{};
  std::cout << "Day-of-year of the first payday (1-365): ";
  std::cin >> anchor;

  std::cout << "\nPayday schedule for the year:\n";
  for (int day = anchor; day <= 365; day += 14) {
    // The fiddly arithmetic is gone -- just a clear call by name.
    int month = month_of_day(day);
    std::cout << "  payday on day " << day
              << "  ->  month " << month
              << " (" << month_length(month) << " days)\n";
  }

  return 0;
}
