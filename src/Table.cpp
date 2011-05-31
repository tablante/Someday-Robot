#include "Table.h"
#include <iostream>


Table::Table()   //DO NOT USE, UNSUPPORTED
{}

Table::Table(Deck* deck_to_use, Felt* f, int num_players, int starting_amounts){
	int i;
	for (i=0;i<5;i++){
		cards[i] = -1;
	}
	for(i=0;i<MAX_SEATS;i++){
		seats[i] = 0;
		active[i]=false;
	}
	cards_out = 0;
	dealer=0;
	t_deck = deck_to_use;
	t_felt = f;
	stats_on = false;
}


void Table::setAllActive(){
	int i;
	for(i=0;i<MAX_SEATS;i++){
		if(seats[i]==1 && the_players[i]->getMoney()>0){
			active[i]=true;
		}
	}
}


void Table::start_collecting(){
	stats_on=true;

	int i,j;
	for(i=0;i<52;i++){
		for(j=0;j<52;j++){
			stats[i][j][0]=stats[i][j][1]=0;
		}
	}
}

void Table::record_stats(){
	int i,j;

	if(MACHINE==2){
		ofstream test("C:/Users/Bartley/Desktop/Research/poker_data/test.txt", ios::out);
		for(i=0;i<52;i++){
			for(j=0;j<52;j++){
				if(stats[i][j][0]>0)test<<i<<", "<<j<<", "<<stats[i][j][0]<<", "<<stats[i][j][1]<<endl;
			}
		}

		test.close();

	}else{

		cout<<" ERRORORROROR, undefined machine for file i/o";
	}


}

void Table::print_center(){
	for(int i=0;i<cards_out;i++){
		cout<<"Card: "<<i+1<<": "<<cards[i];
	}

}
void Table::add_card(int card){
	cards[cards_out++]=card;
}
void Table::clear_table(){
	int i;
	for (i=0;i<5;i++){
		cards[i] = -1;
	}
	cards_out = 0;
}


//we should tell the player where he sits... someday
void Table::seat_player(Player* p, int money){
	int i;
	bool seated=false;
	for(i=0;i<MAX_SEATS;i++){
		if(seats[i] == 0){  //first open seat
			t_felt->add_player(i, p->getName(), money);
			seats[i] = 1;  //occupied
			the_players[i] = p;
			seated=true;
			i=MAX_SEATS;
		}
	}
	if(seated==false) {cout<<"no seat";	}
}

void Table::remove_player(int place){  //TO IMPLEMENT

}


/*	returns 0 if hands over.
round is 0 pre-flop, 1-flop, 2-turn, 3-river
*/
int Table::getBids(int starter, int round){
	int i,j,k, to_match, curr_act;
	bool resolved;
	resolved = false;
	j=0;  //j is the round depth
	int starter2=starter;
	bool started=false;
	//get bets from everyone. ROUND 0
	if(round==0&&currBigBlind < currSmlBlind){j=1;}//a fix to start next line
	for(i=starter;i!=(starter2) || started==false;i=((i+1)%MAX_SEATS)){//for(i=0;i<MAX_SEATS;i++){

		if(i== 0 && started==true){j++;} //original starter. moving to next round
		if(i==starter)started=true; //given a chance
		if(folded[i] == false  && the_players[i]->getMoney() > 0){ 
			//this will return either a -1 for fold, 0 for chk/fold, or a call (which is a fold if < amt)
			//this needs to be filtered later for appropriate bets
			curr_act = the_players[i]->action(i, *t_felt);
			
			th_actions[i][round][j] = curr_act;
			if(curr_act > 0){
				curr_bets[i] += curr_act;
				the_players[i]->takeMoney(curr_act);
			}
			if(the_players[i]->getMoney()<0){cout<<"warning, neg money!!!!";}
			t_felt->add_action(i,round,curr_act,j);
			to_match = 0;
			for(k=0; k<MAX_SEATS;k++){  //see if he's folded
				if(curr_bets[k]>to_match&&k!=i)to_match = curr_bets[k];
			}
			if(curr_bets[i] <to_match && (the_players[i]->getMoney() > 0)){folded[i] = true;}
			else if(curr_bets[i]>to_match){
				starter2 = i;}//re-raise, reset the betting
		}

	}

	j=0;
	for(i=0;i<MAX_SEATS;i++){
		if (folded[i]==false)j++;}//j becomes folks in
	return j-1;
}

