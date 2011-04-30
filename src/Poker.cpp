// RARGLGRAFHL.  (c) 2010, 2011 Bartley Tablante

/* GAME ABBREVIATIONS
	th: hold'em
	ohl: omaha hi/lo
	rz: razz
	s7: 7 card stud */

#include "Config.h"
#include <iostream>
#include <string.h>
#include <fstream>
//#include "Poker.h"
#include "Table.h"
#include "Felt.h"
#include "Deck.h"
#include <time.h>

using namespace std;

int main(int argc, char *argv[]){
/*   CREATE:
		-table
		-deck
		-probabilities (helper)
		-players
*/
Felt f;
Deck d;
Player playas[9];
srand( int(time(NULL)));


/* INITIALIZE:
	Setup then seat players
	Put Deck on table
	Specify an end condition, take it to the limit.*/
//   PLAYYYING

Table t= Table(&d, &f, 3, 10); //deck, players, starting amounts
playas[1] = Player(100000,"Bob");
playas[0] = Player(100000, "Alice");
playas[2] = Player(100000, "Eve");
playas[3] = Player(100000, "P4");
playas[4] = Player(100000, "P5");
playas[5] = Player(100000, "P6");
playas[6] = Player(100000, "P7");
playas[7] = Player(100000, "P8");
playas[8] = Player(100000, "P9");
t.seat_player(&playas[1]);
t.seat_player(&playas[0]);
t.seat_player(&playas[2]);
/*t.seat_player(&playas[3]);
t.seat_player(&playas[4]);
t.seat_player(&playas[5]);
t.seat_player(&playas[6]);
t.seat_player(&playas[7]);
t.seat_player(&playas[8]);
*/

char option = 'y';


// SECTION 1: STANDARD PLAYING
playas[0].setType(1); //1 sets Alice is controlled
playas[1].setType(2); //set to ai
playas[2].setType(2);
t.setAllActive();
t.setDealer(2);//needs to be after all active
while (option != 'n' && option != 'N' && option != '0'){
	t.play_round_th();
	cout<<endl<<"Would you like to play again? Y/N/0: ";
	cin>>option;
}


 //  PRE-FLOP HAND RANK DATA COLLECTION
/*
int i;
t.start_collecting();
for(i=0;i<1000000;i++){
	t.play_round_th();
}
t.record_stats();

*/

/*
// TEST eval_7(int*)

int cards[7];
int i;
double j;
cout<<"Hand 1\n";

cards[0]=42; 	cards[1]=39; 	cards[2]=51; 	cards[3]=44; 	cards[4]=31; 	cards[5]=36; 	cards[6]=0; 
j=eval_7(cards);
cout.precision(20);
cout<<j<<endl;

cout<<"Hand 2\n";
cards[0]=7; 	cards[1]=37; 	cards[2]=51; 	cards[3]=44; 	cards[4]=31; 	cards[5]=36; 	cards[6]=0; 
j=eval_7(cards);
cout.precision(11);
cout<<j<<endl;
cin>>i;
*/


/*
//TEST RANDOMNESS			C:\Users\Bartley\Desktop\Research\poker_data
//int  test_data[5][5];
int i,j;
ofstream test("C:/Users/Bartley/Desktop/Research/poker_data/test.txt", ios::out);
for(i=0;i<1000;i++){
	for(j=0;j<20;j++){
		test<<d.draw_card(0)<<", ";
	}
	test<<endl;
	d.shuffle();
}
test.close();
*/

return 0;

}
