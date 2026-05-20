// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 6 -- the real calendar.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L4
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 6   ::   NEW FEATURE
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Read a year and the day-of-year of its first payday. On the real
//    Gregorian calendar -- real month lengths, real leap years --
//    print each month's paycheck count and the three-paycheck months.
//
// -------------------------------------------------------------------
//  THIS STAGE   (NEW FEATURE -- the made-up calendar is retired)
//    The simplifying assumptions are gone. Two real-world facts
//    arrive:
//
//    * Real month lengths. Jan 31, Feb 28, Mar 31, Apr 30, ... There
//      is NO formula for this -- the twelve numbers are simply data,
//      written down as a table of constants. This is why stage 4's
//      array mattered.
//
//    * Leap years. Every 4th year February has 29 days, except
//      century years, which need to be divisible by 400. We capture
//      the rule in a function, is_leap_year().
//
//    Month length now depends on the year, so month_length() and
//    month_of_day() each take a `year` argument.
//
//    WALL -> this answers the puzzle for ONE first-payday date. Move
//    that date and the answer moves. Stage 7 (NEW FEATURE) tries all
//    14 possibilities -- and must collect a list whose length it does
//    not know ahead of time.
// ===================================================================

#include <iostream>

// The real calendar has no formula -- the twelve lengths are data.
// (February shown as 28; leap years add a day, handled below.)
const int REAL_LENGTH[12] = {31, 28, 31, 30, 31, 30,
                             31, 31, 30, 31, 30, 31};

// Is `year` a leap year?
bool is_leap_year(int year) {
  // TODO: true if `year` is divisible by 4, EXCEPT century years
  //       (divisible by 100), which are leap only if also divisible
  //       by 400. Check yourself: 2024 yes, 2026 no, 1900 no, 2000 yes.
  return false;   // placeholder
}

// Length of month `month` (1..12) in `year`.
int month_length(int month, int year) {
  // TODO: return REAL_LENGTH[month - 1]; but for February (month 2)
  //       in a leap year, return 29.
  return 0;   // placeholder
}

// Which month (1..12) does day-of-year `day` fall in, in `year`?
// No formula now -- subtract real month lengths until `day` fits.
int month_of_day(int day, int year) {
  // TODO: starting at month 1, while `day` reaches past the current
  //       month, subtract that month's length and move to the next.
  return 0;   // placeholder
}

struct Month {
  int length;
  int paydays;
};

int main() {
  int year{};
  int anchor{};
  std::cout << "Year: ";
  std::cin >> year;
  std::cout << "Day-of-year of the first payday: ";
  std::cin >> anchor;

  Month months[12];
  for (int month = 1; month <= 12; month += 1) {
    // TODO: set .length = month_length(month, year), .paydays = 0
  }

  // A leap year is 366 days long, not 365.
  int year_length = 365;
  if (is_leap_year(year)) {
    year_length = 366;
  }

  for (int day = anchor; day <= year_length; day += 14) {
    int month = month_of_day(day, year);
    // TODO: add 1 to months[month - 1].paydays
  }

  // TODO: print each month, then the three-paycheck months.

  return 0;
}
