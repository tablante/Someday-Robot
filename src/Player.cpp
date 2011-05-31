#include "Player.h"

Player::Player(){
	cards[0] = -1;
	cards[1] = -1;
	money = 0;
	automated = false;
	initialized=false;
}

Player::Player(int endowment, string myname){
	cards[0] = -1;
	cards[1] = -1;
	money = endowment;
	automated = false;
	name=myname;
	type = 0; //default type
	get_external_data();
	initialized=true;
}

string Player::getName(){
	return name;
}

int Player::getMoney(){
	return money;
}

void Player::takeMoney(int amt){
	money = money-amt;
}

void Player::setType(int mytype){
	type = mytype;
	initialized=false;
}

void Player::give_cards(int card1, int card2){
	if(card1<card2){
		cards[0]  = card1;
		cards[1] = card2;}
	else{
		cards[0]=card2;
		cards[1]=card1;
	}
	initialize_probs();
	update_probs(cards[0]);
	update_probs(cards[1]);
}

void Player::update_probs(int card){
	int i;
	for(i=0;i<52;i++){
		if(i<card){
			other_probs[others_index(i,card)] = 0;
		}else if(i>card){
			other_probs[others_index(card,i)] = 0;
		}
	}			//they're never equal
}

//initialized to 2 cards
void Player::initialize_probs(int style){
	int i,j;
	int w; //level to initialize
	if(style==0){  //flat probabilities
		w = 1;
		for(i=0;i<52;i++){
			if(i!=cards[0] && i!=cards[1]){
				for(j=i+1;j<52;j++){
					if(j!=cards[0] && j!=cards[1]){
						other_probs[others_index(i,j)] = w;
					}
				}
			}
		}
	}
}

/* 
	Right now all we get is hand prior probabilities
*/
void Player::get_external_data(){
	ifstream file;

	if(MACHINE==2){
		file.open("C:/Users/Bartley/Desktop/Research/Poker/poker_data/starters.csv");
	}

	int i,j;
	int start,end;
	string line;
	//istringstream line_st;
	if(file.is_open()){
		for(i=0;i<169;i++){ //loop over all 2card pairs
			getline(file, line);  //get a line of data
			start=0;
			end = line.find(',');

			for(j=0;j<MAX_SEATS-1;j++){
				start = end+1;
				end = line.find(',',start);
				pocket_vals[i][j][0]= float(atof(line.substr(start,end-start).c_str()));
			}
		}

		file.close();
	}
}


/*
this iterates through all other cards
   0: highcard. 0.13 = ace, 0.01 = 2
	1: low pair.  if we have a pocket pair, this is zeroes
	2: pair- same as 4 kind
	3: two pair- 121050 means 2A2J, Js are mine. 121099 means As are mine, 121011 means i just have a K.
	4: 3 of a kind- same as 4kind
	5: straight- look at card contributing, else a 0
	6: flush- look at card contributing, else a 0
	7: full house- same as 2P, but trips staht
	8: 4 of a kind- 1199 means 4 of a kind with one of those in hand. 1112 means 4 of a kind on table, ace is MY kicker
	9: straight flush- look at card contributing. 11 means i would have a st flush with a pocket K
*/
void Player::eval_th_o(int* my_cards, int numCards, double* probs_tie_win, double* probs_higher){
	int i,j,k,x;
	int total_w = 0;
	int w;
	int temp_cards[7];
	for(i=2;i<7;i++){
		temp_cards[i]=my_cards[i];
	}
	int theirGHs[10], myGHs[10];
	double theirProbs[10];

	for(k=0;k<10;k++){
		theirProbs[k] = probs_higher[k] = probs_tie_win[k] = 0;
	}
	eval_th_m(my_cards, numCards, myProbs, myGHs);
	for(i=0;i<52;i++){
		for(j=i+1;j<52;j++){
			if(other_probs[others_index(i,j)] > 0){
				temp_cards[0]=i;
				temp_cards[1]=j;
				w= other_probs[others_index(i,j)];
				total_w += w;
				eval_th_m(temp_cards, numCards, theirProbs, theirGHs);
				for(k=8;k>=0;k--){
					if(theirProbs[k]==1){ //erase below
						for(x=k-1;x>=0;x--){theirProbs[x]=0;}
					}}
				
				//fix the pair rankings. mygh[2] > mygh[1]
				if(theirGHs[1]>myGHs[2]&&theirProbs[1]>0){//they have 2 uppers
					theirProbs[2]+=theirProbs[1];
					theirProbs[1]=0;
				}else if(theirGHs[2] < myGHs[1] && theirProbs[2]>0){ //2 lowers
					theirProbs[1]+=theirProbs[2];
					theirProbs[2]=0;
				}
				for(k=0;k<10;k++){
					if(theirGHs[k]>myGHs[k] || myProbs[k]==0){
						probs_higher[k]+=theirProbs[k]*w;
					}else{
						probs_tie_win[k]+=theirProbs[k]*w;
					}
				}
			}//done checking over meaningful cards
		}}//done iterating over all cards
	for(k=0;k<=9;k++){
		probs_higher[k]/=total_w;
		probs_tie_win[k]/=total_w;
	}

}

