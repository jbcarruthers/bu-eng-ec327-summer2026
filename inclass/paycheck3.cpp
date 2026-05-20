// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 3 -- count paydays per month.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L3
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 3   ::   NEW FEATURE
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Read the day-of-year of the year's first payday. Print, for each
//    of the 12 months, how many paychecks it gets -- and list the
//    months that get three.
//
// -------------------------------------------------------------------
//  THIS STAGE   (NEW FEATURE -- stage 2 listed paydays; now count them)
//    This is the puzzle's real question for a year: how many paychecks
//    land in each month? We answer it the only way we can so far --
//    with TWELVE separate counter variables, one per month.
//
//    This stage is meant to feel clumsy. That is the point.
//
//    WALL -> twelve near-identical variables, twelve near-identical
//    if-branches, twelve near-identical print lines. Stage 4
//    (REFACTOR) collapses all of it with one array and one loop.
// ===================================================================

#include <iostream>

// --- brought forward from stage 2 (paste your working versions) ---
int month_of_day(int day) {
  return 0;   // >>> your stage-2 month_of_day
}
int month_length(int month) {
  return 0;   // >>> your stage-2 month_length
}

int main() {
  int anchor{};
  std::cout << "Day-of-year of the first payday (1-365): ";
  std::cin >> anchor;

  // One counter per month. Twelve of them. Notice the repetition.
  int paydays1 = 0;   int paydays2 = 0;   int paydays3 = 0;
  int paydays4 = 0;   int paydays5 = 0;   int paydays6 = 0;
  int paydays7 = 0;   int paydays8 = 0;   int paydays9 = 0;
  int paydays10 = 0;  int paydays11 = 0;  int paydays12 = 0;

  // Walk every payday; add 1 to the counter for the month it lands in.
  for (int day = anchor; day <= 365; day += 14) {
    int month = month_of_day(day);

    // TODO: increase the counter for `month`. One branch is shown --
    //       you need all twelve. (Feel the wall.)
    if (month == 1) {
      paydays1 += 1;
    }
    // else if (month == 2) { paydays2 += 1; }
    // ... continue through month 12 ...
  }

  // TODO: print all twelve months. One line is shown -- you need
  //       twelve. Format: "month M (L days): P paychecks".
  std::cout << "month 1 (" << month_length(1) << " days): "
            << paydays1 << " paychecks\n";
  // ... eleven more print lines ...

  // TODO: print the numbers of the months that got 3 or more.

  return 0;
}
