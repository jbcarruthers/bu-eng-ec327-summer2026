// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: Capstone -- read a data file, parse it, write a formatted report.
//          File I/O (part 3) + std::format (part 2), put together.
// Topic: io-files
// Category: demo
// Lecture: L13

// L13, part 4 -- the whole pipeline most real programs are: READ structured
// input from a file, COMPUTE something, WRITE a human-readable report. Every
// piece here is from parts 1-3; this just assembles them.
//
//   in  : "city high low" per line     ->  parse with >>
//   out : an aligned temperature table ->  std::format column specs
//
// The report is written to a file AND echoed to the terminal -- by sending the
// same std::format-ed lines to two different streams. That is the payoff of
// "streams": the formatting code does not know or care where the bytes go.
//
// Build:  g++ -std=c++20 -Wall -Wextra -O0 -g l13_report.cpp -o l13_report
// Run:    ./l13_report      (writes weather.txt, then report.txt)

#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Day {
  std::string city;
  int high = 0;
  int low = 0;
};

const std::string kData = "weather.txt";
const std::string kReport = "report.txt";

// Make a small input file so the demo is self-contained.
bool seed_input() {
  std::ofstream out(kData);
  if (not out) {
    return false;
  }
  out << "Boston 81 64\n"
         "Phoenix 107 78\n"
         "Reykjavik 52 44\n"
         "Singapore 90 79\n";
  return true;
}

// Read the file into a vector<Day>. Returns an empty vector on open failure;
// the caller checks. The >> loop stops cleanly at end-of-file.
std::vector<Day> load(const std::string& path) {
  std::vector<Day> days;
  std::ifstream in(path);
  if (not in) {
    return days;  // empty
  }
  Day d;
  while (in >> d.city >> d.high >> d.low) {
    days.push_back(d);
  }
  return days;
}

// Build one formatted report line. Returning the string (instead of printing)
// is what lets us send it to BOTH the file and the terminal -- the formatting
// is decoupled from the destination.
std::string format_row(const Day& d) {
  const int range = d.high - d.low;
  //  city left in 12,  three right-justified numbers,  a unit suffix
  return std::format("{:<12} {:>4} {:>4} {:>5}F\n", d.city, d.high, d.low,
                     range);
}

}  // namespace

int main() {
  if (not seed_input()) {
    std::cerr << "could not write input file\n";
    return 1;
  }

  const std::vector<Day> days = load(kData);
  if (days.empty()) {
    std::cerr << std::format("no data read from {}\n", kData);
    return 1;
  }

  std::ofstream report(kReport);
  if (not report) {
    std::cerr << std::format("could not open {} for writing\n", kReport);
    return 1;
  }

  // The header and each row are built ONCE, then written to two streams.
  const std::string header =
      std::format("{:<12} {:>4} {:>4} {:>5}\n", "CITY", "HIGH", "LOW", "RANGE");
  std::cout << header;
  report << header;

  for (const auto& d : days) {
    const std::string row = format_row(d);
    std::cout << row;  // to the terminal
    report << row;     // ... and to the file -- same string, two endpoints
  }

  std::cout << std::format("\nreport for {} cities written to {}\n",
                           days.size(), kReport);
  return 0;  // report's destructor closes weather.txt's reader and report (RAII)
}
