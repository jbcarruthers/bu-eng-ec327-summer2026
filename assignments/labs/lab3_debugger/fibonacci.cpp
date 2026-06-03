// Copyright 2021 J.Carruthers jbc@bu.edu

// Recursion

#include <iostream>
using std::cout;
using std::cin;

int count=0;

// calculate the fibonnaci number F_x
//

int fib(int x) {
  count++;
  if (x==0)
    return 1;
  if (x==1)
    return 1;

  return fib(x-1)+fib(x-2);
}
int main() {
  int N;
  cout << "Enter a number x to calculate F_x :";
  cin >> N;
  
  cout << "F_" << N << " = " << fib(N) << "\n";
  cout << "I used " << count << " function calls.\n";
  return 0;
}
