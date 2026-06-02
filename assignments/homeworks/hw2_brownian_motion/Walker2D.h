#pragma once

#include <random>
#include <vector>

// =============================================================
//  Walker2D.h  --  EC327 Summer 2026, Homework 2
//
//  This header DECLARES a class and three free functions. The matching
//  Walker2D.cpp DEFINES them; hw2_main.cpp #includes this header to learn
//  what it can call.
//
//  This is your HW1 `Walker` struct, grown up. Three things changed:
//    1. It is a CLASS now: the position is PRIVATE, reached through const
//       accessors. The class controls its own data (encapsulation).
//    2. It lives in 2D: an (x, y) grid instead of a number line.
//    3. step() takes the random engine BY REFERENCE instead of hiding a
//       static one inside itself. Whoever owns the engine (main) controls
//       the randomness -- that is what makes the simulation reproducible.
//
//  The bodies in Walker2D.cpp are empty. Fill them in per the spec. The
//  header is your contract: implement exactly these signatures.
// =============================================================

// A random walker on the 2D integer grid. At each step it moves one unit
// in one of four directions -- North, South, East, or West -- chosen
// uniformly at random.
class Walker2D {
 public:
  // Start the walker at the origin (0, 0). (Part 1)
  Walker2D();

  // Take one random unit step: N (y+1), S (y-1), E (x+1), or W (x-1),
  // each with probability 1/4. The engine is passed by reference so its
  // state advances correctly across every call. (Part 1)
  void step(std::mt19937& rng);

  // Where is the walker? These do not change it, so they are const. (Part 1)
  int x() const;
  int y() const;

  // Squared distance from the origin, x*x + y*y. Returned as long long
  // because sums of squares grow fast. (Part 1)
  long long r_squared() const;

  // Straight-line distance from the origin, sqrt(x*x + y*y). (Part 1)
  double distance_from_origin() const;

 private:
  int x_;
  int y_;
};

// Take N steps in a row by calling step() N times, threading the same
// engine through each call. (Part 2)
void walk_for(Walker2D& w, int N, std::mt19937& rng);

// Run K independent walkers, each for N steps, each starting at the
// origin. Returns the vector of K finished walkers. (Part 3)
std::vector<Walker2D> run_trials(int K, int N, std::mt19937& rng);