//seat can be thought of as "where am i"
//action should return a number that is how much they're putting in.  0 is check/fold, -1 is fold?
int Player::action(int seat, Felt f){
	/* debugging:*/
	int act;
		//need to decide:
	if(type==0){  //this type acts automatically
		act = act_call(seat, f);
	}else if(type==1){ //type = 1, uses io
		act = human_action(seat, f);
	}else if(type==2){
		act = comp_action1(seat, f,true);
	}

	/* let's just pick up how much we need to raise */
return act;
}

/*
	This returns the call that needs to be made
*/
int Player::act_call(int seat, Felt f){
	int act,i;
	int* pot;
	act = 0;
	pot = f.what_pot();
	//DBG cout<<"comparing"<<endl;
	for(i=0;i<MAX_SEATS;i++){
		if(pot[i] - pot[seat] > act){act = pot[i] - pot[seat];}
	//	cout<<i<<" bid"<<pot[i] - pot[seat]<<"  ";
	}
	return act;
}


//WORKHERE HERE HEREHERE
double Player::prob_win(){
	double prob_win,prob_lose;
	int high_achieve = 0; //start with high_Card
	int i,j;
	for(i=0;i<10;i++){
		if(myProbs[i]==1){
			high_achieve=i;
		}
	}
	prob_win=prob_lose=0;
/*	for(i=0;i<10;i++){
		if(i<high_achieve){ //have this beat
			prob_win += probs_tie_win[i] + probs_lose[i];
		}else if(i==high_achieve){
			prob_win += probs_tie_win[i];
			prob_lose += probs_lose[i];
		}
	}
*/
	for (i=9;i>=0 && (i==9 || myProbs[i+1]!=1); i--){ //iterate over myprobs
		for(j=0;j<10;j++){//iterate over opponent probs
			if(j<i){//i'd win
				prob_win += myProbs[i]*(probs_tie_win[j]+probs_lose[j]);
			}else if(j==i){//depends on type
				prob_win += myProbs[i]*(probs_tie_win[j]);
				prob_lose += myProbs[i]*probs_lose[j];
			}else{  //j>i, they'd win
				prob_lose += myProbs[i]*(probs_tie_win[j]+probs_lose[j]);
			}
		}
	}
	
	return prob_win/(prob_win+prob_lose);
}

double Player::spontaneity(){
	int high_achieve = 0; //start with high_Card
	int i;
	double spont = 0;
	for(i=0;i<10;i++){
		if(myProbs[i]==1){
			high_achieve=i;
		}
	}

	for(i=high_achieve+1;i<10;i++){
		spont +=  myProbs[i];
	}
	return spont;
}

int Player::human_action(int seat, Felt f){
	int act, i,r;
	char char_act;
	f.print_chart(seat, cards[0], cards[1]);
	int* tablecards = f.cards();
	for(i=0; i!= 5 && tablecards[i] != -1; i++){
		cards[2+i] = tablecards[i];
	} //i would be 0-preflop, 3-postflop, 4-postturn, 5-postriver
	if(i==3){
		update_probs(tablecards[0]);
		update_probs(tablecards[1]);
		update_probs(tablecards[2]);
	}else if(i==4){update_probs(tablecards[3]);}
	else if(i==5){update_probs(tablecards[4]);}
	if(FIXED_LIMIT){
		if(i<=3){
			r=DEF_BIG_BLIND;
		}else{r=DEF_BIG_BLIND*2;}
	}
	if(i>0){  //DISPLAY INFORMATION
		eval_th_o(cards, i+2, probs_tie_win, probs_lose);
		eval_th_m(cards, i+2, myProbs, myGHs);
		//output_my_future(myProbs, myGHs);
		if(HELP_PLAYER){
			output_my_future2(myProbs, myGHs, probs_tie_win, probs_lose);
			cout<<endl<<"Probability of winning is: "<<prob_win()<<"  Spontaneity is:"<<spontaneity()<<endl;
			cout<<"A robot would do: "<< comp_action1(seat,f)<<endl;
		}
	}
	if(FIXED_LIMIT){
		i=act_call(seat,f);
		char_act ='z';
		while(char_act!='c' && char_act!='C' && char_act!='r' && char_act!='R' 
			&& char_act!='f' && char_act!='F' && char_act!='0' && char_act!='1'
			&& char_act!='2'){
				if(i>0){ //need to call
					cout<<endl<<name<<": fold [f/F/0], call "<<i<<" [c/C/1] or raise "<<
						i+r<<"[r/R/2]: ";
				}else{
					cout<<endl<<name<<": fold [f/F/0], check "<<" [c/C/1] or raise "<<
						i+r<<" [r/R/2]: ";
				}
				cin>>char_act;
		}
		if(char_act == 'c' || char_act == 'C' || char_act == '1'){
			act = i;
		}else if(char_act == 'r' || char_act == 'R' || char_act == '2'){
			act = i+r;
		}else if(char_act == 'f' || char_act == 'F' || char_act == '0'){
				act=-1;
			}else{cout<<"ERROR, invalid entry somehow";}
		}else{

			cout<<endl<<"Enter "<<name<<"'s move: ";
			cin>>act;
		}
		return act;
}


