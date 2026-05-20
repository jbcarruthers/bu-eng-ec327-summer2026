// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 4 -- introduce arrays.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L3
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 4   ::   REFACTOR  (introduces: arrays)
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Read the day-of-year of the year's first payday. Print each
//    month's paycheck count and list the three-paycheck months.
//    EXACTLY the same output as stage 3.
//
// -------------------------------------------------------------------
//  THIS STAGE   (REFACTOR -- same output as stage 3, far less code)
//    Stage 3's twelve counter variables were really one idea, twelve
//    times over. An ARRAY is that idea: many values of the same type,
//    under one name, reached by an index.
//
//    NEW C++ FEATURE -- the array:
//        int paydays[12] = {};   // twelve ints, all starting at 0
//        paydays[0]              // the first   (month 1)
//        paydays[11]             // the last    (month 12)
//    An array is indexed from 0, so month M lives at index M - 1.
//    Watch that off-by-one.
//
//    The twelve if-branches become one indexing expression; the
//    twelve print lines become one loop.
//
//    WALL -> we now keep two arrays in step: each month's length AND
//    its payday count -- two facts about one thing, stored apart.
//    Stage 5 (REFACTOR) bundles them with a struct.
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

  // Two arrays, kept in step. Index M-1 holds month M.
  int lengths[12] = {};   // each month's length
  int paydays[12] = {};   // each month's paycheck count (starts at 0)

  // TODO: fill `lengths` -- for each month 1..12, store
  //       month_length(month) at index month-1.
  for (int month = 1; month <= 12; month += 1) {
    // lengths[month - 1] = ...;
  }

  // Walk every payday; add 1 to the array slot for its month.
  for (int day = anchor; day <= 365; day += 14) {
    int month = month_of_day(day);
    // TODO: add 1 to paydays[month - 1]
  }

  // TODO: one loop prints all twelve months, format
  //       "month M (L days): P paychecks".
  for (int month = 1; month <= 12; month += 1) {
    // std::cout << "month " << month << " (" << ... ;
  }

  // TODO: one more loop prints the three-paycheck months.

  return 0;
}
