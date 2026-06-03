#include <iostream>
#include <cmath>

using std::cin;
using std::cout;

int main()
{
    double num, lognum;
    cout << "*Log Finder*\n";
    cout << "Please enter a number:";
    cin >> num;
    if(num >= 0){ //computes log only for positive numbers
        double lognum = std::log(num);
    } else {
        cout << "You entered a negative number!\n";
    }
    cout << "The natural logarithm of " << num << " is " << lognum << "\n";
    return 0;
}
