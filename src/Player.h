#ifndef PLAYER_H
#define PLAYER_H
#include "Table.h"
#include <iostream>
#include <string>
#include <math.h>
#include "Felt.h"
#include "Evaluators.h"

class Table;  //forward declaration I guess

using namespace std;

class Player{
public:
		Player(int money, string myname);
		Player();
		void give_cards(int card1, int card2);
		int action(int location, Felt f); //return a -1 for fold, else number for bet/chk. this calls other action fxns
		string getName();
		void setType(int myType); //for now, 0 just does check/Fold and 1 does input your bet
		int getMoney();  //return money amt
		void takeMoney(int amt);
/*
	Cards range from 0-51
	0-12 2-Ace Spades
	13-25 Clubs
	26-38 Diamonds
	39-51 Hearts
*/
protected:
	int cards[7];  //0 and 1 are mine.
	int money;
	bool automated;
	int strategy;
	string name;
	int type;  //1 is active, uses I/O. 0 always calls. >1 does diff stuff. handled by action()
	int other_probs[1326]; //to get a,b. a<b. index: 52*a - (a^2+3a+2)/2 +b
	//these are for evaluatin' hands
	int myGHs[10];
	double myProbs[10];
	double probs_tie_win[10];
	double probs_lose[10];



	/* compiles results */
	void eval_th_o(int* my_cards, int numCards, double* probs_tie_win, double* probs_higher);

	/* cards are intilized to -1, 0 is burnt, 1-n is the player they've been dealt to */

	inline int others_index(int lower, int upper){
		return (52*lower - (lower*lower + 3*lower + 2)/2 + upper);
	}

	void initialize_probs(int style=0);



	
	

/**** ACTION HELPERS    *****/

	
	double prob_win(); //based on myProbs, probs_tie_win, and probs_lose, calc prob of winning 1v1
	int act_call(int location, Felt f);  //called by player type 0. determines amt to raise
	int human_action(int s, Felt f); //called if it's a human. does all human interaction

	//one version AI. the random adds randomness if the player requests it
	int comp_action1(int seat, Felt f, bool prand=false);
	/* spontaneity: some measure of how chancy your hand is.  
	given the same prob_win,
	a hand with high spontaneity is good for calling, lower for raising 
	this needs a lot of work, needs to reflect my spontaneity vs table
	*/
	double spontaneity();

	//sets those to 0
	void update_probs(int card);

};

#endif
