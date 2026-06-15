#include <filesystem>
#include <iostream>

#include "DataSet.h"
#include "csv_writer.h"
#include "npy_writer.h"

// =============================================================
//  hw3_main.cpp  --  EC327 Summer 2026, Homework 3
//
//  The driver. It generates the dataset (HW2 simulation, provided) and asks
//  your two writers to serialize it. Most of this is written for you; the
//  TODOs mark the two calls you wire up once your writers exist.
//
//  Reproducibility: the seed is FIXED so the dataset is identical every run.
//  That is what lets the Python test suite check the actual numbers. Do not
//  reseed with std::random_device here -- the test depends on K, N, and the
//  seed being the documented values below.
// =============================================================

namespace {
constexpr int kNumWalkers = 5000;  // K rows
constexpr int kNumSteps = 200;     // N steps per walker
constexpr unsigned kSeed = 327;    // fixed -- do not change
}  // namespace

int main() {
  std::filesystem::create_directory("data");

  const DataSet data = generate_dataset(kNumWalkers, kNumSteps, kSeed);
  std::cout << "Generated " << data.size() << " rows x " << kNumColumns
            << " columns (K=" << kNumWalkers << ", N=" << kNumSteps << ").\n";

  // Part 1: write data/walks.csv with write_csv(). Check the return value
  //   and report success/failure.
  // TODO: if (write_csv("data/walks.csv", data)) ... else ...

  // Part 2: write data/walks.npy with write_npy(). Check the return value.
  // TODO: if (write_npy("data/walks.npy", data)) ... else ...

  std::cout << "Now run:  python3 test_interop.py\n";
  return 0;
}
