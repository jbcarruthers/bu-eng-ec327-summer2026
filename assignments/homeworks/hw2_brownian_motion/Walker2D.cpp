#include "Walker2D.h"

#include <cmath>
#include <random>
#include <vector>

// =============================================================
//  Walker2D.cpp  --  implementations of the things declared in Walker2D.h
//
//  The bodies are empty. Implement each one to satisfy the contract in the
//  header and the spec. Build often -- do not write everything before
//  compiling once. The starter compiles and runs as-is; it just returns
//  placeholder values.
// =============================================================

Walker2D::Walker2D() : x_(0), y_(0) {}

void Walker2D::step([[maybe_unused]] std::mt19937& rng) {
  // Part 1
}

int Walker2D::x() const {
  return x_;
}

int Walker2D::y() const {
  return y_;
}

long long Walker2D::r_squared() const {
  // Part 1
  return 0;
}

double Walker2D::distance_from_origin() const {
  // Part 1
  return 0.0;
}

void walk_for([[maybe_unused]] Walker2D& w, [[maybe_unused]] int N,
              [[maybe_unused]] std::mt19937& rng) {
  // Part 2
}

std::vector<Walker2D> run_trials([[maybe_unused]] int K, [[maybe_unused]] int N,
                                 [[maybe_unused]] std::mt19937& rng) {
  // Part 3
  return {};
}
