#ifndef PLAYER_H
#define PLAYER_H



#include "Config.h"

#include "Table.h"
#include <iostream>
#include <string>
#include <math.h>
#include "Felt.h"


class Table;  //forward declaration I guess



class Player{
public:
		Player(int money, string myname);
		Player();
		void give_cards(int card1, int card2);
		int action(int location, Felt f, bool canRaise = true); //return a -1 for fold, else number for bet/chk. this calls other action fxns
		string getName();
		void setType(int myType); //for now, 0 just does check/Fold and 1 does input your bet
		void updateStrategy(string strat); //also sets type
		
		string getStrategy();

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
	
	int money;
	bool automated; //not sure how used, sort of a strategy thing.
	//int strategy;
	string strategy; // struct_strat_ZZ_P#_param1_param2
	vector<string> strat_params;
	string struct_strat;

	string name;
	int type;  /* a short for struct_strat
			   1 is active, uses I/O. 
			   0 always calls. 
			   >1 does diff stuff. handled by action()
		*/
	bool initialized;  //strategy dependent

	/* Data structures that will help to process one deal/round */
	int cards[7];  //0 and 1 are mine.
	int other_probs[1326]; //to get a,b. a<b. others_index: 52*a - (a^2+3a+2)/2 +b
	//these are for evaluatin' hands
	int myGHs[10];
	double myProbs[10];
	double probs_tie_win[10];
	double probs_lose[10];

	float pocket_vals[169][MAX_SEATS-1][2]; 
	/*how to read pocket_vals:
		1: cards = 325 = 13 (lower)*13*unstd + 13*12 suited
			unsuited: 13*lower-(lower^2+lower)/2+higher
			suited: 90+13*lower-(lower^2+3*lower)/2+higher
		2: #other players - 1, 0->2 heads up, 7->9 players
		3: # order. 0 is roll-out value, 1 is dynamicity.
	*/

	/* compiles results */
	void eval_th_o(int* my_cards, int numCards, double* probs_tie_win, double* probs_higher);

	/* cards are intilized to -1, 0 is burnt, 1-n is the player they've been dealt to */

	inline int others_index(int lower, int upper){
		return (52*lower - (lower*lower + 3*lower + 2)/2 + upper);
	}

	void initialize_probs(int style=0);

	void get_external_data();


	
	

/**** ACTION HELPERS    *****/

	
	double prob_win(); //based on myProbs, probs_tie_win, and probs_lose, calc prob of winning 1v1
	int act_call(int location, Felt f);  //called by player type 0. determines amt to raise
	int human_action(int s, Felt f); //called if it's a human. does all human interaction

	//one version AI. the random adds randomness if the player requests it
	int mr_probability_act(int seat, Felt f, bool prand=false);
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


/*
	Types >1

	Rounds: pre-flop (pf), flop (f), turn (t), river (r)

	2:	Name: mr_probability
		Strategy:
			Looks (1) strength of what we have now, (2) strength of what we might get
			probabilistically looks at chance of winning given a random pocket hand
			assuming everyone else stayed in
			and raises/calls accordingly
		Parameters: 18
				use_rand, prob_rand, spont_rand, pf_rs, pf_initial_rs, pf_cl, (6)
				f_rs, f_initial_rs, f_cl, f_spont (4)
				t_rs, t_initial_rs, t_cl, t_spont (4)
				r_rs, r_initial_rs, r_cl (3)
			Restrictions:
				rs > initial_rs > cl, rands are mean 0 uniform [-rand/2, rand/2] <0.6
	3:	Name: sr_markov
		Strategy:

		Parameters:
			

*/