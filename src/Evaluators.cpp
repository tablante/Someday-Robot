/*
 *		Evaluators 

 */

#include "Evaluators.h"
#include <iostream>


/*		Higher is better.  additional cards are over 100. takes 5 cards or more.
	So flush J7532 = 5.1006040201
	0: highcard. 0.13 = ace, 0.01 = 2
	1: pair:
	2: two pair:
	3: 3 of a kind: 
	4: straight: 
	5: flush:
	6: full house
	7: 4 of a kind
	8: straight flush
*/
double eval_7(int mycards[7]){
	double value;
	int nums[13];
	int suits[4];
	bool cards[13][4];
	int i,j,type,st_counter;
	long long high_cnt;
	bool hit;
	for(i=0;i<13;i++){nums[i]=0;
		for(j=0;j<4;j++){cards[i][j]=false;}}
	for(i=0;i<4;i++){suits[i]=0;}
	for(i=0;i<7;i++){
		if(mycards[i]>=0){
		nums[mycards[i]/4]++;
		suits[int(mycards[i]%4)]++;
		cards[int(mycards[i]/4)][mycards[i]%4]=true;
		}
	}

	//check for flush (5) and st. flush (8)
	hit=false;
	value = 0;
	for(i=0;i<4;i++){
		if(suits[i]>4){hit=true;
		type = i;}} //this is the suit.
	if(hit==true){ //have a flush or straight flush, nothing else
		//check straight flush
		st_counter=0;
		hit =false;
		if(cards[12][type] == true && cards[0][type]==true)
			{st_counter=1;}//chk low st
		for(j=0;j<12;j++){//check st flush
			if(cards[j][type] == true && cards[j+1][type]==true){
				st_counter++;}
			else{st_counter = 0;}
			if(st_counter==4){
			hit=true;
			type=j+1;}//labels highest card
		} //done checking st flush
		if(hit==true){//st_flush
			value = 8+float(type)/100;
		}else{//flush. can't have FH or 4kind
			value=5;
			high_cnt=100;
			j=0;
			hit=true;
			for(i=12;i>=0;i--){
				if(cards[i][type]==true){
					value+= float(i)/float(high_cnt);
					high_cnt*=100;
					j++;
				}
				if(j==5){//all 5 found
					i=-1;}
			}//done counting flush
		}//done evaluating flush
	}else{//no flush, check 4kind, etc.
		for(i=0;i<13;i++){
			if(nums[i]==4){
				hit=true;
				value=7+float(i)/100;
				for(j=12;j>=0;j--){
					if(nums[j]>0 && nums[j]<4){
						value += float(j)/10000;
						j=-1;
					}
				}
			}
		}if(hit==false){//no 4kind, check FH then st
				for(i=12;i>=0;i--){
					if(nums[i]==3){
						for(j=12;j>=0;j--){
							if(j!= i && nums[j]>=2){
								value=6+float(i)/100+float(j)/10000;
								j=-1;
								hit=true;
							}
						}
					}
				}

			}//done chkn FH
	}//nothing above a flush.
	if(hit==false){
		st_counter=0;
		
		if(nums[12]>0&&nums[0]>0){st_counter=1;}
		for(i=0;i<12;i++){
			if(nums[i]>0&&nums[i+1]>0){st_counter++;}
			else{st_counter=0;}
			if(st_counter>=4){
				hit=true;
				value=4+(float(i)+1)/100;
			}
		}
	}
	if(hit==false){//no st. best is trips.
		for(i=0;i<13;i++){
			if(nums[i]==3){
				hit=true;
				value=3+float(i)/100;
				high_cnt=10000;
				for(j=12;j>=0;j--){
					if(nums[i]==1){
					value+= float(j)/float(high_cnt);
					high_cnt*=100;
					if(high_cnt==1000000)j=-1;//found all
					}
				}//done eval. trips
			}
		}
	}if(hit==false){ //2pr?
		for(i=12;i>=0;i--){
			if(nums[i]==2){
				for(j=i-1;j>=0;j--){
					if(nums[j]==2){
						hit=true;
						value=2+double(i)/100+double(j)/10000;
						j=i=-1; //exit both loops
					}
				}
			}
		}
		if(hit==true){//identify type of 2pair
			for(i=12;i>=0;i--){
				if(nums[i]==1){
					value+= double(i)/1000000;
					i=-1;
				}
			}
		}
	}if(hit==false){  //maybe 1p
		for(i=0;i<13;i++){
			if(nums[i]==2){
				value=1+double(i)/100;
				high_cnt=10000;
				hit =true;
				for(j=12;j>=0;j--){
					if(nums[j]==1){
						value+=double(j)/double(high_cnt);
						high_cnt *=100;
						if(high_cnt==10000000000){
							j=-1;
						}
					}
				}
			}
		}
	}if(hit==false){
		value=0;
		high_cnt=100;
		for(j=12;j>=0;j--){
			if(nums[j]==1){
			value+=double(j)/double(high_cnt);
			high_cnt *=100;
			if(high_cnt==10000000000){
				j=-1;
			}
	}
}
	}
	return value;
}

