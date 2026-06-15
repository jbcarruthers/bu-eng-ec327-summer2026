#include "DataSet.h"

#include <random>
#include <vector>

#include "Walker2D.h"

// =============================================================
//  DataSet.cpp  --  provided complete. Generates the table you serialize.
// =============================================================

DataSet generate_dataset(int K, int N, unsigned seed) {
  std::mt19937 rng(seed);
  std::vector<Walker2D> walkers = run_trials(K, N, rng);

  DataSet data;
  data.reserve(walkers.size());
  for (const Walker2D& w : walkers) {
    data.push_back(WalkRecord{
        static_cast<double>(w.x()),
        static_cast<double>(w.y()),
        static_cast<double>(w.r_squared()),
        w.distance_from_origin(),
    });
  }
  return data;
}
