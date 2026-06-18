#pragma once
#include <optional>
#include <string>
#include <vector>

namespace gradebook {

struct Student {
  std::string name;
  std::vector<int> scores;
};

// Parse one "Ada,90,85" row. Returns nullopt on a malformed row
// (no name, or a non-integer score).
std::optional<Student> parse_row(const std::string& line);

double mean(const Student& s);  // average score, 0..100
char letter(double pct);        // A/B/C/D/F

}  // namespace gradebook
