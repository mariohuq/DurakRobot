
#include "card.h"
#include "rule.h"

bool Rule::istrump(const iCard* card) {
	return card->suit() == this->trump->suit();
}

iCard* Rule::last(std::vector<iCard*>& desk) {
	if (desk.empty()) return nullptr;
	return desk.back();
}

std::unordered_set<std::string> Rule::shown_ranks(std::vector<iCard*>& desk) {
	std::unordered_set<std::string> result;
	for (auto& card : desk)
		result.insert(card->rank());
	return result;
}

Rule::Rule(const iCard* trump) : trump(trump) {}

std::vector<iCard*> Rule::attack(std::vector<iCard*>& desk, std::vector<iCard*>& inhand) {
	
	// If inhand is empty
	if (inhand.empty()) return std::vector<iCard*>();

	// When the offense board's desktop is empty
	// you can play every cards.
	if (desk.empty()) return inhand;

	// Otherwise, you can only play cards that you have ever shown.
	std::vector<iCard*> possible;
	std::unordered_set<std::string> shown_ranks = this->shown_ranks(desk);

	for (auto& card : inhand) {
	
		// Deal with existed card
		const std::string rank = card->rank();
		if (shown_ranks.find(rank) != shown_ranks.end())
			possible.push_back(card);
	}

	return possible;
}

std::vector<iCard*> Rule::defend(std::vector<iCard*>& desk, std::vector<iCard*>& inhand) {

	// If inhand or desk is empty
	if (inhand.empty()) return std::vector<iCard*>();

	// In defend module, you can play card with bigger or trump card
	std::vector<iCard*> possible;
	const iCard* last = this->last(desk);

	std::string trump_suit = trump->suit();
	std::string last_suit = last->suit();
	bool defend_trump = (last_suit == trump_suit);

	for (iCard* card : inhand) {

		std::string card_suit = card->suit();

		// When in the trump defensive mode, 
		// the current card needs to be greater than the last;
		if (defend_trump) {

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
			if (card > last)
				possible.push_back(card);
		}
	}

	return possible;
}
