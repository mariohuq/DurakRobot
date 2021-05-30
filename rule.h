#pragma once

#include "header.h"

class Rule {
private:
	const iCard* trump;
	bool istrump(const iCard*); // Return if it's trump card
	static const iCard* last(std::vector<const iCard*>&); // Return last card in desk
	static std::unordered_set<std::string> shown(std::vector<const iCard*>&); // Return list for shown card
		
public:
	Rule(const iCard*);
	std::vector<const iCard*> attack(std::vector<const iCard*>&, std::vector<const iCard*>&); // Analysis for attack
	std::vector<const iCard*> defend(std::vector<const iCard*>&, std::vector<const iCard*>&); // Analysis for defend
};
