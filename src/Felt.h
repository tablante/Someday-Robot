/*
	The felt contains everything the players and spectators can see.  Pot, chip stacks, etc.
	Each player only sees the felt when it's their turn, because turns are taken sequentially
*/

#ifndef FELT_H
#define FELT_H

#include <iostream>
#include <string>
#include "Config.h"

//this is manipulated by the table, and queried by the
class Felt{
public:
		Felt();

//CALLED BY PLAYER
		int* whos_in();  //returns list of players that are in
		int* whos_got();  //returns chip stacks.  these are net of pot, someone all in has 0
		int* cards();  //returns community cards
		int* what_pot();  //returns a number for each player
		int raise_level(int round);  //returns what number raise you're on
		string whos_sitting(int seat);
		
		//THIS NEEDS TO BE FIXED:
		int getActions(int player, int round, int step); //return the whole array. -2 tells not reached
		void print_chart(int playernum, int card1, int card2, bool show_all = false, int* all_cards=0);		//print a graph of the entire hand

// CALLED BY TABLE
		void reset_round();
		void add_community(int card);
		void add_player(int place, string name, int chips);
		void remove_player(int place);
		// , -1 fold | 0 check | 1 raise, then the amount
		void add_action(int player_num, int round, int amount, int raise_lvl);
		void set_first_bet_seat(int round, int seat);
		void fold_player(int seat); //for convenience, so felt doesn't calculate.
		void setChips(int seat, int amt);
		void setDealer(int d);

		
private:


		int community_cards[MAX_COMMUNITY_CARDS];  //-1 means not filled yet

		//current pot available for each player
		//as players go out pot becomes 0 for them, their raises are held by the pot
		//pots are additive, [0, 2, 4, 6, 6] means p1 is out, p2 can win 2, p3 can win 4
		int pot[MAX_SEATS];
		int dealer;
		int actions[MAX_SEATS][TH_ROUNDS][MAX_RAISES]; //number is amount put into pot that turn, -1 if folded, -2 if unreached
			//SHOULD THIS BE MAX_RAISES+1 ???????

		int action_depth[TH_ROUNDS]; //how much action for each round. this is one-indexed
		//just keeps track of who's got how much
		int chip_counts[MAX_SEATS];
		string player_names[MAX_SEATS];
		int player_status[MAX_SEATS]; //-1 is empty, 0 folded, 1 has cards
		int starters[TH_ROUNDS]; //sets who starts each round, -1 is uninitialized

};



#endif FELT_H
