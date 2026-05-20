// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 0 -- one month, on its own.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L2
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 0   ::   START
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Read a month's length in days and the day-of-month its first
//    payday lands on. Print how many paychecks fall within that
//    month, given paydays come every 14 days.
//
// -------------------------------------------------------------------
//  THIS STAGE
//    The starting point. One month, standing alone: it is
//    `month_length` days long, its first payday is on day
//    `first_payday`, and paydays repeat every 14 days. Count them.
//    Uses only int, if/else, and arithmetic -- nothing new.
//
//    WALL -> a year is twelve linked months. Stage 1 (NEW FEATURE)
//    walks a whole year.
//
// -------------------------------------------------------------------
//  ABOUT THIS SERIES   (read once, then skip)
//    The `paycheck` programs grow one idea in small stages. Every
//    stage is labelled:
//      START       -- the first program.
//      NEW FEATURE -- the program now does something new.
//      REFACTOR    -- SAME output as the stage before, rewritten more
//                     cleanly with a C++ feature you just met.
//    Each stage hits a WALL; the next stage hands you the tool to get
//    over it -- that is how functions, structs, arrays, vectors, and
//    maps enter. Full puzzle write-up: paycheck-puzzle-summary.md.
//    Early-stage simplifications (dropped at stage 6): no leap years,
//    365-day years, no real calendar.
// ===================================================================

#include <iostream>

int main() {
  int month_length{};   // how many days in this month
  int first_payday{};   // day-of-month (1..14) of the first payday

  std::cout << "Month length in days: ";
  std::cin >> month_length;
  std::cout << "Day-of-month of the first payday (1-14): ";
  std::cin >> first_payday;

  // The first three candidate paydays fall on these days of the month.
  // (A fourth, first_payday + 42, can never fit in a <= 31-day month.)
  int payday1 = first_payday;
  int payday2 = first_payday + 14;
  int payday3 = first_payday + 28;

  // TODO: count how many of payday1, payday2, payday3 actually land
  //       on or before the last day of the month (that is, are
  //       <= month_length). Store the answer in `paydays`.
  int paydays = 0;
  // ... your if-statements go here ...

  std::cout << "Paychecks this month: " << paydays << '\n';

  // TODO: a "three-paycheck month" is one with 3 (or more) paydays.
  //       Print a line saying whether this month is one of them.

  return 0;
}
