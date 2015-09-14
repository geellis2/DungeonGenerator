/**
 * @file beastiary.h
 * @author Graham Ellis
 * @date Spring 2015
 */
#ifndef _BEASTIARY_H_
#define _BEASTIARY_H_

#include <fstream>
#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

class Beastiary {
    public:

	//object that stores a single beast
	//should have made a seperate file for this. ugh.
	class beast {
		public:
		beast(string & name, double cr, string alignment, vector<string> types){
		this->name = name; this->cr = cr; this->alignment = alignment; this->types = types;};
		string name;
		double cr;
		string alignment;
		vector<string> types;		
	};
	class trap {
		public:
		trap(double cr, bool attack){
			this->cr = cr; disable = 20; perception = 20; bonusOrSave = 10;
			if(!attack)
				bonusOrSave+=10;
		};
		string description;
		double cr;
		int disable;
		int perception;
		int bonusOrSave;
	};
	trap itsATrap(double cr);
        Beastiary(const std::string & filename);
	beast & getRandomBeast(double cr);
	beast & getRandomBeast(double lo, double hi);
	vector<string> getEncounter(double cr, beast & finalB);
	vector<string> getEncounter(double lo, double hi, beast & finalB);
	bool similar(beast & b1, beast & b2);
	void readMinionTypes();
	void buildDungeon(double apl); 	
	void getPoisons();
    private:
	//why did I want a vector of a vector of beasts? sort by CR?
	//P.S. that was the reason.
	vector<vector<beast>> beasts;
	map<string, vector<string>> minionTypes;
	vector<vector<string>> poisons;
        std::ifstream infile; /**< std::ifstream used for reading the file */
};
#endif
