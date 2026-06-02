#pragma once

#include <map>
#include <vector>

// =============================================================
//  Histogram2D.h  --  EC327 Summer 2026, Homework 2  (Part 4)
//
//  Two binning functions and a printer. You will recognize most of this
//  from HW1 -- histogram_int is essentially your HW1 histogram(), and
//  print_histogram is your HW1 printer unchanged. histogram_distance is
//  new, and is actually EASIER than HW1 (distances are never negative, so
//  the toward-zero rounding gotcha does not apply).
// =============================================================

// Bin integer values into buckets of the given width. The map key is the
// bucket's lower edge; the value is the count. Values can be NEGATIVE
// (e.g. a walker's x-coordinate), so mind the same toward-zero rounding
// gotcha you handled in HW1: (-3) / 5 == 0 in C++, but -3 belongs in
// bucket -5.
std::map<int, int> histogram_int(const std::vector<int>& values,
                                 int bucket_width);

// Bin non-negative real values (e.g. distances from the origin) into
// buckets of the given width. The map key is the bucket's integer lower
// edge. No negative-rounding gotcha here -- distances are >= 0.
std::map<int, int> histogram_distance(const std::vector<double>& values,
                                      double bucket_width);

// Print a sideways bar chart: one row per bucket, rows sorted by bucket. A
// std::map iterates in sorted key order for free. SCALE the bars so the
// tallest is a fixed width (~56 '*') -- as many of you found in HW1, one
// '*' per count floods the terminal at K=10000. Here, scaling is part of
// the contract: find the largest count, then size each bar relative to it.
void print_histogram(const std::map<int, int>& h);
