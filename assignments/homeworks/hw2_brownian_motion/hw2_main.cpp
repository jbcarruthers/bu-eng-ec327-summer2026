#include <iostream>
#include <random>

#include "Histogram2D.h"
#include "Walker2D.h"

// =============================================================
//  hw2_main.cpp  --  EC327 Summer 2026, Homework 2
//
//  This file orchestrates the experiment. Build it up one Part at a time --
//  get Part 1 printing before you touch Part 2. The starter compiles and
//  runs as-is; it just does not do anything yet.
//
//  Create exactly ONE random engine here in main and thread it through
//  every call (step / walk_for / run_trials). Seed it with
//  std::random_device{}() for a different run every time, or with a fixed
//  number (e.g. 42) while you are debugging and want the same walk twice.
// =============================================================

int main() {
  std::mt19937 rng(std::random_device{}());

  std::cout << "EC327 HW2 -- 2D Brownian motion "
               "(starter -- nothing implemented yet)\n";

  // Part 1: create one Walker2D, step it 10 times, and print its position
  //   and distance after each step.

  // Part 2: run one fresh walker for N = 10, 100, 1000 and print each final
  //   position and distance. Run the program a few times.

  // Part 3: use run_trials() with K = 10, 100, 1000 (N = 100). Compute the
  //   mean distance, the mean-squared displacement <r^2>, and the max
  //   distance -- using <numeric> / <algorithm>, not hand-rolled loops.

  // Part 4: run K = 10000 walkers for N = 100. Print TWO histograms from the
  //   same batch: the x-coordinates (histogram_int, width 5) and the
  //   distances from the origin (histogram_distance, width 2.0). Capture
  //   output for at least three N values into output.txt.

  return 0;
}
