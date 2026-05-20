// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 9 -- Monte Carlo shuffling.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L17
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 9   ::   NEW FEATURE  (introduces: random)
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Measure, for the real calendar, the share of three-paycheck
//    months that are 31 days long. Then measure the same share for
//    many randomly REORDERED calendars and average it. Print both.
//
// -------------------------------------------------------------------
//  THIS STAGE   (NEW FEATURE -- is the result real, or an accident?)
//    Stage 8 found 31-day months oddly favoured. To test whether the
//    real calendar's month ORDER causes that, we keep the same twelve
//    month lengths but shuffle their order, thousands of times, and
//    see if the effect survives.
//
//    A calendar here is just twelve month lengths in an array.
//
//    NEW C++ FEATURE -- <random>:
//        std::mt19937 rng(12345);                 // generator; seed 12345
//        std::shuffle(a.begin(), a.end(), rng);   // reorder at random
//    A fixed seed makes the run REPRODUCIBLE: same seed, same shuffle
//    sequence, same numbers every time -- essential for an experiment.
//
//    WALL -> if rearranging the calendar changes the answer, which
//    arrangement is BEST? Stage 10 (NEW FEATURE) searches for it.
// ===================================================================

#include <algorithm>
#include <array>
#include <iostream>
#include <random>

// A calendar is twelve month lengths. The real one (no leap years
// here -- a fixed 365-day year keeps the experiment clean):
const std::array<int, 12> GREGORIAN = {31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31};

// Of the three-paycheck months produced by calendar `cal`, what
// share are 31 days long? Averaged over all 14 pay schedules.
double share_of_31(const std::array<int, 12>& cal) {
  // TODO: for each phase 1..14, walk paydays (every 14 days) across
  //       the 365-day year, count paydays per month, and for every
  //       month with 3+ paychecks check whether cal[that month] is
  //       31. Return (31-day three-pay months) / (all three-pay
  //       months).
  return 0.0;   // placeholder
}

int main() {
  std::cout << "Real calendar:     31-day share = "
            << share_of_31(GREGORIAN) << '\n';

  std::mt19937 rng(12345);          // fixed seed -> reproducible run
  std::array<int, 12> cal = GREGORIAN;

  double total = 0.0;
  const int trials = 100000;
  for (int t = 0; t < trials; t += 1) {
    std::shuffle(cal.begin(), cal.end(), rng);
    // TODO: add share_of_31(cal) to `total`
  }

  // TODO: print total / trials -- the shuffled-calendar average --
  //       and compare it with the real calendar's share above.

  return 0;
}
