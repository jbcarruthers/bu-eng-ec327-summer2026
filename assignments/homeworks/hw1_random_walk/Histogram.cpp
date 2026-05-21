#include "Histogram.h"

#include <iostream>
#include <map>
#include <vector>

// =============================================================
//  Histogram.cpp  --  implementations of the things declared in
//  Histogram.h. You write these in Part 4.
// =============================================================

std::map<int, int> histogram(
    [[maybe_unused]] const std::vector<int>& positions,
    [[maybe_unused]] int bucket_width) {
  // TODO (Part 4): bin every position into a bucket of width
  //   `bucket_width` and count how many positions landed in each.
  //
  //   PITFALL: integer division rounds toward zero in C++, so
  //   (-3) / 5 == 0 -- but position -3 belongs in bucket -5. See the
  //   bucket_of() helper in the spec's "Hints and pitfalls".
  return {};
}

void print_histogram([[maybe_unused]] const std::map<int, int>& h) {
  // TODO (Part 4): print one row per bucket -- the bucket label,
  //   then one '*' for each unit of count. A std::map iterates in
  //   sorted key order for free, so the rows come out in order.
}
