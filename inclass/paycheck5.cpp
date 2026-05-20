// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 5 -- introduce structs.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L3
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 5   ::   REFACTOR  (introduces: structs)
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Read the day-of-year of the year's first payday. Print each
//    month's paycheck count and list the three-paycheck months.
//    EXACTLY the same output as stages 3 and 4.
//
// -------------------------------------------------------------------
//  THIS STAGE   (REFACTOR -- same output as stage 4, better organised)
//    Stage 4 carried two arrays, `lengths` and `paydays`, that had to
//    be kept in step by hand: index M-1 in BOTH meant month M. When
//    two arrays must always agree, they want to be one array.
//
//    NEW C++ FEATURE -- the struct: bundle related values into one
//    named type.
//        struct Month {
//          int length;    // days in the month
//          int paydays;   // paychecks counted in it
//        };
//    Now one array, Month months[12], holds both facts together;
//    reach a field with a dot:   months[2].paydays
//
//    WALL -> our calendar is still the made-up 7*30 + 5*31. The real
//    world has months of 28/29/30/31 days in a fixed, irregular
//    order, and leap years. Stage 6 (NEW FEATURE) brings it in.
// ===================================================================

#include <iostream>

// --- brought forward from stage 2 (paste your working versions) ---
int month_of_day(int day) {
  return 0;   // >>> your stage-2 month_of_day
}
int month_length(int month) {
  return 0;   // >>> your stage-2 month_length
}

// NEW: a struct -- two facts about a month, bundled into one type.
struct Month {
  int length;    // days in the month
  int paydays;   // paychecks counted in it
};

int main() {
  int anchor{};
  std::cout << "Day-of-year of the first payday (1-365): ";
  std::cin >> anchor;

  // ONE array of structs replaces stage 4's two parallel arrays.
  Month months[12];

  // TODO: for each month 1..12, set months[month-1].length from
  //       month_length(month), and set .paydays to 0.
  for (int month = 1; month <= 12; month += 1) {
    // months[month - 1].length  = ...;
    // months[month - 1].paydays = 0;
  }

  // Walk every payday; add 1 to the right month's .paydays field.
  for (int day = anchor; day <= 365; day += 14) {
    int month = month_of_day(day);
    // TODO: add 1 to months[month - 1].paydays
  }

  // TODO: one loop prints all twelve months, same format as stage 4.
  // TODO: one more loop prints the three-paycheck months.

  return 0;
}
