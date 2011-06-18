#ifndef DECK_H
#define DECK_H

//using namespace std;
#include "Config.h"



class Deck{
public:
		Deck();
	/*	-2 is the place		*/
		int draw_card(int place);
		void print_hand(int hand[5]);
		void shuffle();
		int cards_left();

/*
	Low Two bits will be suits. is this still true?
		then 4 bits to determine size
		0-3 are 2s. Spades Clubs Diamonds Hearts
		4-7 are 3s
		48-51 are aces
*/
private:
	int cards[52];
	int cards_remaining;

	/* cards are intilized to -1, -5 is on the table,  1-n is the player they've been dealt to */

};

#endif