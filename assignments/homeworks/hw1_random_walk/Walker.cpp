#include "Walker.h"

#include <vector>

// =============================================================
//  Walker.cpp  --  implementations of the things declared in Walker.h
//
//  Fill in the TODOs below. Build often; do not write all three
//  functions before compiling once.
// =============================================================

void step([[maybe_unused]] Walker& w) {
  // TODO (Part 1): move the walker +1 or -1 with equal probability.
  //
  //   You will need to  #include <random>  at the top of this file.
  //
  //   PITFALL (the #1 HW1 bug): the random-number engine must PERSIST
  //   across calls. Declare it `static` inside this function so it is
  //   created once and reused. A fresh engine on every call makes
  //   every step identical. See the spec's "Hints and pitfalls".
}

void walk_for([[maybe_unused]] Walker& w, [[maybe_unused]] int N) {
  // TODO (Part 2): call step(w) exactly N times.
}

std::vector<int> run_trials([[maybe_unused]] int K, [[maybe_unused]] int N) {
  // TODO (Part 3): run K independent walkers. For each of the K trials:
  //     - create a Walker at position 0
  //     - walk_for(walker, N)
  //     - record its final position
  //   Return the vector of K final positions.
  return {};
}
