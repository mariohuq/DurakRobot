#pragma once

#include "header.h"

class Rule {
	const iCard* trump;
	bool istrump(const iCard*); // Return if it's trump card
	static iCard* last(std::vector<iCard*>&); // Return last card in desk
	static std::unordered_set<std::string> shown(std::vector<iCard*>&); // Return list for shown card
		
public:
	Rule(const iCard*);
	std::vector<iCard*> attack(std::vector<iCard*>&, std::vector<iCard*>&); // Analysis for attack
	std::vector<iCard*> defend(std::vector<iCard*>&, std::vector<iCard*>&); // Analysis for defend
};
