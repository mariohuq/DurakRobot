
#include "card.h"
#include "rule.h"

bool Rule::istrump(const iCard* card) {
	return card->suit() == this->trump->suit();
}

const iCard* Rule::last(std::vector<const iCard*>& desk) {
	if (desk.size() == 0) return nullptr;
	return desk.back();
}

std::unordered_set<std::string> Rule::shown(std::vector<const iCard*>& desk) {
	std::unordered_set<std::string> shown;
	for (auto& card : desk)
		shown.insert(card->rank());
	return shown;
}

Rule::Rule(const iCard* trump) : trump(trump) {}

std::vector<const iCard*> Rule::attack(std::vector<const iCard*>& desk, std::vector<const iCard*>& inhand) {
	
	// If inhand is empty
	if (inhand.size() == 0) return std::vector<const iCard*>();

	// When the offense board's desktop is empty
	// you can play every cards.
	if (desk.size() == 0) return inhand;

	// Otherwise, you can only play cards that you have ever shown.
	std::vector<const iCard*> possible;
	std::unordered_set<std::string> shown = this->shown(desk);
	std::unordered_set<std::string>::iterator notexist = shown.end();

	for (auto& card : inhand) {
	
		// Deal with existed card
		const std::string rank = card->rank();
		if (shown.find(rank) != notexist)
			possible.push_back(card);
	}

	return possible;
}

std::vector<const iCard*> Rule::defend(std::vector<const iCard*>& desk, std::vector<const iCard*>& inhand) {

	// If inhand or desk is empty
	if (inhand.size() == 0) return inhand;

	// In defend module, you can play card with bigger or trump card
	std::vector<const iCard*> possible;
	const iCard* last = this->last(desk);

	std::string trump_suit = trump->suit();
	std::string last_suit = last->suit();
	bool defend_trump = (last_suit == trump_suit);

	for (auto& card : inhand) {

		std::string card_suit = card->suit();

		// When in the trump defensive mode, 
		// the current card needs to be greater than the last;
		if (defend_trump == true) {

			// If it is general card
			if (card_suit != trump_suit)
				continue;

			if (card > last)
				possible.push_back(card);

			continue;
		}

		// Otherwise, if the current card is a trump card, 
		// we can defend successfully;
		if (card_suit == trump_suit) {
			possible.push_back(card);
			continue;
		}

		// When both cards are not ace, 
		// check if the two cards are the same suit;
		if (card_suit == last_suit) {

			// Defensive success when the same suit 
			// and the current card is larger
			if (card->operator>(last))
				possible.push_back(card);

		}

	}

	return possible;
}
