#include "gradebook.h"

#include <sstream>

namespace gradebook {

std::optional<Student> parse_row(const std::string& line) {
  std::stringstream ss(line);
  std::string field;
  if (not std::getline(ss, field, ',') or field.empty()) return std::nullopt;
  Student s;
  s.name = field;
  while (std::getline(ss, field, ',')) {
    try {
      s.scores.push_back(std::stoi(field));
    } catch (...) {
      return std::nullopt;  // a non-integer score -> bad row
    }
  }
  return s;
}

double mean(const Student& s) {
  if (s.scores.empty()) return 0.0;
  int total = 0;
  for (int x : s.scores) total += x;
  return double(total) / s.scores.size();
}

char letter(double pct) {
  if (pct >= 90) return 'A';
  if (pct >= 80) return 'B';
  if (pct >= 70) return 'C';
  if (pct >= 60) return 'D';
  return 'F';
}

}  // namespace gradebook
