/*
 * Evaluators
 *
 *  Created on: Dec 22, 2010
 *      Author: btablante
 */

#ifndef EVALUATORS_H_
#define EVALUATORS_H_
#include <iostream>
#include <string>
#include "Config.h"
using namespace std;

//EVALUATORS: with just a number returns what you have.
double eval_7(int cards[7]);  //FLAT, just returns a number.  
		//can make some numbers negative to reflect unshown

//The FOLLOWING are NAIVE operators, and judge based on all other pockets (excl mine) being available
//
void eval_th_m(int* cards, int numCards, double* myProbs, int* myGHs); //evaluate flop + my 2, judge strength- probabilities of having each hand
/*float* eval_5o(int cards[5]); //evaluate evaluate flop for others, given what i have.

float* eval6m(int cards[6]); //evaluate flop+turn + my 2, judge strength
float* eval_6o(int cards[6]); //evaluate evaluate flop+turn for others, given what i have.

float* eval_7o(int cards[7]); //evaluate evaluate flop+turn for others, given what i have.
*/

float weight1(double* myProbs, int* myGHs, int round);

#endif /* EVALUATORS_H_ */
