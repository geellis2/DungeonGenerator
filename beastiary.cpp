/**
 * @file beastiary.cpp
 * Implementation of the beastiary class.
 */

#include <iostream>
#include "beastiary.h"
using std::string;
using std::ifstream;
using std::to_string;


Beastiary::Beastiary(const string & filename){
	getPoisons();	
	infile.open (filename);
	if ( ! infile ) {
                   std::cout << "Error: Can't open the file named " << filename << std::endl;
                   exit(1);
                }

	for(int i = 0; i < 21; i++)
		beasts.push_back(vector<beast>());
	/**read a name
	*then read a CR
	*then read an XP
	*then read an alignment
	*then read a bunch of types 
	**/
	string line = "";
	getline(infile, line);
	while ( ! infile.eof() ){
		getline(infile, line);

		int pos = line.find("\t");
		//read a name
		string name = line.substr(0, pos);
		if (name == "")
			break;
		line = line.substr(pos + 1, line.size() - pos);

		pos = line.find("\t");\
		//read a cr
		double cr = std::stod(line.substr(0, pos));
		line = line.substr(pos + 1, line.size() - pos);
		pos = line.find("\t");
		//read an XP(actually don't)
		//line = line.substr(pos + 1, line.size() - pos);
		//pos = line.find("\t");
		//read an alignment
		string alignment = line.substr(0, pos);
		line = line.substr(pos + 1, line.size() - pos);
		pos = line.find("\t");
		//read a size
		string size =line.substr(0, pos);
		line = line.substr(pos + 1, line.size() - pos);
		//get land, air and sea speeds
		pos = line.find("\t");
		int land = stoi(line.substr(0, pos));
		line = line.substr(pos + 1, line.size() - pos);
		
		pos = line.find("\t");
		int fly = stoi(line.substr(0, pos));
		line = line.substr(pos + 1, line.size() - pos);

		pos = line.find("\t");
		int swim = stoi(line.substr(0, pos));
		line = line.substr(pos + 1, line.size() - pos);

		bool aquatic = (swim > 0 && land < 15 && fly < 15);
		//get what book the monster came from
		pos = line.find("\t");
		string book = line.substr(0, pos);
		line = line.substr(pos + 1, line.size() - pos);
		//loop to create vector of types
		vector<string> types;
		bool typesLeft = true;
		bool humanoid = false;
		bool badBook = book == "Tome of Horrors 4" || book == "Inner Sea Bestiary";
		string curr;
		while(typesLeft)
		{
			pos = line.find("\t");

			if (pos == 0 || pos == int(line.size()) - 1 || pos == int(string::npos))
				typesLeft = false;
			curr = line.substr(0, pos);	
			if(curr == "")
				break;
			line = line.substr(pos + 1, line.size() - pos);
			types.push_back(curr);
			if(curr == "humanoid")
				humanoid = true;
			if(curr == "aquatic")
				aquatic = true;
		}
		if(cr > 20.0)
			cr = 21.0;
		if(humanoid)
			humanoid = alignment.find("E") == string::npos;
		if(alignment.find("G") == string::npos && !humanoid && !aquatic && !badBook)
			beasts[int(cr)].push_back( beast(name, cr, alignment, types));
	}
}

Beastiary::beast & Beastiary::getRandomBeast(double cr){
	//get random dude from beasts[cr]
	return beasts[int(cr)].at(rand() % beasts[int(cr)].size());
}

Beastiary::beast & Beastiary::getRandomBeast(double lo, double hi){
	//randomly choose number between lo and hi
	//call grb(that number^)
	if (lo < 0)
		lo = 0;
	return getRandomBeast((rand() % int(hi-lo))+lo);
}

vector<string> Beastiary::getEncounter(double cr, beast & finalB){
	//delete this, just avoiding warnings
	vector<string> creatures;	
	beast * miniB;
	bool boss = rand() % 10 < 3; 
	if(boss || cr <= 4 || cr < finalB.cr - 4){
		if(rand() % 10 == 6)
		{
			creatures.push_back(itsATrap(cr).description);
			return creatures;
		}
		do{
			miniB = &getRandomBeast(cr);
		}while(!similar(*miniB, finalB));
		creatures.push_back(miniB->name);
		return creatures;
	}
	boss = rand() % 10 < 8;
	/*randomly choose miniboss or mob fight.
	*
	*for mob, randomly choose a cr from like cr-5 to cr-2.
	*add monsters of that level to vector.
	*
	*for miniboss randomly choose a creature of cr or cr-1
	* add cr -3 to cr-4 until at cr.
	**/
	if (boss)
	{
		if(rand() % 10 == 6)
		{
			creatures.push_back(itsATrap(cr).description);
			return creatures;
		}
		do{
			miniB = &getRandomBeast(cr - 2);
		}while(!similar(*miniB, finalB));
		creatures = getEncounter(cr - 2, finalB);
		creatures.push_back(miniB->name);	
	}
	else
	{
		if(rand() % 10 == 6)
		{
			creatures.push_back(itsATrap(cr - 2).description);
			creatures.push_back(itsATrap(cr - 2).description);			
			creatures.push_back(itsATrap(cr - 2).description);
			return creatures;
		}
		creatures = getEncounter(cr - 2, finalB);
		vector<string> temp = creatures;
		creatures.insert(creatures.end(), temp.begin(), temp.end());
		creatures.insert(creatures.end(), temp.begin(), temp.end());
	}
	return creatures;
}

