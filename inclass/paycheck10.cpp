// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: biweekly-paycheck puzzle, stage 10 -- design the calendar.
// Topic: paycheck-puzzle
// Category: starter
// Lecture: L17
//
// ===================================================================
//  PAYCHECK PUZZLE -- STAGE 10   ::   NEW FEATURE   (the capstone)
// ===================================================================
//
//  WHAT THIS PROGRAM DOES
//    Search for the 12-month, 365-day calendar that maximises the
//    share of three-paycheck months landing on 31-day months. Print
//    the best calendar found and its share.
//
// -------------------------------------------------------------------
//  THIS STAGE   (NEW FEATURE -- stop measuring calendars, DESIGN one)
//    Stage 9 showed the month order matters. So which order is best?
//    We search. Start from some calendar and repeatedly make a small
//    change -- move one day from one month to another, total fixed at
//    365 -- keeping any change that raises the 31-day share. Always
//    stepping uphill is a "hill climb".
//
//    Watch what it converges to. The summary calls it the "Gregor
//    reform": eleven 31-day months and one short, sacrificial month.
//    Then EVERY three-paycheck month is a 31-day month -- share 1.0.
//
//  THE PAYOFF -- this is gerrymandering. PACK all the favourable
//    structure (31-day months) into the winning category; CRACK the
//    rest into one throwaway bucket that can never qualify. The
//    change looks locally fair -- still 12 months, still 365 days --
//    yet it rigs the global outcome. Same move, same maths, whether
//    the districts are months or voters. See paycheck-puzzle-summary.md.
// ===================================================================

#include <array>
#include <iostream>

// --- brought forward from stage 9 (paste your working version) ----
double share_of_31(const std::array<int, 12>& cal) {
  return 0.0;   // >>> your stage-9 share_of_31
}

int main() {
  // Start the search from the real calendar.
  std::array<int, 12> best = {31, 28, 31, 30, 31, 30,
                              31, 31, 30, 31, 30, 31};
  double best_score = share_of_31(best);

  // TODO: hill-climb. Many times over:
  //         - copy `best` into a trial calendar;
  //         - move one day from one month to another (keep every
  //           month within a sane range, keep the total at 365);
  //         - score the trial with share_of_31;
  //         - if it beat best_score, adopt the trial as the new best.
  //       Stop when no single move improves things any further.

  std::cout << "Best 31-day share found: " << best_score << '\n';
  // TODO: print the twelve month lengths of `best`.

  return 0;
}
