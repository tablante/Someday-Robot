#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string.h>
#include <iostream>
#include "Player.h"
#include "Deck.h"
#include "Config.h"
#include "Felt.h"
#include "Evaluators.h"
#include <fstream>

using namespace std;



class Player;
class Table{
public:
		Table();
		void print_center();
		void add_card(int card);
		void clear_table();
		Table(Deck* deck_to_use, Felt* f, int num_players, int starting_amounts);  //most of this unused
		void play_round_th();
		int advanceDealer();//investigates blinds situation
		/* a precreated player p joins the table in spot n */
		void seat_player(Player* p, int money=10);
		
		/* int* should be a pointer to the number of players. if 3 players [0,1,0] means show third player */
		void ShowPlayers(int* num_players);  //IS THIS USED? DELETE?

		/* 1 if good, 0 if not */
		bool checkToDeal();
		void setDealer(int d);
		void setAllActive();

		void start_collecting(); //turn on data collection
		void record_stats(); //finish data collection
		
		int still_in(); //return # players havent folded

		void remove_player(int place);

/*
	Cards range from 0-51
	0-12 2-Ace Spades
	13-25 Clubs
	26-38 Diamonds
	39-51 Hearts

	Max raises:


*/
private:
	
	int stats[52][52][2];
	bool stats_on;
	int stats_type;  //to implement later

//TABLE INFORMATION- MID GAME

	//-3 initialized, -2 is out, -1 is folded, otherwise it's the bet
	int th_actions[MAX_SEATS][TH_ROUNDS][MAX_RAISES+1];
	int curr_bets[MAX_SEATS];  //just the money they've put in, regardless of status.
	int cards[7];
	int cards_out;
	Felt* t_felt;
	int pot;  //felt and table keep track separately. maybe redundant but w/e
	int getBids(int starter, int round);  //returns 1 if we should continue, 0 if not


//TABLE INFORMATION- ACROSS GAMES
	bool active[MAX_SEATS]; //false as they sit. true when they're bb.
	int seats[MAX_SEATS];/* just a placeholder, 0 is empty 1 is filled */
	bool folded[MAX_SEATS];  //true or false, false if they're in
	int dealer; /* seats[dealer] should be 1 */
	int next_small, next_big; //keep track of who's up for the blinds.
	Deck* t_deck;
	Player* the_players[MAX_SEATS];
	int currBigBlind; //seat indices
	int currSmlBlind;

	/* cards are intilized to -1, 0 is burnt, 1-n is the player they've been dealt to */

};

#endif
