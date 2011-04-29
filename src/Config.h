/*
 * Config.h
 *
 *  Created on: Dec 22, 2010
 *      Author: btablante
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

const int MAX_COMMUNITY_CARDS = 5;
const int MAX_SEATS = 9;

const int MAX_NAMELENGTH=8;  //min 8
const int START_OUTPUT = 2; //basically, 4 spaces in output the number, then space the rest out
								//has to be at least 2 (start action and raise)
const int MAX_RAISES = 3; //max number of raises for limit poker
const int TH_ROUNDS =  4; //pre-flop ----- flop ------ turn -------- river

const int DEF_BIG_BLIND = 2;
const int DEF_SML_BLIND = 1;
const int DEF_ANTE = 0;

const bool USE_BLINDS = true; //if false, using antes.
const bool FIXED_LIMIT = true; //implemented on player side

int length(int n);
void output_card(int c);
void output_card_small(int c);
int input_num();
void output_my_future(double* probs, int* ghs); //*these are 
void output_my_future2(double* probs, int* ghs, double* p_tie_win, double* p_lose); //*these are 


#endif /* CONFIG_H_ */
