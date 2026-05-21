#pragma once

#include <vector>

// =============================================================
//  Walker.h  --  EC327 Summer 2026, Homework 1
//
//  This is a HEADER. A header DECLARES things -- it says "a struct
//  named Walker exists" and "a function named step exists" without
//  saying how step works. The matching .cpp file DEFINES (implements)
//  them. hw1_main.cpp #includes this header to learn what it can call.
// =============================================================

// A random walker on the integer number line.
//
// TODO (Part 1): give this struct the data it needs.
//   A walker only ever needs to know one thing: where it is.
struct Walker {
  // TODO (Part 1): declare the walker's position here.
};

// Take one random step: +1 or -1, each with probability 1/2.
// The walker is passed BY REFERENCE (the `&`) so the change this
// function makes is visible to the caller. (Part 1)
void step(Walker& w);

// Take N steps in a row by calling step() N times. (Part 2)
void walk_for(Walker& w, int N);

// Run K independent walkers, each for N steps, each starting at
// position 0. Returns a vector holding the K final positions. (Part 3)
std::vector<int> run_trials(int K, int N);
