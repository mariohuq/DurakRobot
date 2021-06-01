#pragma once

#include "header.h"

class Rule {
	static iCard* last(std::vector<iCard*>&); // Return last card in desk
	static std::unordered_set<std::string> shown_ranks(std::vector<iCard*>&); // Return list for shown card
		
public:
	static std::vector<iCard*> attack(std::vector<iCard*>&, std::vector<iCard*>&); // Analysis for attack
	static std::vector<iCard*> defend(std::vector<iCard*>&, std::vector<iCard*>&); // Analysis for defend
};
