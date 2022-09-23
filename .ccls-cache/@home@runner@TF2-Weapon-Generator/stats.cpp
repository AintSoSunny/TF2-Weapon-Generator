#include "stats.hpp"
//this is messy but probably temporary >.>
int tempInt, mod, statCat, subStat;
const int statCatNum[4] = {1,8,5,4};
const string damTypes[4] = {"bullet", "explosive", "fire", "critical"};
const double damTypeRatings[4] = {4, 3.25, 2.5, 1.25};
const string critTypes[2] = {"critical hits", "mini-crits"};
const double critTypeRatings[2] = {200, 35};
const string conditions[5] = {"burning", "wet", "blast jumping", "slowed", "bleeding"};
const double conditionMods[5] = {.65, .25, .15, .05, .188};
const string posNegStringPair[7][2] = {
	{"",""},
	{"bonus", "penalty"},
	{"resistance", "vulnerability"},
	{"crits whenever it would normally mini-crit", "mini-crits whenever it would normally crit"},
	{"more", "less"},
	{"regenerated", "drained"},
	{"faster", "slower"}
};
const string posNegSign[3] = {"+", "-", ""};
Stat::Stat(int mod, int statCat, int subStat, string wepType, string wepSlot, string tfClass, bool whenActive, bool hasMeter, bool wearable){
	//var assigment
	posNegIndex = 0;
	posNegSignIndex = -1;
	numInterval = 5;
	//stat selection
	switch(statCat){
		case 0:
			//more & better attributes coming soon
			switch (subStat) {
				case 0:
					if (mod > 0) {
						statTemp = "This weapon reloads while holstered";
						ratingBase = 15;
					} else {
						statTemp = "This weapon fires tracer rounds";
						ratingBase = 5;
					};
			}
			break;
		case 1:
			type = damage;
			switch(subStat) {
				case 0:
					//damage bonus / penalty
					statTemp = "$n% damage $a";
					posNegIndex = 1;
					ratingBase = 5;
					break;
				case 1:
				//conditional damage bonus / penalty
					statTemp = "$n% damage $a on $c players";
					posNegIndex = 1;
					ratingBase = 3;
					break;
				case 2:
					/*minis to crits & visa versa
					lazy solution but shhh*/
					statTemp = "This weapon $a";
					ratingBase = 4.5;
					posNegIndex = 3;
					break;
				case 3:
					if (mod < 0) {
						statTemp = "No random critcal hits";
						ratingBase = 15;
					} else {
						//conditional crit/minicrit
					//the specific wording of this stat is EXTREMELY INCONSISTANT from weapon to weapon so I just chose one
						statTemp = "100% $r on $c players";
					}
					break;
				case 4:
					//bullet spread
					statTemp = "$n% $a accurate";
					ratingBase = 1.875;
					posNegIndex = 4;
					break;
				case 5:
					//Firing speed
					statTemp = "$n% firing speed";
					ratingBase = 4.25;
					break;
				case 6:
					//Clip size
					statTemp = "$n% clip size";
					ratingBase = 2.5;
					break;
				case 7:
					//reserve ammo
					statTemp = "$n% max $s ammo";
					ratingBase = .875;
					break;
				}
			break;
		case 2:
			type = defense;
			switch(subStat){
				case 0:
					//damage resistance
					statTemp = "$n% damage $a $w";
					ratingBase = 4.375;
					posNegIndex = 2;
					posNegSignIndex = 2;
					
					break;
				case 1:
					//damage type resistance
					statTemp = "$n% $d damage $a $w";
					posNegIndex = 2;
					posNegSignIndex = 2;
					break;
				case 2:
					//max HP
					statTemp = "$n max HP $w";
					numInterval = 2.5;
					ratingBase = 2;
					break;
				case 3:
					//max OVH
					statTemp = "$n% max overheal $w";
					ratingBase = .75;
					break;
				case 4:
					//HP regen (medic-like)
					statTemp = "$n HP $a per second $w";
					ratingBase = 9;
					posNegIndex = 5;
					numInterval = .75;
					break;
			}
			break;
		case 3:
			type = utility;
			switch(subStat) {
				case 0:
					statTemp = "$n% movement speed $w";
					ratingBase = 1.75;
					break;
				case 1:
					statTemp = "This weapon deploys $n% $a";
					ratingBase = 1.25;
					posNegIndex = 6;
					posNegSignIndex = 2;
					break;
				case 2:
					statTemp = "This weapon holsters $n% $a";
					ratingBase = 0.35;
					posNegIndex = 6;
					posNegSignIndex = 2;
					break;
				case 3:
					statTemp = "$n% switch speed $w";
					ratingBase = 1.75;
					posNegIndex = 6;
					break;
				case 4: 
					//the greatest & most op stat
					statTemp = "$n% jump height $w";
					ratingBase = 0.35;
			}
			break;
	}
	statString = statTemp; //copy template to main string 
	if (mod > 0) {
			upside = 0;
		}
	else {
			upside = 1;
		};
	if (posNegSignIndex < 0) {
		posNegSignIndex = upside;
	}
	//number/percentage value
	if (statString.find("$n") != string::npos) {
		tempString = to_string(round(abs(mod*numInterval)));
		tempString = tempString.substr(0, tempString.find(".")); //should remove leading zeros
		statString.replace(statString.find("$n"), 2, posNegSign[posNegSignIndex] + tempString);
	} else {
		mod /= abs(mod);
	}
	//pos/neg text
	if (statString.find("$a") != string::npos) {
		statString.replace(statString.find("$a"), 2, posNegStringPair[posNegIndex][upside]);
	}
	if (statString.find("$c") != string::npos) {
		tempInt = rand() % 4;
		statString.replace(statString.find("$c"), 2, conditions[tempInt]);
		ratingMod = conditionMods[tempInt];
	}
	if (statString.find("$r") != string::npos) {
		tempInt = rand() % 2;
		statString.replace(statString.find("$r"), 2, critTypes[tempInt]);
		ratingBase = critTypeRatings[tempInt];
	}
	if (statString.find("$d") != string::npos) {
		tempInt = rand() % 3;
		statString.replace(statString.find("$d"), 2, damTypes[tempInt]);
		ratingBase = damTypeRatings[tempInt];
	}
	if (statString.find("$w") != string::npos) {
		statString.replace(statString.find("$w"), 2, "on wearer");
		if (whenActive == true) {
			tempInt = distance(weaponSlots, find(begin(weaponSlots), end(weaponSlots), wepSlot));
			switch(tempInt) {
				case 0:
					ratingMod *= .8;
					break;
				case 1:
					ratingMod *= .45;
					break;
				case 2:
					ratingMod *= .2;
					break;
				default:
					ratingMod *= .1;
					break;
			}
		}
	}
	if (statString.find("$s") != string::npos) {
		statString.replace(statString.find("$s"), 2, wepSlot);
	}
	rating = (ratingBonus + ratingBase * mod) * ratingMod;
	cout << "/(mod = " << mod << "; ratingBase = " << ratingBase << "; ratingMod = " << ratingMod << "; ratingBonus = " << ratingBonus << ";\n rating = " << rating << ")\\...";
}
//1.35 hp on hit mult