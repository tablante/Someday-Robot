#include "Deck.h"
#include <iostream>

Deck::Deck()
{
	int i;
	for(i=0;i<=51;i++){
		cards[i] = -1;
	}

	cards_remaining = 52;
}


int Deck::draw_card(int place){
	int card_num,new_card,i;
	if(cards_remaining == 0){
		cout<<"no cards left";
		return -8;
	}
	if(cards_remaining < 0){
		cout<<"negative cards left";
		return -2;
	}
	card_num = rand()%(cards_remaining);
	//	cout<<"drawing "<<card_num<<"card\n";
	for(new_card= i = 0; i<card_num || cards[new_card] != -1; new_card++){
		if(cards[new_card] == -1){i++;}
	}
	cards[new_card] = place;
	cards_remaining = cards_remaining-1;
	return new_card;
}

void Deck::shuffle(){
	int i;
	for(i=0;i<=51;i++){
		cards[i] = -1;
	}
	cards_remaining = 52;
}



int Deck::cards_left(){
	return cards_remaining;
}
void Deck::print_hand(int hand[5]){
	//cout<<"";
		
	return;

}