int Player::comp_action1(int seat, Felt f, bool prand){
	int act, i,r,phase;
	int num_opponents_orig;
	int num_opponents_curr;
	char char_act;
	int* tablecards = f.cards();
	double spont, prob, advant;
	int* fplayers;
	fplayers = f.whos_in();
	num_opponents_orig = num_opponents_curr = 0;
	for(i=0; i<MAX_SEATS;i++){
		if(fplayers[i] == 0){ //0 is folded
			num_opponents_orig++;
		}else if(fplayers[i]==1){ //1 has cards
			num_opponents_orig++;
			num_opponents_curr++;
		}
	}
	num_opponents_orig--;  //  don't count yourself as an opponent
	num_opponents_curr--;  // "    "

	for(phase=0; phase!= 5 && tablecards[phase] != -1; phase++){
		cards[2+phase] = tablecards[phase];
	} //i would be 0-preflop, 3-postflop, 4-postturn, 5-postriver
	if(phase==3){
		update_probs(tablecards[0]);
		update_probs(tablecards[1]);
		update_probs(tablecards[2]);
	}else if(phase==4){update_probs(tablecards[3]);}
	else if(phase==5){update_probs(tablecards[4]);}
	if(FIXED_LIMIT){
		if(phase<=3){
			r=DEF_BIG_BLIND;
		}else{r=DEF_BIG_BLIND*2;}
	}
	if(phase==0){
		int hole_index=0;
		if( (cards[0]&3) == (cards[1]&3) ){ //low 2 bits same means same suit
			//13*lower-(lower^2+lower)/2+higher
			hole_index = 90+13*int(cards[0]/13) - (int(cards[0]/13)*(int(cards[0]/13)+3))/2 + int(cards[1]/13);
		}else{  //unsuited
				//90+13*lower-(lower^2+3*lower)/2+higher
			hole_index = 13*int(cards[0]/13) - (int(cards[0]/13)*(int(cards[0]/13)+1))/2 + int(cards[1]/13);
		}
		advant = pocket_vals[hole_index][num_opponents_orig-1][0] / double(num_opponents_orig+1.0);
		if(prand){
			advant+= double(rand()%50)/double(250);
		}
		spont=0; //TO FIX
	}
	if(phase>0){  //post flop
		eval_th_o(cards, phase+2, probs_tie_win, probs_lose);
		eval_th_m(cards, phase+2, myProbs, myGHs);
		prob=prob_win();
		spont=spontaneity();
		advant = pow(prob, num_opponents_orig);
		if(prand){
			spont+= double(rand()%100)/double(300);
			advant+= double(rand()%50)/double(250);
		}
	}
	if(FIXED_LIMIT){
		i=act_call(seat,f);

		if(phase==0){ //PRE FLOP
			if(advant>0.6){ //want to raise regardless
				if(f.raise_level(1)<3){
					act=i+r;
				}else{
					act=i;
				}
			}else if(advant>0.5){//want to do initial raise
				if(f.raise_level(1)<2){
					act=i+r;
				}else{
					act=i;
				}
			}else if(advant>0.3){ //willing to call
				act=i;
			}else{
				act=0;    //check fold
			}
		}else if(phase==3){
			if(advant>0.6){ //want to raise regardless
				if(f.raise_level(1)<3){
					act=i+r;
				}else{
					act=i;
				}
			}else if(advant>0.5){//want to do initial raise
				if(f.raise_level(1)<2){
					act=i+r;
				}else{
					act=i;
				}
			}else if(advant>0.3 && spont > 0.3){ //willing to call
				act=i;
			}else{
				act=0;    //check fold
			}
		}else if(phase==4){
			if(advant>0.6){ //want to raise regardless
				if(f.raise_level(2)<3){
					act=i+r;
				}else{
					act=i;
				}
			}else if(advant>0.5){//want to do initial raise
				if(f.raise_level(2)<2){
					act=i+r;
				}else{
					act=i;
				}
			}else if(advant>0.25 && spont > 0.2){ //willing to call
				act=i;
			}else{
				act=0;  //check fold
			}

		}else if(phase==5){
			if(advant>0.7){ //want to raise regardless
				if(f.raise_level(3)<3){
					act=i+r;
				}else{
					act=i;
				}
			}else if(advant>0.5){//want to do initial raise
				if(f.raise_level(3)<2){
					act=i+r;
				}else{
					act=i;
				}
			}else if(advant>0.3){ //willing to call
				act=i;
			}else{ //check fold
				act=0;
			}
		}else{
			cout<<"error, wrongphase123";
		}
	}else{ //done with limit, unknown how to deal with NL
		act=act_call(seat,f);
	}
	return act;
}