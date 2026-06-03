// Copyright 2021 J.Carruthers jbc@bu.edu

// Counting sheep

#include<iostream>
#include<cstdint>
using std::cout;
using std::cin;


int main() {
  std::int16_t i = 1;
  int counter=1;
  while (i != 0) {
    i++;
    cout << i*1000 << "\n";
    counter++;
  }
  return 0;
}
