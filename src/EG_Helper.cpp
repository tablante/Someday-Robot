/*
 * EG_Helper.cpp
 *
 *  Created on: May 10, 2011
 *      Author: btablante
 */

#include "EG_Helper.h"

EG_Helper::EG_Helper(){


}
/* called by an outsider */
void EG_Helper::initialize(const char* spec, const char* payoffs, const char* features){
		started=true;
		spec_f.open(spec, ios::in);
		if(OVERWRITE){payoffs_f.open(payoffs, ios::out);}
		else {payoffs_f.open(payoffs, ios::app);}
		features_folder.assign(features);//features_f.open(features, ios::app);
		strat_iter = 0;
	}

/* called by an outsider */
	void EG_Helper::finish(){
		if(started==true){
			started=false;
			payoffs_f.close();
		}else{

			cout<<"error, trying to finish again";
		}
	}


	void EG_Helper::read_spec(){
		char inp[200];
		spec_f>>inp;
		if(strcmp(inp,"---")==0){
			spec_f>>inp;
			while(strcmp(inp,"---")!=0 && strlen(inp)>0){
				types.push_back(string(inp+1)); //cutoff first char
				spec_f>>inp;
			}
			spec_f>>inp;

			//require last char to not be empty.
			while(strcmp(inp,"---")!=0 && strlen(inp)>0){
							inp[strlen(inp)-1]=0;//remove last char- :
							param_types.push_back(inp);
							spec_f>>inp;
							param_vals.push_back(inp);
							spec_f>>inp;
				}
		}else{
			cout<<"error, didn't read first thing";
		}
		setup_type_profile();
	}

	bool EG_Helper::more_strats(){

		return strat_iter < int(types.size());
	}

	bool EG_Helper::have_param(string key){
		int i;
		bool found=false;
		for(i=0;i<int(param_types.size())&&found==false; i++){
			if(param_types[i].compare(key)==0)found=true;
		}
		return found;
	}

	string EG_Helper::param_value(string key){
		int i;
		bool found=false;
		for(i=0;i<int(param_types.size())&&found==false; i++){
			if(param_types[i].compare(key)==0){
				found=true;
			i--;
			}
		}
		return param_vals[i];
	}

	//convert types to profile. unique list, etc.
	void EG_Helper::setup_type_profile(){
		int i,j;
		bool found;
		for(i=0;i<int(types.size());i++){
			//check if it's already contained:
			found=false;
			for(j=0;j<int(type_profile.size());j++){
				if(types[i].compare(type_profile[j])==0){
					found=true;
					type_occurrences[j]++;
					j=type_profile.size();
				}
			}
				if(found==false){
					type_profile.push_back(types[i]);
					type_occurrences.push_back(1);
				}
			}

		j=0;
		//now we've populated type_profile
		for(i=0;i<int(type_profile.size());i++){
			j+=type_occurrences[i];
		}
		for(i=0;i<int(type_profile.size());i++){
			type_probabilities.push_back(float(type_occurrences[i])/float(j));
			type_occurrences[i]=0;
			total_payoffs.push_back(0.0);
		}


	}

	void EG_Helper::add_payoff(string type, float payoff){
		int i;
		bool found=false;
		for (i=0;i<int(type_profile.size()) && found==false;i++){
			if(type_profile[i].compare(type)==0){
				found=true;
				i--;
			}
		}
		if(found==false){
			cout<<"error, trying to add an invalid payoff";
		}else{
			type_occurrences[i]++;
			total_payoffs[i]+=payoff;
		}
		//cout<<"type: "<<type<<" "<<i<<" of "<<type_profile.size()<<": "<<type_occurrences[i]<<" "<<total_payoffs[i]<<endl;
	}

	void EG_Helper::add_feature(string type, string id, float val){
		int i;
		bool found=false;
		for(i=0;!found && i<int(feature_names.size());i++){
			if(feature_names[i].compare(id)==0 && feature_types[i].compare(type) == 0){
				found=true;
				i--;
			}
		}
		if(found){
			feature_vals[i]+=val;
		}else{
			feature_names.push_back(id);
			feature_vals.push_back(val);
			feature_types.push_back(type);
		}

	}

	void EG_Helper::add_spec_to_features(){
		int i;
		for(i=0;i<int(param_types.size());i++){
			add_feature(string("params"), param_types[i],atof(param_vals[i].c_str()));
		}
		for(i=0;i<int(type_profile.size());i++){
			add_feature(string("params"), type_profile[i],type_probabilities[i]);
		}

	}

	//write to the ofile we saved
	void EG_Helper::write_payoffs(){
		payoffs_f<<"---"<<endl;
		int i;
		for(i=0;i<int(type_profile.size());i++){
			payoffs_f<<type_profile[i]<<": ";
			if(type_occurrences[i]>0)payoffs_f<<(total_payoffs[i] / type_occurrences[i]);
			else{payoffs_f << -1;}
			payoffs_f<<endl;
			//cout<<type_profile[i]<<" "<<type_occurrences[i]<<" "<<total_payoffs[i]<<endl;
		}
	}
	//write to the ofile the features
	void EG_Helper::write_features(){
		int i,j;

		vector<string> completedFeatures;
		vector<ofstream*> feature_files;
		ofstream* file;
		string f_name; //= features_folder + feature_types[i]
		bool found;
		for(i=0;i<int(feature_names.size());i++){
			found=false;
			for(j=0;j<int(completedFeatures.size())&&found==false;j++){
				if(completedFeatures[j].compare(feature_types[i])==0){
					found=true;
					j--;
				}
			} //end search

			if(found){
					file = feature_files[j];
					*file<<feature_names[i]<<": "<<feature_vals[i]<<endl;
				}
				else{


				f_name = features_folder + feature_types[i];
				if(OVERWRITE){file = new ofstream(f_name.c_str(), fstream::out);}
				else{ file = new ofstream(f_name.c_str(), fstream::app);}
				*file<<"---"<<endl;
				*file<<feature_names[i]<<": "<<feature_vals[i]<<endl;
				feature_files.push_back(file);
				completedFeatures.push_back(feature_types[i]);

				}
			}



		for(i=0;i<int(feature_files.size());i++){
			feature_files[i]->close();
			delete(feature_files[i]);
		}



	}

	//for debugging
	void EG_Helper::test_out(){
		int i,len;
		len = types.size();
		cout<<"types:"<<endl;
		for(i=0;i<len;i++){
			cout<<"type "<<i<<": ."<<types[i]<<"."<<endl;
		}

		cout<<endl;

		len = param_types.size();
		cout<<"params:"<<endl;
		for(i=0;i<len;i++){
			cout<<"param "<<i<<": ."<<param_types[i]<<". = ."<<param_vals[i]<<
					"."<<endl;
		}

		cout<<"Type statistics:"<<endl;
		for(i=0;i<type_probabilities.size();i++){
			cout<<type_profile[i]<<": "<<type_probabilities[i]<<endl;

		}
		cout<<"done with testout"<<endl;

	}

	string EG_Helper::next_strategy(bool in_order){
		string s;
		if(type_profile.size() == 0 ){
			cout<<"error, getting something when type profile not set up!";
			setup_type_profile();
		}
		else if(in_order){
			s=types[strat_iter];
			strat_iter++;
		}else{
			float f = float((rand()%1000) / 1000.0);
			float count = type_probabilities[0];
			int i=0;
			while(f>count){
				i++;
				count+=type_probabilities[i];
			}
			s = type_profile[i];
		}
		return s;
	}
