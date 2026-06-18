#include "gradebook.h"

#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "usage: report FILE\n";
    return 1;
  }
  std::ifstream in(argv[1]);
  if (not in) {
    std::cerr << "cannot open " << argv[1] << '\n';
    return 1;
  }
  std::string line;
  double class_total = 0.0;
  int n = 0;
  while (std::getline(in, line)) {
    auto s = gradebook::parse_row(line);
    if (not s) continue;  // skip bad rows
    double avg = gradebook::mean(*s);
    class_total += avg;
    ++n;
    std::cout << s->name << ": " << gradebook::letter(avg) << " ("
              << avg << ")\n";
  }
  if (n > 0) std::cout << "class mean: " << class_total / n << '\n';
}
