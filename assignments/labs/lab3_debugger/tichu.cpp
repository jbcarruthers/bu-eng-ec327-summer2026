// Copyright 2021 J Carruthers jbc@bu.edu
// Tichu Dealer

// This program constructs a Tichu
// deck and deals 14 cards to each of 4 players

#include<iostream>
#include<vector>
#include<string>
using std::vector;
using std::string;
using std::cin;
using std::cout;

const int NCARDS=14;
const int NPLAYERS=4;

const vector<string> values{"2","3","4","5","6","7","8","9","T","J","Q","K","A"};
const vector<string> suits{"Jade","Sword","Pagoda","Star"};
const vector<string> specialcards{"Dog","Sparrow","Dragon","Phoenix"};


typedef int Card;
typedef vector<Card> Hand;


string show_card(Card c) {
  if (c>52) 
    return specialcards.at(c-100);
   else 
      return values.at(c%13) + " " + suits.at(c/13);
}

int main() {
  int n;
  vector<Card> deck{100,101,102,103};

  for (int i=0; i<52; i++)
    deck.push_back(i);

  // show the original deck
  for (Card c: deck)
     cout << c << " " << show_card(c)<< "\n";
  
  
  // deal the cards into 4 hands.
   vector<Hand> hands;
   for (int player=0; player<NPLAYERS; player++) {
    Hand h;
    for (int i=0; i<NCARDS; i++) {
      h.push_back(player*NCARDS + i);
    }
    hands.push_back(h);
   }

   for (auto h : hands){
     cout << "player\n";
     for (auto c : h)
       cout << show_card(c) << "\n";  // need a breakpoint here.
     cout << "\n";
   }
}