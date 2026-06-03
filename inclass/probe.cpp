#include <cstdio>
int main(){int* d=new int(7); delete d; volatile int x=*d; printf("%d",x); return 0;}
