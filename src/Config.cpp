/*
 * Config.cpp
 *
 *  Created on: Dec 31, 2010
 *      Author: btablante
 */

#include "Config.h"
#include <iostream>



int length(int n){  //only works positive, less than 10k
	int c = 0;
	int negz=0;
	if(n<0){
		negz=1;
		n*=-1;
	}
	if(n<10) c=1;
	else if(n<100) c=2;
	else if(n<1000) c=3;
	else if(n<10000)c=4;
	else if(n<100000)c=5;
	else if(n<1000000)c=6;
	else if(n<10000000)c=7;
	else if(n<100000000)c=8;
	return c+negz;
}


void output_card(int c){
	int suit = c%4;
	int number = c/4;
	switch (number){
		case 12:
			cout<<"A";
			break;
		case 11:
			cout<<"K";
			break;
		case 10:
			cout<<"Q";
			break;
		case 9:
			cout<<"J";
			break;
		case 8:
			cout<<"T";
			break;
		default:
			cout<< number+2;
			break;}
	switch(suit){
		case 0:
			cout<<"S";
			break;
		case 1:
			cout<<"C";
			break;
		case 2:
			cout<<"D";
			break;
		case 3:
			cout<<"H";
			break;}
}

float uRand(float lower, float upper){
	int factor = 1000;
	int modulator;
	modulator = int(factor*(upper-lower));
	float random = float((rand()%modulator)/float(factor));
	return random+lower;

}

void output_card_small(int c){
	int suit = c%4;
	int number = c/4;
	switch (number){
		case 12:
			cout<<"a";
			break;
		case 11:
			cout<<"k";
			break;
		case 10:
			cout<<"q";
			break;
		case 9:
			cout<<"j";
			break;
		case 8:
			cout<<"t";
			break;
		default:
			cout<< number+2;
			break;}
	switch(suit){
		case 0:
			cout<<"s";
			break;
		case 1:
			cout<<"c";
			break;
		case 2:
			cout<<"d";
			break;
		case 3:
			cout<<"h";
			break;}
}

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


/* 0-highcard, 1-pair strictly below high pocket, 
	2-pair above high pocket, 3-twopair and so on */
void output_my_future(double* probs, int* ghs){
int i,j;
cout<<"Name      Qualty  Probabil";
for(i=0;i<10;i++){
	cout<<endl;
	switch (i){
		case 0:
			cout<<"HighCard";
			break;
		case 1:
			cout<<"LowPair ";
			break;
		case 2:
			cout<<"HighPair";
			break;
		case 3:
			cout<<"TwoPair ";
			break;
		case 4:
			cout<<"Trips   ";
			break;
		case 5:
			cout<<"Straight";
			break;
		case 6:
			cout<<"Flush   ";
			break;
		case 7:
			cout<<"TheBoat ";
			break;
		case 8:
			cout<<"FourKind";
			break;
		case 9:
			cout<<"StrFlush";
			break;

	}
	cout<<"  "<<ghs[i];
	for(j=0;j<8-length(ghs[i]);j++){cout<<" ";}
		cout<<probs[i];
}
}

void output_my_future2(double* probs, int* ghs, double* p_tie_win, double* p_lose){
int i,j;
cout<<"Name      Qualty  Probabil  Tie/Win   Lose";
for(i=0;i<10;i++){
	cout<<endl;
	switch (i){
		case 0:
			cout<<"HighCard";
			break;
		case 1:
			cout<<"LowPair ";
			break;
		case 2:
			cout<<"HighPair";
			break;
		case 3:
			cout<<"TwoPair ";
			break;
		case 4:
			cout<<"Trips   ";
			break;
		case 5:
			cout<<"Straight";
			break;
		case 6:
			cout<<"Flush   ";
			break;
		case 7:
			cout<<"TheBoat ";
			break;
		case 8:
			cout<<"FourKind";
			break;
		case 9:
			cout<<"StrFlush";
			break;

	}
	cout.precision(5);
	cout<<"  "<<ghs[i];
	for(j=0;j<8-length(ghs[i]);j++){cout<<" ";}
	cout<<fixed<<probs[i]+0.0;

	cout<<"   ";
	cout<<p_tie_win[i];

	cout<<"  "<<p_lose[i]+0.0;

}
}