//just sets dealer to next 2.  doesn't activate anyone. like advanceDealer
void Table::setDealer(int d){
	dealer = d;
	int i;
	bool started=false;
	currSmlBlind = currBigBlind = -1;
	for(i=(d+1)%MAX_SEATS; i!=(d+1)%MAX_SEATS || (started==false) ;i=(i+1)%MAX_SEATS){
		if(i==d+1){
			started=true;
		}
		if( active[i] ){
			if(currSmlBlind == -1){
				currSmlBlind = i;
			}else if(currBigBlind==-1){
				currBigBlind=i;
				i=(d+1)%MAX_SEATS;
			}
		}
	}
	if(currBigBlind==-1){
		cout<<"error, tried to set dealer when 1 person at table";
	}
}

//make sure we're all set
bool Table::checkToDeal(){
	if((active[currSmlBlind] && active[currBigBlind])==false){
		return false;
	}else{
		return true;}
}

int Table::advanceDealer(){  /*always have a big blind */
//either, all fine- last small and blind were assigned
//last hand had no small
//last hand had no big

	int i,k;
	int lastSml = currSmlBlind;
	int lastBig = currBigBlind;
	if(active[currBigBlind] && the_players[currBigBlind]->getMoney() > 0){
		currSmlBlind=currBigBlind;
		for(i=(currSmlBlind+1)%MAX_SEATS; i!=currSmlBlind; i=(i+1)%MAX_SEATS){
			if(active[i] || (seats[i]==1 &&the_players[currBigBlind]->getMoney() > 0)){
				currBigBlind=i;
				active[i]=true;  //time to get in
				i=currSmlBlind-1;
			}
		}
		if(currSmlBlind == currBigBlind){
			cout<<"error, small blind big are same";
		}
	}else{//no small blind
		currSmlBlind=-1;
		k=currBigBlind;
		for(i=(k+1)%MAX_SEATS;i!=k;i=(i+1)%MAX_SEATS){
			if(active[i] || (seats[i]==1 &&the_players[currBigBlind]->getMoney() > 0)){
				currBigBlind=i;
				active[i]=true;
				i=k;
			}
		}
	}
	if(currSmlBlind!=-1){
		dealer = lastSml;
	}else{
		dealer = (currBigBlind-1)%MAX_SEATS;
	}
	return dealer;
}

