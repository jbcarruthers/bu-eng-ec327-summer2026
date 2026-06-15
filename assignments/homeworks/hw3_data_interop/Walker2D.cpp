#include "Walker2D.h"

#include <cmath>
#include <random>
#include <vector>

// =============================================================
//  Walker2D.cpp  --  the reference HW2 walker, provided complete.
//  You do not need to edit this file for HW3.
// =============================================================

Walker2D::Walker2D() : x_(0), y_(0) {}

void Walker2D::step(std::mt19937& rng) {
  std::uniform_int_distribution<int> dir(0, 3);
  switch (dir(rng)) {
    case 0: y_ += 1; break;  // N
    case 1: y_ -= 1; break;  // S
    case 2: x_ += 1; break;  // E
    case 3: x_ -= 1; break;  // W
  }
}

int Walker2D::x() const { return x_; }
int Walker2D::y() const { return y_; }

long long Walker2D::r_squared() const {
  return static_cast<long long>(x_) * x_ + static_cast<long long>(y_) * y_;
}

double Walker2D::distance_from_origin() const {
  return std::sqrt(static_cast<double>(r_squared()));
}

void walk_for(Walker2D& w, int N, std::mt19937& rng) {
  for (int i = 0; i < N; ++i) w.step(rng);
}

std::vector<Walker2D> run_trials(int K, int N, std::mt19937& rng) {
  std::vector<Walker2D> walkers;
  walkers.reserve(K);
  for (int i = 0; i < K; ++i) {
    Walker2D w;
    walk_for(w, N, rng);
    walkers.push_back(w);
  }
  return walkers;
}
