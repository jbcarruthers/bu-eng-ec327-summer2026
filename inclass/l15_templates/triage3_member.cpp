// EC327 L15 activity -- TRIAGE SNIPPET 3 of 3.  THIS IS SUPPOSED TO FAIL.
// Build: g++ -std=c++20 -Wall -Wextra triage3_member.cpp -o t3
//
// This is the snippet where AI is ALLOWED -- but only AFTER you've tried the
// triage routine by hand, and only if you DISCLOSE it and RECOMPILE to verify
// the fix. Paste the first ~15 lines of the error + this code, and ask:
// "which of MY lines is wrong, and what's the smallest fix?"  The compiler is
// ground truth, not the chat.
//
// Hint to check yourself: the wall points INTO the template body and says
// member 'size' was requested on something of non-class type 'const int'.

#include <iostream>

template <typename T>
void describe(const T& x) {
  std::cout << "length is " << x.size() << '\n';  // <-- does every T have .size()?
}

int main() {
  describe(std::string{"hello"});  // fine: string has .size()
  describe(42);                    // <-- int has no .size()
  return 0;
}
