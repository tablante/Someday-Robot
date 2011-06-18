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


	EG_Helper file_help;
	//EG_Helper file_help;
	if(MACHINE==1){ //   mac \ freebsd

	}else if(MACHINE==2){ //
		file_help.initialize(
			"C:/Users/Bartley/Someday-Robot/data_files/simulation_spec.yaml",
			"C:/Users/Bartley/Someday-Robot/data_files/payoff_data.txt",
			"C:/Users/Bartley/Someday-Robot/data_files/features/");
		file_help.read_spec();
	}

	int start_chips = 100;
	int num_players = 2;
	int i;
	Table t= Table(&d, &f, 3, 10); //deck, players, starting amounts
	playas[0] = Player(start_chips, "Alice");
	playas[1] = Player(start_chips,"Bob");
	playas[2] = Player(start_chips, "Eve");
	playas[3] = Player(start_chips, "P4");
	playas[4] = Player(start_chips, "P5");
	playas[5] = Player(start_chips, "P6");
	playas[6] = Player(start_chips, "P7");
	playas[7] = Player(start_chips, "P8");
	playas[8] = Player(start_chips, "P9");

	string temp_strat;
	for(i=0;i<num_players;i++){
		t.seat_player(&playas[i]);
		if(!(i==0  && HUMAN_IN)){
			temp_strat= file_help.next_strategy(true);
			playas[i].updateStrategy(temp_strat);
		}
	}
	//t.seat_player(&playas[0]);
	//t.seat_player(&playas[2]);
	/*t.seat_player(&playas[3]);
	t.seat_player(&playas[4]);
	t.seat_player(&playas[5]);
	t.seat_player(&playas[6]);
	t.seat_player(&playas[7]);
	t.seat_player(&playas[8]);
	*/

	char option = 'y';


	// SECTION 1: STANDARD PLAYING
	if(HUMAN_IN){
	playas[0].setType(1); //1 sets Alice is controlled
	playas[0].updateStrategy(string("its_me_do_well"));
	}
	t.setAllActive();
	t.setDealer(2);//needs to be after all active

	i=0;
	int max_rounds = 10000;
	while (option != 'n' && option != 'N' && option != '0' && i < max_rounds){
		t.play_round_th();
		
		i++;
		if(HUMAN_IN){
			cout<<endl<<"Would you like to play again? Y/N/0: ";
			cin>>option;
		}
	}

	float payoff;
	for(i=0;i<num_players;i++){
		payoff = float(playas[i].getMoney()-start_chips)/float(max_rounds);
		temp_strat = playas[i].getStrategy();
		file_help.add_payoff(temp_strat, 
			payoff);
	}

	file_help.write_payoffs();
	file_help.finish();

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
	ofstream test("C:/Users/Barctley/Desktop/Research/poker_data/test.txt", ios::out);
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
