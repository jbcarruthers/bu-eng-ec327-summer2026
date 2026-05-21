#pragma once

#include <map>
#include <vector>

// =============================================================
//  Histogram.h  --  EC327 Summer 2026, Homework 1  (Part 4)
//
//  Binning a list of final positions into a histogram so you can
//  SEE the shape of the distribution.
// =============================================================

// Bin every value in `positions` into a bucket of width `bucket_width`.
// The key of the returned map is the bucket's lower edge; the value is
// the number of positions that landed in that bucket.
//
// PITFALL: negative-position bucketing is the Part 4 gotcha -- see the
// bucket_of() helper in the spec's "Hints and pitfalls" section.
std::map<int, int> histogram(const std::vector<int>& positions,
                             int bucket_width);

// Print a sideways bar chart: one row per bucket, one '*' per count,
// rows sorted by bucket.
void print_histogram(const std::map<int, int>& h);
