#include <iostream>
using std::cout;

// iref is an integer and its the SAME one as in the calling program.

void fib(int & iref) {

}


int main() {
  // pNAME is a valid NAME of a var, but implies pointer to
  // pstudent
  //Student* pbeststudent;

  // declarations: * means "this is a pointer (an address)"
  // declarations: & means "this is a reference (same)"
  char* pc;
  double* pd;
  float* pf;
  int* pi;
  vector<int> v;
  char str[20];

  double total;

  char a = 'a';

  pc = &a;

  //i=fib(5);

  // expressions: * means "de-reference" or "go to this address" and "be" the
  // var there.

  // expressions:  & mean "address of" or "become this address"


  *pc = 'b';
  std::cout << a << "\n";

  return 0;
}