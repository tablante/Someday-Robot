/*
 * EG_Helper.h
 *
 *  Created on: May 10, 2011
 *      Author: btablante
 */

#ifndef EG_HELPER_H_
#define EG_HELPER_H_

#include "Config.h"
using namespace std;

class EG_Helper{

public:
	EG_Helper();

	void initialize(const char* spec,const char* payoffs,const char* features);

	void read_spec();
	void write_payoffs();
	void write_features();
	void finish();
	void test_out();  //print some debugging information
	
	void add_payoff(string type, float payoff); //adds to the collection
	void add_feature(string type, string id, float val); //mostly a count of occurrences

	//so we can save each set of features for easy comparison
	void add_spec_to_features();

	bool have_param(string type);
	string param_value(string val);

	bool more_strats();

	//called by anyone to get the next strategy
	string next_strategy(bool in_order=false);


protected:

	ifstream spec_f;
	ofstream payoffs_f;
	string features_folder;

	//set to true if we're going through strategies in order
	int strat_iter;

	vector<string> types;  //agent types, as it's read in
	vector<string> param_types;
	vector<string> param_vals;
	bool started;  //if it's ready to use

	void setup_type_profile();

	//to be implemented: all below

	/* calculated  ordered lists: payoffs */
	vector<string> type_profile;  //ordered list of unique profiles (strings)
	vector<float> type_probabilities; //
	vector<unsigned int> type_occurrences;
	vector<float> total_payoffs; //sum of total payoffs

	/* features */
	vector<string> feature_types;  //each of these creates a file with this name
	vector<string> feature_names;  //entries within a file
	vector<float> feature_vals; //additively cumulative

};

#endif /* EG_HELPER_H_ */
