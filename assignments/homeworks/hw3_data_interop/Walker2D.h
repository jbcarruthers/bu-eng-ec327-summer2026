#pragma once

#include <random>
#include <vector>

// =============================================================
//  Walker2D.h  --  EC327 Summer 2026, Homework 3
//
//  This is your HW2 walker, provided COMPLETE this time. You are not
//  reimplementing it -- HW3 is about getting data OUT of the simulation
//  and into files a Python data scientist can load, not about the walk
//  itself. DataSet.cpp uses run_trials() to produce the dataset you will
//  serialize. Read it if you are curious; you do not need to change it.
// =============================================================

// A random walker on the 2D integer grid: each step moves one unit in one
// of four directions (N/S/E/W), chosen uniformly at random.
class Walker2D {
 public:
  Walker2D();                     // start at the origin (0, 0)
  void step(std::mt19937& rng);   // one random N/S/E/W unit step
  int x() const;
  int y() const;
  long long r_squared() const;          // x*x + y*y
  double distance_from_origin() const;  // sqrt(x*x + y*y)

 private:
  int x_;
  int y_;
};

// Step a walker N times, threading the same engine through each call.
void walk_for(Walker2D& w, int N, std::mt19937& rng);

// Run K independent walkers, each for N steps from the origin. Returns the
// vector of K finished walkers.
std::vector<Walker2D> run_trials(int K, int N, std::mt19937& rng);
