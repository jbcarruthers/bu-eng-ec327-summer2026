// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: Lab 2 format/lint target — correct output, deliberately messy code.
// Topic: style
// Category: shell
// Lecture: lab2
//
// This program WORKS. Build it and it prints the right answer. The problem is
// everything else: the spacing is cramped, the braces are wherever, and it
// trips four clang-tidy checks. Your job in Parts 1-2 is to turn it into a
// fixed.cpp that clang-format leaves alone and clang-tidy accepts — WITHOUT
// changing what it prints.
#include<iostream>
#include<vector>
using namespace std;
int main(){
vector<int> v={4,8,15,16,23,42};
int total=0;
for(size_t i=0;i<v.size();i++){
total+=v[i];
}
int* first=NULL;
if(!v.empty()){first=&v[0];}
bool finished=true;
if(finished==true){
cout<<"sum = "<<total<<endl;
cout<<"first = "<<*first<<endl;
}
return 0;
}
