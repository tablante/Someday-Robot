#include "Felt.h"
#include "Config.h"

Felt::Felt(){
	int i;
	for (i=0;i<MAX_SEATS;i++){
		player_names[i] = string("n");
		player_status[i] = -1;
	}};

	string Felt::whos_sitting(int seat){
		return player_names[seat];		
	}

	int Felt::getActions(int player, int round, int step){
		return actions[player][round][step];
	}

	int* Felt::cards(){
		return community_cards;
	}

	void Felt::setDealer(int d){
		dealer=d;
	}

	void Felt::reset_round(){
		int i,j,k;
		int rounds = TH_ROUNDS;
		for(i=0;i<MAX_COMMUNITY_CARDS;i++){
			community_cards[i] = -1;
		}
		for(i=0;i<MAX_SEATS;i++){
			pot[i] = 0;
			if(player_status[i]==0)player_status[i] = 1;	}
		for(i=0;i<MAX_SEATS;i++){
			for(j=0;j<rounds;j++){
				for(k=0;k<MAX_RAISES;k++){
					actions[i][j][k] = -2;	}}}
		for(j=0;j<rounds;j++){action_depth[j]=0;}  //no actions to start
		for(i=0;i<TH_ROUNDS;i++){starters[i]=-1;	}
	}

	void Felt::setChips(int seat, int amt){
		chip_counts[seat]=amt;
	}

	void Felt::add_player(int place, string name, int chips){
		if(player_status[place] == -1){
			player_names[place]=name;
			player_status[place]= 0;
			chip_counts[place]=chips;
		}else{
			cout<<endl<<"ERORR, TRYING TO PUT PLAYER IN FELT SPOTS TAKEN";
		}
	}

	void Felt::remove_player(int place){
		player_status[place] = -1;
	}

	void Felt::fold_player(int seat){
		player_status[seat] = 0;
	}

	void Felt::add_community(int card){
		int i;
		for(i=0;i<MAX_COMMUNITY_CARDS;i++){
			if(community_cards[i]==-1){
				community_cards[i] = card;
				i = MAX_COMMUNITY_CARDS;
			}
		}
	}


	//used for printing the chart
	void Felt::set_first_bet_seat(int round, int seat){
		starters[round]=seat;
	}


	void Felt::add_action(int player_num, int round, int amount, int raise_lvl){


		//NEW METHOD
		actions[player_num][round][raise_lvl] = amount;
		if(raise_lvl+1 > action_depth[round])action_depth[round] = raise_lvl+1;
		//what does this do?


		if(amount>0){
			pot[player_num]+= amount;
			chip_counts[player_num]-=amount;	
		}
	}

	//print out a chart. take in two cards and show community.  this is only for texas hold'em at the moment...
	//to do: identify dealer, output cards properly
	void Felt::print_chart(int playernum, int card1, int card2, bool show_all, int* all_cards){
		//CLEAR THE CONSOLE
		int i,j,k,y,z,x, players_in, chart_length,temp_pot;
		players_in = 0;
		//top of the title
		cout<<endl<<"title:"<<endl;
		for (i=0;i<MAX_SEATS;i++){if(player_status[i] != -1){players_in++;}}
		chart_length = players_in * MAX_NAMELENGTH+ (MAX_SEATS- players_in)*5 + 5; //namelength for each player, 2 spaces for empty seats, 5 spaces for pot
		for(i=0;i<chart_length;i++){cout<<"-";}
		cout<<endl;
		//player names
		cout<<"|";
		for(i=0;i<MAX_SEATS;i++){
			if(player_status[i] == -1){cout<<"   |";}
			else{
				if(i==dealer){
					cout<<"(D)";}
				cout<<player_names[i];
				if(i==dealer){
					for(j=0;j<(MAX_NAMELENGTH-int(player_names[i].length()) - 3);j++)cout<<" ";
				}else{for(j=0;j<(MAX_NAMELENGTH-int(player_names[i].length()));j++)cout<<" ";}

				cout<<"|";
			}

		}
		cout<<"  Pot   |"<<endl; //extra space at the end for pots. 8 spaces

		//Now print their chipstacks
		cout<<"|";
		for(i=0;i<MAX_SEATS;i++){
			if(player_status[i] == -1){cout<<"   ";}
			else{
				for(j=0;j<START_OUTPUT;j++)cout<<" ";
				cout<<chip_counts[i];				
				for(j=0;j<MAX_NAMELENGTH - length(chip_counts[i]) - START_OUTPUT;j++)cout<<" ";
			}
			cout<<"|";
		}
		cout<<"        |"<<endl; //extra space at the end for pots. 8 spaces



		//need to print cards  of the player calling this fxn, or everyone if show_all
		cout<<"|";
		for(i=0;i<MAX_SEATS;i++){
			if(player_status[i] == -1){cout<<"   ";}
			else if(player_status[i] == 1 && i == playernum && show_all==false){
				for(j=0;j<START_OUTPUT;j++)cout<<" ";
				output_card(card1);
				cout<<" ";
				output_card(card2);
				for(j=0;j<MAX_NAMELENGTH - 5 - START_OUTPUT;j++)cout<<" ";
			}
			else{
				for(j=0;j<START_OUTPUT;j++)cout<<" ";
				if(player_status[i] == 1){
					if(show_all==false){cout<<"? ?";}
					else{output_card(all_cards[2*i]);
					cout<<" ";
					output_card(all_cards[2*i+1]);}
				}
				else{cout<<" - ";}
				if(show_all==false){for(j=0;j<MAX_NAMELENGTH - 3 - START_OUTPUT;j++)cout<<" ";}
				else{for(j=0;j<MAX_NAMELENGTH - 5 - START_OUTPUT;j++)cout<<" ";}
			}
			cout<<"|";
		}
		cout<<"        |"<<endl; //extra space at the end for pots. 8 spaces


		//now print their actions
		for(j=0;j<TH_ROUNDS;j++){
			if(j==1 && community_cards[2] != -1){//print flop if we have it
				cout<<"     ";
				output_card(community_cards[0]);
				cout<<"     ";
				output_card(community_cards[1]);
				cout<<"     ";
				output_card(community_cards[2]);
				cout<<endl;
			}else if(j==2 && community_cards[3] != -1){//print the turn
				cout<<"             ";
				output_card(community_cards[3]);
				cout<<endl;	
			}else if(j==3 && community_cards[4] != -1){//print the river
				cout<<"             ";
				output_card(community_cards[4]);
				cout<<endl;
			}

			for(k=0;k<action_depth[j];k++){ //L1
				cout<<"|";
				for(i=0;i<MAX_SEATS;i++){
					if(player_status[i] == -1){cout<<"   |";}else{
						//if they've folded, print a dash
						if(actions[i][j][k] == -1){for(z=0;z<START_OUTPUT;z++)cout<<" ";
						cout<<"-";
						for(z=0;z<MAX_NAMELENGTH - 1 - START_OUTPUT;z++)cout<<" ";
						cout<<"|";
						}else{
							//print their action
							for(z=0;z<START_OUTPUT-1;z++)cout<<" ";
							if(k==0 && starters[j] == i){cout<<">";}
							else{cout<<" ";}
							if(actions[i][j][k]==-2){cout<<"  ";} //unreached
							else{cout<<actions[i][j][k];}
							for(z=0;z<MAX_NAMELENGTH - length(actions[i][j][k]) - START_OUTPUT ;z++){cout<<" ";}
							cout<<"|";
						}
					}
				}
				temp_pot = 0;
				for(y=0;y<=j;y++){ //rounds
					for(z=0;z<MAX_SEATS;z++){  //seems right
						for(x=0;x<action_depth[y]&&(y<j || x<= k);x++){ //over raises
							if(actions[z][y][x]>0){temp_pot += actions[z][y][x];
							//					cout<<actions[z][y][x]<<"("<<z<<y<<x<<") + ";
							}}}}
				cout<<" "<<temp_pot;
				if(temp_pot==10){
					temp_pot=temp_pot;
				}
				for(z=0;z< 8 - length(temp_pot) - 1;z++)cout<<" ";
				cout<<"|"<<endl;
			}//L1
		}


	} //done with print_chart()

	int* Felt::whos_in(){
		return player_status;
	}

	int* Felt::what_pot(){
		return pot;
	}

	int* Felt::whos_got(){
		return chip_counts;
	}
	int Felt::raise_level(int round){
		return action_depth[round];	
	}  //returns what number raise you're on