vector<string> Beastiary::getEncounter(double lo, double hi, beast & finalB){
	//delete this, just avoiding warnings	
	if (lo < 0)
		lo = 0;
	return getEncounter((rand() % int(hi-lo))+lo, finalB);
}

bool Beastiary::similar(beast & b1, beast & b2){
	//doesn't work for things that are different but work well together...
	if (b2.alignment.find("E") != string::npos || b1.alignment == b2.alignment)
		return true;
	for(size_t i = 0; i < b1.types.size(); i++)
		for(size_t j = 0; j <  b2.types.size(); j++)
			if(b1.types[i] == "swarm" || b1.types[i] == b2.types[j])
				return true;
	return false;
}

void Beastiary::readMinionTypes()
{
	//idk... things...
	//maybe I can implement this later. rn it's too much...
}
Beastiary::trap Beastiary::itsATrap(double cr)
{
	bool attack = rand() % 10 < 4;
	trap mytrap(cr, attack);
	int items = 0;
	if (cr >0){
		items++; cr--;
	}
	if (cr >0){
		items++; cr--;
	}
	if (cr >0){
		mytrap.bonusOrSave+=5; cr--;
	}
	if (cr >0){
		mytrap.perception+=5;cr--;
	}
	if (cr >0){
		mytrap.disable+=5;cr--;
	}
	if (cr >0){
		items++; cr--;
	}
	if (cr >0){
		items++; cr--;
	}
	if (cr >0){
		mytrap.bonusOrSave+=5; cr--;
	}
	if (cr >0){
		items++; cr--;
	}
	if (cr >0){
		mytrap.perception+=5;cr--;
	}
	if (cr >0){
		items++; cr--;
	}
	if (cr >0){
		items++; cr--;
	}
	if (cr >0){
		mytrap.disable+=5;cr--;
	}
	while(cr>0)
	{
		items++; cr--;
	}
	mytrap.description = "A trap which ";
	if (rand() % 20 < 3)
	{
		int place = items/2 + 1;
		if(place > 8)
			place = 8;
		string poison = poisons[place].at(rand() % poisons[place].size());
		mytrap.description += "uses " + poison + " and ";
		items -= items/2;
		//poison!
	}
		
	bool multiTarget = !attack || (rand() % items) > 1;
	if (multiTarget)
		items--;
	int rounds = 1;
	for(int i =0; (i < 3 && i <items && attack); i++ )
		if(rand() % (10) <1)
			rounds++;	
	int avgDamage = 10*items/rounds;
	//make all of the stats less perfect.
	int diff = abs(5 - rand() % 6 -rand() % 6);
	mytrap.disable -= diff;
	diff = abs(5 - rand() % 6 -rand() % 6);
	mytrap.perception -= diff;
	diff = abs(5 - rand() % 6 -rand() % 6);
	mytrap.bonusOrSave -= diff;
	diff = abs(5 - rand() % 6 -rand() % 6);
	avgDamage -= diff;
	
	if(multiTarget)
		mytrap.description += "hits everybody within a " +
to_string(5*(rand() % 3 + 2)) + " foot line/square, dealing";	
	else
		mytrap.description += "deals";
	mytrap.description += " an average of " + to_string(avgDamage) ;
	mytrap.description += " damage, unless it is seen with a perception check greater than " ;
	mytrap.description += to_string(mytrap.perception) + " or the character";
	if(!attack)
		mytrap.description += " dodges it by making a save of " ;
	else
		mytrap.description += "'s AC is better than an attack with bonus of ";
	mytrap.description += to_string(mytrap.bonusOrSave) + ".";
	mytrap.description += " It can be disabled with a disable device check of ";
	mytrap.description += to_string(mytrap.disable) + ".";
	return mytrap;
	//d4		 vs 	d6 	vs 	d8 	
	//pit spikes and falling damage
	//arrows and swords and axes
	//also -1 item for multi target.
	//damage = damage/number of rounds.
}
void Beastiary::getPoisons()
{
	for(int i = 0; i < 9; i++)
		poisons.push_back(vector<string>());
	infile.open("poisons.txt");
	while ( ! infile.eof() ){
		string line = "";
		getline(infile, line);
		int pos = line.find("\t");
		string name = line.substr(0, pos);
		if (name == "")
			break;
		line = line.substr(pos + 1, line.size() - pos);
		pos = line.size();
		int cost = std::stoi(line.substr(0, pos));
		poisons[cost].push_back(name);
	}
	infile.close();

}

void Beastiary::buildDungeon(double apl){
	//step 1: get a Boss
	beast boss = getRandomBeast(apl, apl+1);
	std::ofstream outfile;
 	outfile.open("dungeons/" + boss.name +".txt", std::ofstream::out | std::ofstream::trunc);
	outfile << boss.name << "\t" << boss.cr << "\t" << boss.types[0] << std::endl << std::endl;
	while(rand() % 10 < 3){	
		trap bossTrap = itsATrap(boss.cr - 2);
		outfile << bossTrap.description << std::endl << std::endl;
	}
	//step 2: get encounters
	int dieRoll = rand() % 6 + rand() % 6 + rand() % 6 + rand() % 6 + 5;
	vector<string> encounter;
	for(int i = 0; i < dieRoll; i++){
		outfile << "\nencounter " << i << ":\n";
		encounter =  getEncounter(apl-1, apl, boss);
		for(size_t j = 0; j < encounter.size(); j++)
			outfile << encounter[j] << std::endl;
	}
}