/*		
 *		Probabilistically Evaluate My hand:
 * my cards + whatever IS on the table (require at least 3 on table.)
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
//TOFIX: straight flush quality not assigned
//evaluate texas holdem my cards
//PRE CONDITION: cards 0 and 1 are mine. numcards are pocket+table.
void eval_th_m(int* zcards, int numCards, double* myProbs, int* myGHs){
	int nums[13];//what you have
	int for_st[13]; //what you need to hit that straight.  1 means this is all you need, 2 means you need this + another.
	int suits[4];
	bool cards[13][4];
	int i,j; //iterators
	int x,y,z; //keep track of card informations
	int high_cnt;
	int cards_to_go = 7 - numCards;
	int mycard_num1, mycard_num2;  //2 will always be weakly higher than 1.
	int mycard_suit1, mycard_suit2;

	//INITIALIZE:
	for(i=0;i<=9;i++){//set up hands
		myProbs[i] = 0;
		myGHs[i] = -1;
	}
	for(i=0;i<13;i++){ //set things to 0
		nums[i]=0;
		for_st[i]=0;
		for(j=0;j<4;j++){cards[i][j]=false;}}
	for(i=0;i<4;i++){suits[i]=0;}

	//set up the data sets to iterate over hands fast!
	for(i=0;i<numCards;i++){
		if(cards[i]>=0){
			nums[zcards[i]/4]++;
			suits[int(zcards[i]%4)]++;
			cards[int(zcards[i]/4)][zcards[i]%4]=true;
		}
	}

	if(zcards[0] > zcards[1]){  //card 0 is weakly greater
		mycard_num1 = zcards[1]/4;
		mycard_suit1 = zcards[1]%4;
		mycard_num2 = zcards[0]/4;
		mycard_suit2 = zcards[0]%4;
	}else{
		mycard_num1 = zcards[0]/4;
		mycard_suit1 = zcards[0]%4;
		mycard_num2 = zcards[1]/4;
		mycard_suit2 = zcards[1]%4;}

	//Straightflush:
	x = -1;  //require at least 3 of same suit, same req for flush.  this tracks 1 good suit.
	for(i=0;i<4;i++){
		if(suits[i] + cards_to_go >=5){ //can hit a flush in this suit.
			x=i; //uniquely finds possible suit.
		}
	}
	if(x>=0){ //defn possible flush, maybe not straight flush.  x keeps track of suit.
		for(i=12;i!=9;i=(i+1)%13){ //10 possible straights, highs of 5-A.  two scenarios: need 2 cards or need 1 card
			y = cards[i%13][x]+ cards[(i+1)%13][x]+cards[(i+2)%13][x]+cards[(i+3)%13][x]+cards[(i+4)%13][x];
	//  cards[12][x]+ cards[0][x]+cards[1][x]+cards[2][x]+cards[3][x]
			if(y+cards_to_go >=5){  //straight flush assignment
				if(mycard_suit2 == x && x == mycard_suit1 ){//both cards may potentially be involved
					if( (mycard_num2 == i )&&(i==12 ) ){  //straight starting with A
						if(mycard_num1==i || (mycard_num1>= (i+1)%13 && mycard_num1 <= (i+4)%13)){    //check if 1 counts
							myGHs[9] = mycard_num1;
						}
						else if(mycard_num2== (i%13 )|| (mycard_num2>= (i+1)%13 && mycard_num2 <= (i+4)%13)){ //if 2 counts, use it!
							myGHs[9] = mycard_num2;
						}else{ //both cards match, but they're not between. st flush on table, you have 7 flush cards.. strange, but whatever
							//could be code here
						}
					}
				}//end section of both cards matching suits
				else if(mycard_suit1 == x){//first card matches
					if(mycard_num1==(i%13 )|| (mycard_num1>= (i+1)%13 && mycard_num1 <= (i+4)%13)){    //check if 1 counts
						myGHs[9] = mycard_num1;
					}
				}else if(mycard_suit2 ==x){//second card matches
					if(mycard_num2==(i%13) || (mycard_num2>= (i+1)%13 && mycard_num2 <= (i+4)%13)){    //check if 1 counts
						myGHs[9] = mycard_num2;
					}
				}
			}
			if(y==5){//have a st flush
				myProbs[9] = 1;

			}else if(y==4 && cards_to_go > 0){//1 card to go.
				if(myProbs[9] < 1){ //if we already have it, don't count.  identify cards we need
					// stick in a 2 in for_st[card we need]
					if(!cards[i%13][x]){
						for_st[i%13] = 10;
					}else if (!cards[(i+1)%13][x]){
						for_st[(i+1)%13]=10;
					}else if (!cards[(i+2)%13][x]){
						for_st[(i+2)%13]=10;
					}else if (!cards[(i+3)%13][x]){
						for_st[(i+3)%13]=10;
					}else{
						for_st[(i+4)%13]=10;
					}
				}//done adding in, if we only needed 4 here.
			}else if(y==3 && cards_to_go == 2){//need both
				// stick in a 2 in for_st[card we need]
				if(!cards[i%13][x]&&for_st[i%13]==0){for_st[i%13] = 1;}
				if (!cards[(i+1)%13][x] && for_st[(i+1)%13] == 0){for_st[(i+1)%13]=1;}
				if (!cards[(i+2)%13][x]&& for_st[(i+2)%13] == 0){for_st[(i+2)%13]=1;}
				if (!cards[(i+3)%13][x]&& for_st[(i+3)%13] == 0){for_st[(i+3)%13]=1;}
				if (!cards[(i+4)%13][x]&& for_st[(i+4)%13] == 0){for_st[(i+4)%13]=1;}
			}
		}
		if(myProbs[9] != 1){ //now use all the cards we have and the cards we need. still straight flush
			high_cnt=0;
			for(j=0;j<13;j++){
				high_cnt+= for_st[j];
			}
			if(high_cnt == 0){myProbs[9]=0;}
			else if(cards_to_go == 1){
				if(high_cnt == 10){
					myProbs[9] = 1.0/46.0; //= ?
				}else if(high_cnt == 20){
					myProbs[9] = 2.0/46.0; // = ?
				}else{
					cout<<"error type 12478A"; //if 1 card to go, there should be at most 2 ways to make st_flushes
				}
			}else{//cards to go = 2. can be 2 cards to make 1, 1 card to make 1 and 2 cards to make 2, or more combinations or 34678
				if(high_cnt ==10){ //only edge cases, ex: A234. not sure if this can happen
					cout<<"borken12341";
				}
				else if(high_cnt == 2){
					myProbs[9] = 0.00092506938; //  2/47*1/46
				}else if(high_cnt == 3){ //ex 234
					myProbs[9] = 0.00185013876;   // 1/47*2/46 + 2/47*1/46
				}else if(high_cnt == 4){ // 2/47*2/46 + 2/47*1/46*2
					myProbs[9] = 0.00370027752;
				}
				else if(high_cnt == 10 || high_cnt == 11){ //need exactly one card still eg: A234
					myProbs[9]= 0.0425531915;// 2/47 =1 - 46/47 * 45/46
				}
				else if(high_cnt == 12 || high_cnt ==13){ //ex: A345 for 22, 34578 for 23 (getting 9 doesn't help)
					myProbs[9] = 0.0434782609;  //  = 1 - 44/47 * 43/46 - 44/47 * 2/46 - 2/47 * 44/46
				}
				else if(high_cnt == 20 || high_cnt == 21 || high_cnt == 22 || high_cnt == 23 ){ //i think 42 is only possible
					myProbs[9] = 0.0841813136;// = 1 - 45/47 * 44/46

				}
				else{cout<<endl<<"unanticipated st flush high cnt: "<<high_cnt<<" from "<<endl;
					output_card(zcards[0]);
					output_card(zcards[1]);}
			}//done with 2 card scenario
		}//done calc'n probabilitiy

		//end check for a st flush. now check for flush

		if(suits[x] >= 3){ //set high/low equal to highest of suit, all that matters
			if(mycard_suit1 == x && mycard_suit2 == x){
				if(mycard_num1 > mycard_num2){
					myGHs[6] = mycard_num1;
				}else { myGHs[6] = mycard_num2;}
			}else if(mycard_suit1 == x){
				myGHs[6] = mycard_num1;
			}else{myGHs[6] = mycard_num2;}
		}

		if(suits[x]==3){ //need next 2 to count
			myProbs[6] = 0.0416281221; // = 1-10/47 * 9/46
		}else if(suits[x]==4){
			if(cards_to_go == 2){myProbs[6] =  0.349676226;} // = 1 - 38/47 * 37/46
			else{myProbs[6] = 9.0/46.0;}
		}else if(suits[x]>=5){
			myProbs[6] = 1;
		}


	}else{  //x<0, no flush or straight flush
		myProbs[9] = 0;
		myProbs[6] = 0;
	}

	for(i=0;i<12;i++){
		for_st[i]=0;
	}

	//now look for st, rank 4. then we'll look for all the paired business.
	//START ST!!!  note we don't actually need to reset for_st, the same things count towards my straight.
	for(i=12;i!=9;i=(i+1)%13){ //10 possible straights, highs of 5-A.  two scenarios: need 2 cards or need 1 card
		y = (nums[i%13]>0) + (nums[(i+1)%13] > 0) + (nums[(i+2)%13] > 0) + (nums[(i+3)%13] > 0) + (nums[(i+4)%13] > 0);
		if(y+cards_to_go >= 5){
			if( mycard_num2 == i && i==12 ){ //have an ace in a low starting straight. num2 is higher so would be ace
				if(mycard_num1 <= 3){ //12 0 1 2 3
					myGHs[5] = mycard_num1;
				}else{
					myGHs[5] = 0;
				}
			}else if(mycard_num2 >= (i%13) && mycard_num2 <= (i+4)%13){ //if 2 counts, use it!
				myGHs[5] = mycard_num2;
			}else if(mycard_num1 >= (i%13) && mycard_num1 <= (i+4)%13){
				myGHs[5] = mycard_num1;
			}else if(i==12){ //going for low st, don't have a helper myself
				myGHs[5] = 0;
			}else{
				myGHs[5] = i;
			}
		} //end section identifying what we have for the st
		if(y==5){//have a st
			myProbs[5] = 1;
		}else if(y==4 && cards_to_go > 0){//1 card to go.
			if(myProbs[5] < 1){ //if we already have it, don't count.  identify cards we need
				// stick in a 2 in for_st[card we need]
				if(nums[i]==0){
					for_st[i%13] = 10;
				}else if (nums[(i+1)%13]==0){
					for_st[(i+1)%13]=10;
				}else if (nums[(i+2)%13]==0){
					for_st[(i+2)%13]=10;
				}else if (nums[(i+3)%13]==0){
					for_st[(i+3)%13]=10;
				}else{
					for_st[(i+4)%13]=10;
				}
			}//done adding in, if we only needed 4 here.
		}else if(y==3 && cards_to_go == 2){//need both
			// stick in a 2 in for_st[card we need]
			if(nums[i]==0&&for_st[i%13]==0){for_st[i%13] = 1;}
			if (nums[(i+1)%13]==0 && for_st[(i+1)%13] == 0){for_st[(i+1)%13]=1;}
			if (nums[(i+2)%13]==0 && for_st[(i+2)%13] == 0 ){for_st[(i+2)%13]=1;}
			if (nums[(i+3)%13]==0 && for_st[(i+3)%13] == 0){for_st[(i+3)%13]=1;}
			if (nums[(i+4)%13]==0 && for_st[(i+4)%13] == 0){for_st[(i+4)%13]=1;}
		}
	}
	if(myProbs[5] != 1){ //now use all the cards we have and the cards we need
		high_cnt=0;
		for(j=0;j<13;j++){
			high_cnt+= for_st[j];
		}
		if(high_cnt == 0){myProbs[5]=0;}
		else if(cards_to_go == 1){
			if(high_cnt == 10){
				myProbs[5] = 0.0869565217; //= 4.0/46.0?
			}else if(high_cnt == 20){
				myProbs[5] = 0.173913043; // = 8/46?
			}else{
				cout<<"error type 1243378A"; //if 1 card to go, there should be at most 2 ways to make st_flushes
			}
		}else{//cards to go = 2. can be 2 cards to make 1, 1 card to make 1 and 2 cards to make 2, or more combinations or 34678
			if(high_cnt ==10){ //only edge cases, ex: A234. not sure if this can happen
				cout<<"errorbzbs";
			}else if(high_cnt == 2){ //A23
				myProbs[5] =  0.0148011101; //  = 8/47*4/46
			}else if(high_cnt == 3){ // 234
				myProbs[5] = 0.0296022202;		// 4/47 * 8/46 + 8/47* 4/46
			}else if(high_cnt == 4){ //345
				myProbs[5] = 0.0444033302;  // 8/47*8/46 + 8/47*4/46
			}
			else if(high_cnt == 10 || high_cnt == 11 || high_cnt==12){ //need exactly one card still eg: A234
				myProbs[5]= 0.16466235;// 2/47 =1 - 43/47 * 42/46
			}
			else if(high_cnt == 12 || high_cnt == 13 || high_cnt == 14){  //ex: A345 for 22, 34578 for 23 (not getting 9 doesn't help)
				myProbs[5]= 0.190564292;// = 1 - 35/47 * 34/46 - 35/47 * 8/46 - 8/47 * 35/46
			}
			else if(high_cnt == 21 || high_cnt == 22 || high_cnt ==23){ //ex: 5 789 J
				myProbs[5] = 0.314523589;  // 1-39/47*38/46 
			}					//			no cards		none then 1		1 then none
			else if(high_cnt == 40 || high_cnt == 41 || high_cnt == 42 || high_cnt == 43 || high_cnt ==44){ //i think 42 is only possible
				myProbs[5] = 0.296484736;// = 1 - 39/47 * 39/46

			}
			else{cout<<endl<<"unanticipated st high cnt: "<<high_cnt<<" from "<<endl;
					output_card(zcards[0]);
					output_card(zcards[1]);
			}
		}//done with 2 card scenario
	}//done calc'n probability
	//END ST

	//high card
	myProbs[0]=1;  
	myGHs[0]=mycard_num2;

	//2 of a kind.  array ele 1
	for(i=0;i<13;i++){
		if(nums[i]>=2){
			if(i>=mycard_num2){
				y = 2; //this is as good a pair as i can get
			}
			else{
				y=1;  //i can get a better pair
			}
			myProbs[y]=1;
			if(mycard_num1 == i || mycard_num2 ==i){
				myGHs[y]= i*100+99;
			}else{
				myGHs[y] = i*100 + mycard_num2; //num2 will be my kicker
			}
		}
	}
	if(myProbs[2]==0 && cards_to_go>0){ //if i have a higher pair, dont care about the lower
		myGHs[2] = mycard_num2;
		if(cards_to_go == 1){
			myProbs[2] = 0.0652173913;  // = 3/46. just for the high card. TOFIX
		}else{myProbs[2] = 0.124884366;}// = 1-44/47*43/46
		if(myProbs[1]==0){ //my upper isn't beat
			myGHs[1] = mycard_num1;
			if(cards_to_go == 1){
				myProbs[1] = 0.0652173913;  // = 3/46. just for the high card. TOFIX
			}else{myProbs[1] = 0.124884366;}// = 1-44/47*43/46
		}
	}//done with a pair!


	//done with a pair

	//now 2P. x=low pair, y=high pair
	x=y=-1;
	for(i=0;i<13;i++){
		if(nums[i]>=2){
			x=y;
			y=i;
		}
	}
	if(x>-1){  //we have 2P
		myProbs[3]=1;
		if(mycard_num1 == y || mycard_num2==y){
			myGHs[3]= y*10000+x*100+99;
		}else if(mycard_num1==x || mycard_num1==x){
			myGHs[3]= y*10000+x*100+50;
		}
		else{  //neither mine
			if(mycard_num1>mycard_num2){
				myGHs[3]= y*10000+x*100+mycard_num1;}
			else{myGHs[3] = y*10000+x*100+mycard_num2;}
		}
	}else if(y>-1 && cards_to_go > 0){  //we have 1P.  we'll only track possibility of matching upper, (or lower) even though there're tons of ways to hit it.
		if(y!= mycard_num2 && y!=mycard_num1){ //pair isn't with upper, so we'll tie it to.
			if(cards_to_go==2){	myProbs[3] = 0.241443108;} // = 1-41/47*40/46
			else{	myProbs[3] = 0.130434783; }
		}else{
			if(cards_to_go==2){	myProbs[3] = 0.124884366;} // = 1-44/47*43/46
			else{	myProbs[3] = 0.0652173913; } // = 3/46
		}
		if(y>mycard_num2){
			myGHs[3] = y*100+mycard_num2;
		}else{
			myGHs[3] = mycard_num2*100+y;
		}
	}else if(cards_to_go ==2){ //don't even have a pair. probability of hitting 2p...
		myGHs[3] = mycard_num2*100+mycard_num1;
		myProbs[3] = 0.00832562442; // = 6/47*3/46
	}

	for(i=0;i<13;i++){
		if(nums[i] >= 3){
			myProbs[4]=1;
			if(mycard_num1 == i || mycard_num2 ==i){myGHs[3]=i*100 + 99;}
			else{myGHs[4]=i*100+mycard_num2; }
		}
	}

	if(myProbs[4] ==0 && cards_to_go > 0){ //searching for trips, don't have them
		if(y>-1){//have 1p
			if(cards_to_go==2){
				myProbs[4] = 0.0841813136; // = 1 - 45/47*44/46
			}else{ //cards to go is 1
				myProbs[4] = 0.043478260; // = 2/46
			}
			if(mycard_num1 == y || mycard_num2 ==y){myGHs[4]=y*100 + 99;}//have the two p
			else{myGHs[4]=y*100+mycard_num2; }
		}else if(cards_to_go==2){ //don't have even a pair, and thinking about trips	
			myProbs[4] = 0.00555041628; // = 3/47*2/46
			myGHs[4] = mycard_num1;	
		}
	}//end search for trips

	for(i=0;i<13;i++){
		if(nums[i]==4){
			myProbs[8]=1;
			if(mycard_num2==i || mycard_num1 ==1){myGHs[8]=i*100+99;}
			else{myGHs[8]=i*100+mycard_num2; }
		}
	}


	if( myProbs[8] !=1 && ((myProbs[4]==1 && cards_to_go > 0) || (y>-1 && cards_to_go ==2))){
		if(myProbs[4]==1){
			myGHs[8] = myGHs[4];
			if(cards_to_go ==1){
				myProbs[8]= 0.0217391304;// = 1/46
			}else{
				myProbs[8]= 0.0425531915;// = 1-46/47*45/46 = 2/47			
			}
		}else{   // don't have trips, have 2p or less
			if(myProbs[3] == 1){ //have 2p
				myProbs[8] = 0.104995375;// = 1-43/47*45/46
				if(y == mycard_num2 || x == mycard_num2){ //y and x are pairs
					myGHs[8] = mycard_num2*100+99;
				}else if(y == mycard_num1 || x == mycard_num1){ //y and x are pairs
					myGHs[8] = mycard_num1*100+99;
				}else{
					myGHs[8] = myGHs[2];
				}
			}else{ //we have 1p
				myProbs[8] = 0.00092506938;// = 2/47*1/46
				if(myProbs[2]==1){
					myGHs[8]=myGHs[2];
				}else{
					myGHs[8]=myGHs[1];
				}
			}
		}
	}//end search for 4-kind

	//begin fullhouse
	if(myProbs[4]==1 && myProbs[3]==1){
		myProbs[7]=1;
		x=(myGHs[4]%100)/100;//this is number of trips
		for(i=0;i<13;i++){
			if(nums[i]>=2 && i!=x){
				myGHs[4]=x*100+i;
			}
		}
	}

	if(myProbs[7]!=1 && cards_to_go >0){ //search for fullhouse.  we're ignoring going 3P->FH
		if(myProbs[3]==1){ //have 2p
			myGHs[7]=myGHs[3];
			if(cards_to_go == 2){
				myProbs[7]= 0.16466235;// 1-43/47*42/46. could add 3/47*2/46
			}else{
				myProbs[7]=0.0869565217;
			}
		}else if(myProbs[4]==1){ //have trips
			x=(myGHs[4]%100)/100;
			if(mycard_num1==mycard_num2){ //trips with hand. can't have just a P
				myGHs[7]=x*100+99;
			}
			else if(x==mycard_num2 ){
				myGHs[7]=x*100+mycard_num1;
			}else if(x==mycard_num1){
				myGHs[7]=x*100+mycard_num2;
			}else{//trips are on the table
				myGHs[7]=x*100+mycard_num2;
			}
			if(cards_to_go == 2){
				myProbs[7]= 0.298334875;// = 1-41/47*40/46 + 41/47*3/46
			}else{
				myProbs[7] = 0.195652174;// = 3 cards can match. 9/46
			}

		}else if(cards_to_go==2 && (myProbs[1]==1 || myProbs[2]==1)){//have 1P, 2 cards to go
			if(myProbs[2]==1){myGHs[7]=myGHs[2];}
			else{myGHs[7]=myGHs[1];}
			myProbs[7] = 0.0249768733;// = 9/47*2/46+2/47*9/46*2
		}
	}
}

/* just some weighting function. returns hand quality i guess */
float weight1(double* myProbs, int* myGHs, int round){
	float myVal = 0;

	

	return myVal;
}