/* Assuming HOLD'EM */
void Table::play_round_th(){
	
	if(!checkToDeal()){
		cout<<"error, something's wrong";
		return;
	}

	int i,j,k;
	pot = 0;
	int player_cards[2*MAX_SEATS];

	//RESETSTUFF
	t_deck->shuffle();
	if(checkToDeal()==0){
		return;
	}
	int card1,card2,card3,card4,card5;
	int burnt1, burnt2;
	card1 =card2=card3=card4=card5=burnt1=burnt2=0;
	t_felt->reset_round();
	for(i=0;i<MAX_SEATS;i++){
		curr_bets[i] = 0;
		if(seats[i]==1)t_felt->setChips(i, the_players[i]->getMoney());//update felt
		for(j=0;j<TH_ROUNDS;j++){for(k=0;k<MAX_RAISES+1;k++){
			th_actions[i][j][k] = -3;
		}}}


	//old: t_felt->set_first_bet_seat(0,0);
	t_felt->setDealer(dealer);


	//1: give everyone their cards
	for(i=0;i<MAX_SEATS;i++){
		if(active[i]){ //give them a card. TODO: start left of dealer

			if(USE_BLINDS){
				if(i==currSmlBlind){ //small blind
					if(DEF_SML_BLIND > the_players[i]->getMoney()){
						k=the_players[i]->getMoney();
					}else {k=DEF_SML_BLIND;}
					th_actions[i][0][0] = k;
					curr_bets[i] = k;
					the_players[i]->takeMoney(k);
					pot += k;
					t_felt->add_action(i,0,k,0);
				}else if(i==currBigBlind){ //big blind
					if(DEF_BIG_BLIND > the_players[i]->getMoney()){
						k=the_players[i]->getMoney();
					}else {k=DEF_BIG_BLIND;}

					curr_bets[i] = k;
					the_players[i]->takeMoney(k);
					pot += k;
					if(currBigBlind < currSmlBlind){
					th_actions[i][0][1] = k;
					t_felt->add_action(i,0,k,1);}
					else{
					th_actions[i][0][0] = k;
					t_felt->add_action(i,0,k,0);}
				}


			
			}else{
				th_actions[i][0][0] = DEF_ANTE;
				curr_bets[i] = DEF_ANTE;
				the_players[i]->takeMoney(DEF_ANTE);
				pot += DEF_ANTE;
				t_felt->add_action(i,0,DEF_ANTE, 0);
			}


			card1 = t_deck->draw_card(-5);
			card2 = t_deck->draw_card(-5);
			folded[i]=false;
			the_players[i]->give_cards(card1, card2);
			player_cards[2*i] = card1;
			player_cards[2*i+1] = card2;
		}else{
			folded[i]=true;}
	}


	//2: elicit an action pre-flop
	//j=1;
	if(USE_BLINDS){
		if(currSmlBlind !=-1){//this is first seat?? TOCHECK
			t_felt->set_first_bet_seat(0,currSmlBlind);
		}else {t_felt->set_first_bet_seat(0,currBigBlind);}
		//old: t_felt->set_first_bet_seat(0,(dealer+3)%MAX_SEATS); //this sets first actor
		getBids((currBigBlind+1)%MAX_SEATS,0);}
	else{  //start left of dealer
		t_felt->set_first_bet_seat(0,(dealer+1)%MAX_SEATS); 
	getBids((dealer+1)%MAX_SEATS,0);

	}

	cards[0] = t_deck->draw_card(0);
	cards[1] = t_deck->draw_card(0);
	cards[2] = t_deck->draw_card(0);
	t_felt->add_community(cards[0]);
	t_felt->add_community(cards[1]);
	t_felt->add_community(cards[2]);

	//3: elicit a POOOOOOOOOOOOOOOOOOOOOST FLOOOOOOOOOOOP ACTION
	if(still_in()>1){
		if(currSmlBlind !=-1){//this is first seat?? TOCHECK
			t_felt->set_first_bet_seat(1,currSmlBlind);
		}else {t_felt->set_first_bet_seat(1,currBigBlind);}
		if(currSmlBlind!=-1){getBids(currSmlBlind,1);}
		else{getBids(currBigBlind,1);}
	}

	// THE TURN
	cards[3] = t_deck->draw_card(0);
	t_felt->add_community(cards[3]);

	if(still_in()>1){
		if(currSmlBlind !=-1){//this is first seat?? TOCHECK
			t_felt->set_first_bet_seat(2,currSmlBlind);
		}else {t_felt->set_first_bet_seat(2,currBigBlind);}
		if(currSmlBlind!=-1){getBids(currSmlBlind,2);}
		else{getBids(currBigBlind,2);}
	}


	//4: elicit a post-turn action


	cards[4] = t_deck->draw_card(0);
	t_felt->add_community(cards[4]);

	if(still_in()>1){
		if(currSmlBlind !=-1){//this is first seat?? TOCHECK
			t_felt->set_first_bet_seat(3,currSmlBlind);
		}else {t_felt->set_first_bet_seat(3,currBigBlind);}
		if(currSmlBlind!=-1){getBids(currSmlBlind,3);}
		else{getBids(currBigBlind,3);}
	}



	//6: evaluate winner, reassign everything
	cout.precision(11);
	if(!stats_on)t_felt->print_chart(0, 0, 0, true, player_cards);

	float win_hand, hand_val[MAX_SEATS];
	int winners,smallest_stack,  to_allocate, winnings, total_pot, max_in;
	bool leftover;
	max_in = winners =  total_pot = 0;
	win_hand = 0;
	for(i=0;i<MAX_SEATS;i++){
		total_pot += curr_bets[i]; //set pot
		if(max_in < curr_bets[i])max_in = curr_bets[i];
		if(folded[i]==false){
			cards[5]= player_cards[2*i];
			cards[6]= player_cards[2*i+1];
			hand_val[i] = float(eval_7(cards));
			if(hand_val[i]>win_hand){ //this line just for stats.
				win_hand=hand_val[i];
				winners=1;
			}else if(hand_val[i]==win_hand){ //just for stats. actual has to be looped
				winners++;
			}
			if(!stats_on)cout<<endl<<the_players[i]->getName()<<" had: "<<hand_val[i];
		}else{hand_val[i] = -1;}
	}

	if(stats_on&&winners==1){  //record hands
		for(i=0;i<MAX_SEATS;i++){
			if(folded[i]==false){
				if(player_cards[2*i]< player_cards[2*i+1]){
					stats[player_cards[2*i]][player_cards[2*i+1]][0]++;}
				else{stats[player_cards[2*i+1]][player_cards[2*i]][0]++;} 
			}

			if(win_hand ==hand_val[i]){
				if(player_cards[2*i]< player_cards[2*i+1]){
					stats[player_cards[2*i]][player_cards[2*i+1]][1]++;}
				else{stats[player_cards[2*i+1]][player_cards[2*i]][1]++;} 
			}}
	}

	while(total_pot > 0){
		winners=0;
		win_hand=0;
		for(i=0;i<MAX_SEATS;i++){  //find winners
			if(folded[i]==false){
				if(hand_val[i]>win_hand){
					winners=1;
					win_hand = hand_val[i];
				}else if(hand_val[i]==win_hand){
					winners++;
				}
			}
		}
		to_allocate = 0;//chip stacks left
		smallest_stack = total_pot;
		for(i=0;i<MAX_SEATS;i++){  //tiniest player
			if(curr_bets[i] >0){
				to_allocate++;
				if(smallest_stack > curr_bets[i])smallest_stack = curr_bets[i];
			}
		}
		leftover=false;
		if((smallest_stack*to_allocate)%winners !=0)leftover=true;
		winnings = int(smallest_stack*to_allocate/winners);
		for(i=0;i<MAX_SEATS;i++){
			curr_bets[i]-=smallest_stack;
			if(hand_val[i]==win_hand){
				if(leftover){the_players[i]->takeMoney(-1*winnings-1);
				leftover=false;}
				else {the_players[i]->takeMoney(-1*winnings);}
			}
			if(curr_bets[i]<=0)folded[i]=true;
		}
		total_pot -= smallest_stack * to_allocate;

	}

	//update seats. drop losers.
	for(i=0;i<MAX_SEATS;i++){
		if(seats[i]==1 && the_players[i]->getMoney() ==0){
			cout<<endl<<"Dropping "<<the_players[i]->getName();
			active[i]=false;}
	}

	advanceDealer();

	//iterate dealer. don't skip small blind or big blind.

} //end TH_ROUND

int Table::still_in(){
	int i,j;
	for(i=j=0;i<MAX_SEATS;i++){
		if(!folded[i])j++;
	}
	return j;
}


/* identify first move 
deal cards
solicit bidding.
wait for bidding to stop
see if winner exists
deal card?
see if winner exists
assign victory
*/
